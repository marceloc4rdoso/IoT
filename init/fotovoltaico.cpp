#define LED 18
#define ldr 32

int leitura_ldr = 0;
float tensao_ldr = 0; 

void setup() {
   
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
}

void loop() {
  
  leitura_ldr = analogRead(ldr);
  Serial.println(leitura_ldr);
  tensao_ldr =  (3.3 *  leitura_ldr)/4096;
  Serial.print("Tensão: ");
  Serial.println(tensao_ldr);
  delay(1000); 

  if (tensao_ldr >= 1.65){
    Serial.print("Acende: ");
    digitalWrite(LED, HIGH);
  } 
  else {
    Serial.print("Apaga: ");
    digitalWrite(LED, LOW);
    delay(200);
  }
}
