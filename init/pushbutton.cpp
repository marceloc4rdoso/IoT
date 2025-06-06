#define LED_RED 18
#define LED_GRE 21

#define BTN_1 34
#define BTN_2 35

bool read1;
bool read2;

void setup() {

  Serial.begin(9600);

  pinMode(BTN_1, INPUT);
  pinMode(BTN_2, INPUT);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GRE, OUTPUT);

}

void loop() {
  read1 = digitalRead(BTN_1);
  read2 = digitalRead(BTN_2);

  Serial.print("BTN_1: ");
  Serial.println(read1);

  if (read1  == HIGH) digitalWrite(LED_GRE, HIGH);
  else digitalWrite(LED_GRE, LOW);

  if (read2  == HIGH) digitalWrite(LED_RED, HIGH); 
  else digitalWrite(LED_RED, LOW);

  if (read1 == HIGH && read2 == HIGH){
    digitalWrite(LED_GRE, HIGH);
    digitalWrite(LED_GRE, LOW);
    delay(300);
  }



}
