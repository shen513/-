#define IR A0
#define THRESHOLD 900
#define OR_FAN_IN 7
#define DOOR_GATE 8

#define DEBUG(v) \ 
  Serial.println(v)

void unlockDoor()
{
  digitalWrite(DOOR_GATE, LOW);
}
void lockDoor()
{
  digitalWrite(DOOR_GATE, HIGH);
}

void setup() {
  Serial.begin(1000000);
  while(!Serial);
  
  pinMode(IR, INPUT);
  pinMode(DOOR_GATE, OUTPUT);
  pinMode(OR_FAN_IN, INPUT);
  
   DEBUG("init");
}

void loop() {
  
      Open();
      Close();
}


bool isDoorOpened() {
  bool current_state = analogRead(IR) > THRESHOLD;
  delay(100);
  return current_state;
}


void Open() {
  bool requestOpenSignal = digitalRead(OR_FAN_IN);
  
  if (requestOpenSignal)
  {
    unlockDoor();

    bool neverOpen = true;  //門從未被打開
    for (int count = 1; count <= 30; count++)
    {
      bool state = isDoorOpened();  //狀態為開門

      if (state)
      {
        neverOpen = false;
        break;
      }
    }

    if (neverOpen)  //門從未被打開，等待門關閉
    {
        lockDoor();
    }
    else
    {
      // todo: waiting somebody close door
    }
  }
}
void Close() {
  if (!isDoorOpened())
  {
    lockDoor();
  }
}
