#include "SPI.h"
#include "MFRC522.h"

#define SS_PIN 10 //晶片選擇腳位
#define RST_PIN 9 //設定重置腳位

#define GATE 6
#define IR_SENSOR A6

#define ENABLE HIGH

#define ABSTRACT 20
#define THRESHOLD 13
#define DELAY_MS 100
#define OR_FAN_IN 7

MFRC522 rfid(SS_PIN, RST_PIN); //建立MFRC522物件

bool isLock = false;
void setup() 
{
pinMode(OR_FAN_IN, INPUT);
  
  //RFID
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  //IR_Sensor
  pinMode(IR_SENSOR, INPUT_PULLUP);
  pinMode(GATE, OUTPUT);
  Lock();
  //Lock_Door(ABSTRACT, THRESHOLD);
}

void loop() 
{
  if ((Serial.available() &&  Serial.read() == '1') || Validate_NFC() || digitalRead(OR_FAN_IN) )
  {
      Open_Door();
      isLock = false;
      Lock();
      // Lock_Door(ABSTRACT, THRESHOLD);
  }
}

byte cardID[] = {0xA0, 0x79, 0x7A, 0xA2}; //固定rfid卡號


bool Validate_NFC()
{
  //
  if (rfid.PICC_IsNewCardPresent()  && rfid.PICC_ReadCardSerial())
  {
    byte *id = rfid.uid.uidByte;   // 取得卡片的UID
    byte idSize = rfid.uid.size;   // 取得UID的長度

    //設定驗證指標為 true
    bool validate = true;
    //判斷卡片的id是否與cardID一致
    for (int i = 0; i < idSize; i++)
    {
      // 如果其中一個id不符合cardID中對應的id
      if (cardID[i] != id[i])
      {
        //設定驗證指標為 false
        validate = false;
        //跳出迴圈
        break;
      }
    }
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
    return validate;
  }
  return false;
}

void Open_Door()
{
  Unlock();
  // digitalWrite(GATE, ENABLE);
  bool neverOpen = true;
  short abstract = 0;
  for (int i = 0; i < 30; i++)
  {
    int ir =  analogRead(IR_SENSOR) ;
    Serial.println("IR: " + String(ir));
    abstract += ir;
    if (i % 10 == 0 && abstract / 10 > THRESHOLD)
    {
      Serial.println("avr: " + String( abstract / 10));
      //門打開
      neverOpen = false;
      break;
    }
    else
    {
      abstract ^= abstract;
      //門沒開
      delay(100);
    }
  }

  Serial.println("neverOpen: " + String(neverOpen ? "Never Open" : "Opened"));

  if (!neverOpen)
  {
    //直接上鎖
    return;
  }
  else
  {
    //等待門關閉
    while (true)
    {
      if ( Measuring_Distance(5, THRESHOLD) < 0)
        break;
    }
  }
}

// 回傳 > 0 : 門沒關好 ; < 0 門已關好
int Measuring_Distance(int abstract, int threshold)
{
  int summary = 0;
  for (int count = 0; count < abstract; count++)
  {
    summary += analogRead(IR_SENSOR);
    delay(DELAY_MS);
  }
  int state = summary / abstract;
  return (state - threshold);
}

void Lock()
{
  digitalWrite(GATE, !ENABLE);
}

void Unlock()
{
  digitalWrite(GATE, ENABLE);
}

/*
void Lock_Door(int abstract, int threshold)
{
  if (isLock)
    return;

  bool enable = Measuring_Distance(abstract, threshold) > 0;
  Serial.println("Set GATE state: " + String(enable ?  "Open" :  "Close"));
  digitalWrite(GATE, enable);
  isLock = true;
}*/
