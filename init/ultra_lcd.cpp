//Este programa faz a medição da distância entre o sensor HC-SR04 e um objeto à sua frente.
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define o endereço I2C do seu LCD (0x27 é o mais comum) e o número de colunas e linhas.
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define TRIG 18
#define ECHO 2
#define BUZZER 4 // Define o pino do buzzer
int distancia;

// Definindo os caracteres personalizados (emojis)
byte okChar[] = { // Caractere para "OK" (Checkmark)
  0b00000,
  0b00001,
  0b00011,
  0b10110,
  0b11100,
  0b01000,
  0b00000,
  0b00000
};

byte dangerChar[] = { // Caractere para "Perigo" (Triângulo com exclamação)
  0b00100,
  0b01110,
  0b11111,
  0b00100,
  0b00100,
  0b00000,
  0b00100,
  0b00000
};

void setup() {
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT);
  pinMode(BUZZER, OUTPUT); // Configura o pino do buzzer como saída
  pinMode(ECHO, INPUT);

  lcd.init();      // Inicializa o LCD
  lcd.backlight(); // Liga a luz de fundo do LCD
  lcd.print("Iniciando sensor"); // Mensagem de inicialização no LCD
  lcd.setCursor(0, 1);
  lcd.print("aguarde...");
  delay(2000); // Aguarda 2 segundos para a mensagem inicial
}

void loop() {
  medir(); // Chama a função para medir a distância

  // Exibe a distância no Serial Monitor (para depuração)
  Serial.print("Distancia em CM: ");
  Serial.println(distancia);

  // Limpa o display para novas mensagens
  lcd.clear();

  // Lógica para exibir mensagens no LCD com base na distância
  if (distancia > 30) { // Se a distância for maior que 30 cm
    lcd.print("Tudo Ok!");
    digitalWrite(BUZZER, LOW); // Desliga o buzzer
  } else if (distancia <= 30 && distancia > 10) { // Se a distância estiver entre 10 e 30 cm
    lcd.print("Cuidado!");
    lcd.setCursor(0, 1); // Move o cursor para a segunda linha
    lcd.print(distancia); // Exibe a distância
    lcd.print(" cm");
    digitalWrite(BUZZER, HIGH); // Liga o buzzer
    delay(50); // Liga o buzzer por um curto período
    digitalWrite(BUZZER, LOW); // Desliga o buzzer
    delay(50);
  } else { // Se a distância for 10 cm ou menos (colisão iminente)
    lcd.print("COLISAO!");
    lcd.setCursor(0, 1); // Move o cursor para a segunda linha
    lcd.print(distancia); // Exibe a distância
    lcd.print(" cm");
    digitalWrite(BUZZER, HIGH); // Liga o buzzer continuamente para alerta máximo
  }

  delay(300); // Pequeno atraso para estabilidade e leitura
}

void medir() {
  // Garante que o TRIG esteja em LOW antes de enviar o pulso
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  // Envia um pulso HIGH de 10 microssegundos no pino TRIG
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  // Mede a duração do pulso HIGH no pino ECHO
  // O tempo em microsegundos que o som leva para ir e voltar
  distancia = pulseIn(ECHO, HIGH);

  // Calcula a distância em centímetros
  // A velocidade do som no ar é de aproximadamente 0.0343 cm/microsegundo.
  // Dividimos por 2 porque o tempo é para ir e voltar.
  distancia = distancia * 0.0343 / 2;
}
