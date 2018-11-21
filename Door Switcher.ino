
#define GATE 6
#define IR_SENSEOR A6
void setup() {
  Serial.begin(9600);
  pinMode(IR_SENSEOR, INPUT_PULLUP);
  pinMode(GATE, OUTPUT);
}

void loop() {

  Lock_Door(20,10);
 // if(user_request())
   Open_Door();
  
}

void Open_Door()
{
  Serial.println("Open Door");
  digitalWrite(GATE, LOW);
  Serial.println("Delay 3 seconds");
  delay(3000);
}

void Lock_Door(int abstract, int threshold)
{
  Serial.println("Lock Door");
  const int DELAY_MS = 100;
  int summary = 0;
  Serial.println("Evaluate state");
  for (int count = 0; count < abstract; count++)
  {
    summary += analogRead(IR_SENSEOR);
    delay(DELAY_MS);
  }
  

  int state = summary / abstract;
  Serial.println("state: "+ String(state));
  
  Serial.println("Set GATE state");
  digitalWrite(GATE, (state > threshold) ? LOW : HIGH);
}
