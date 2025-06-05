// DIRETIVA DE DEFINIÇÃO
/*
#define LED_RED 18
#define LED_YEL 19
#define LED_GRE 21
*/

const int LED_RED = 18;
const int LED_YEL = 19;
const int LED_GRE = 21;

int pinoPWM = LED_GRE;


void setup() {

  Serial.begin(9600);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YEL, OUTPUT);
  pinMode(LED_GRE, OUTPUT);

}

// the loop function runs over and over again forever
void loop() {
 
  for (int i = 0; i < 10; i++) {

        Serial.println("Green" + i);
        digitalWrite(LED_GRE, HIGH);
        delay(500);
        digitalWrite(LED_GRE, LOW);
        delay(500);
  }
  for (int i = 0; i < 5; i++) {
        Serial.println("Red" + i);
        digitalWrite(LED_RED, HIGH);
        delay(500);
        digitalWrite(LED_RED, LOW);
        delay(500);
  }
  for (int i = 0; i < 2; i++) {
        Serial.println("Yellow" + i);
        digitalWrite(LED_YEL, HIGH);
        delay(500);
        digitalWrite(LED_YEL, LOW);
        delay(500);
  }

  /*
  digitalWrite(LED_GRE, HIGH);
  digitalWrite(LED_GRE, LOW);
  delay(300);
  digitalWrite(LED_YEL, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(300);                      // wait for a second
  digitalWrite(LED_YEL, LOW);   // turn the LED off by making the voltage LOW
  delay(300);                      // wait for a second
  digitalWrite(LED_RED, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second
  digitalWrite(LED_RED, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);
  */ 
}
