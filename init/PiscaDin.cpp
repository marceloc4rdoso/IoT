
#define pinPot 32
#define LED_RED 18

int readPot;
float result;

void setup() {
  Serial.begin(9600); // Em pinos analogicos não se usa pinMode
  pinMode(LED_RED, OUTPUT);
}

void loop() {
  readPot  = analogRead(pinPot);
  Serial.println(readPot);
  result = (3.3 / 4095)*readPot;
  Serial.print("Volts: ");
  Serial.println(result) ;
  delay(300);

  if(readPot >= 2048){
    digitalWrite(LED_RED, HIGH);
  }
  else digitalWrite(LED_RED, LOW);
}
