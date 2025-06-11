#include <LiquidCrystal.h>

//Este programa faz a medição da distância entre o sensor HC-SR04 e um objeto à sua frente.
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define TRIG 18
#define ECHO 2
#define BUZZER 4
int distancia;
int ok = 30;
int cuidado = 10;


void setup() {
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(ECHO, INPUT);

  lcd.init();
  lcd.backlight();
  lcd.print("ESP32 - Projeto");
  lcd.setCursor(0, 1);
  lcd.print("com display!");

}

void loop() {
  // put your main code here, to run repeatedly:
  medir();
  Serial.print("Distancia em CM:");
  Serial.println(distancia);
  //detectar();
}


void medir() {
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  // lê o resultado:
  distancia = pulseIn(ECHO, HIGH);
  distancia = distancia * 0.0343 / 2;  // duração do pulso X velocidade do som no ar a 20°C (343 m/s)/ 2distance/58;


  if (distancia > 30) {
    Serial.println("Tudo OK");
    digitalWrite(BUZZER, LOW);
  }
  else if (distancia >= 10 && distancia <= 30) {
    Serial.println("CUIDADO!");
    digitalWrite(BUZZER, LOW);
  } else {
    Serial.println("Colisão!");
    digitalWrite(BUZZER, HIGH);
  }

  delay(300);
}
