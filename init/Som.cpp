//Este programa faz a medição da distância entre o sensor HC-SR04 e um objeto à sua frente.

#define TRIG 16
#define ECHO 2
#define BUZZER 4
int distancia;

void setup() {
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(ECHO, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
medir();
Serial.print("Distancia em CM:");
Serial.println(distancia);
}


void medir(){
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  // lê o resultado:
  distancia = pulseIn(ECHO, HIGH);
  distancia = distancia * 0.0343 / 2; // duração do pulso X velocidade do som no ar a 20°C (343 m/s)/ 2distance/58;
  delay(300);
}




//dis (velocidade do som * tempo)/2
