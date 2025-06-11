// Este programa faz a medição da distância entre o sensor HC-SR04 e um objeto à sua frente,
// exibindo as informações em um display OLED de 128x64 pixels.

#include <Wire.h>          // Necessário para comunicação I2C
#include <Adafruit_GFX.h>  // Biblioteca gráfica da Adafruit
#include <Adafruit_SSD1306.h> // Driver para o display OLED SSD1306

// Configuração do display OLED
#define SCREEN_WIDTH 128 // Largura do display OLED em pixels
#define SCREEN_HEIGHT 64 // Altura do display OLED em pixels

// Declaração do objeto display SSD1306 conectado via I2C (SDA, SCL pins)
// O pino de reset é -1 para displays I2C, indicando que o reset é via energia
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define TRIG 18
#define ECHO 2
#define BUZZER 4 // Define o pino do buzzer
int distancia;

void setup() {
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT);
  pinMode(BUZZER, OUTPUT); // Configura o pino do buzzer como saída
  pinMode(ECHO, INPUT);

  // Inicializa o display OLED com o endereço I2C padrão 0x3C ou 0x3D
  // Tente 0x3C primeiro. Se não funcionar, tente 0x3D.
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Endereço I2C 0x3C para muitos módulos
    Serial.println(F("Alocação SSD1306 falhou ou display não encontrado."));
    while(1);
  }

  // Limpa o buffer do display
  display.clearDisplay();

  // Configurações de texto iniciais para o OLED
  display.setTextSize(2);      // Tamanho do texto (1 é o menor, aumenta em múltiplos)
  display.setTextColor(SSD1306_WHITE); // Cor do texto (pixels brancos)
  display.setCursor(0,2);      // Posição inicial do cursor (coluna, linha)

  display.println("ESP32 - Projeto");
  display.println("com OLED!");
  display.display(); // Envia o buffer para o display para mostrar o texto
  delay(2000); // Aguarda 2 segundos para a mensagem inicial
}

void loop() {
  medir(); // Chama a função para medir a distância

  // Exibe a distância no Serial Monitor (para depuração)
  Serial.print("Distancia em CM: ");
  Serial.println(distancia);

  // Limpa o buffer do display antes de desenhar novas informações
  display.clearDisplay();

  // Lógica para exibir mensagens no OLED com base na distância
  if (distancia > 30) { // Se a distância for maior que 30 cm
    display.setTextSize(2); // Texto maior para "Tudo Ok!"
    display.setCursor(10, 20); // Centraliza a mensagem
    display.println("Tudo Ok!");
    digitalWrite(BUZZER, LOW); // Desliga o buzzer
  } else if (distancia <= 30 && distancia > 10) { // Se a distância estiver entre 10 e 30 cm
    display.setTextSize(2);
    display.setCursor(10, 0);
    display.println("Cuidado!");
    display.setTextSize(1); // Texto menor para a distância
    display.setCursor(0, 30);
    display.print("Distancia: ");
    display.print(distancia);
    display.println(" cm");
    digitalWrite(BUZZER, HIGH); // Liga o buzzer
    delay(50); // Liga o buzzer por um curto período
    digitalWrite(BUZZER, LOW); // Desliga o buzzer
    delay(50);
  } else { // Se a distância for 10 cm ou menos (colisão iminente)
    display.setTextSize(3); // Texto bem grande para "COLISAO!"
    display.setTextColor(SSD1306_WHITE); // Garante a cor branca (ou invertida se preferir)
    display.setCursor(5, 15);
    display.println("COLISAO!");
    display.setTextSize(1); // Texto menor para a distância
    display.setCursor(0, 50);
    display.print("Dist: ");
    display.print(distancia);
    display.println(" cm");
    digitalWrite(BUZZER, HIGH); // Liga o buzzer continuamente para alerta máximo
  }

  display.display(); // ATENÇÃO: É ESSENCIAL chamar display.display() no final do loop
                     // para que as mudanças no buffer sejam mostradas no OLED.
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
  distancia = pulseIn(ECHO, HIGH);

  // Calcula a distância em centímetros
  distancia = distancia * 0.0343 / 2;
}
