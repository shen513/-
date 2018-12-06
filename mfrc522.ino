#include "SPI.h"
#include "MFRC522.h"

#define SS_PIN 10 //晶片選擇腳位
#define RST_PIN 9 //設定重置腳位

#define SIGNAL 4 //訊號輸出腳位
#define ENABLE HIGH //定義預設電位
#define DELAY_MS 1000 //輸出觸發持續時間，單位為ms

MFRC522 rfid(SS_PIN, RST_PIN); //建立MFRC522物件

void setup() {
  Serial.begin(1000000);
  pinMode(SIGNAL, OUTPUT);
  
    digitalWrite(SIGNAL,!ENABLE);
  SPI.begin();
  rfid.PCD_Init();
  Serial.println("init completed");
}
byte cardID[] = {0xA0, 0x79, 0x7A, 0xA2};//固定rfid卡號 
void loop() {
  //
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;
    
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

  //如果卡片通過驗證
  if (validate)
  {
    Serial.println("Trig: On");
    digitalWrite(SIGNAL,ENABLE);
   
    Serial.println("wait a moment");
    delay(DELAY_MS);
    
    Serial.println("Trig: Off");
    digitalWrite(SIGNAL, !ENABLE);
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
