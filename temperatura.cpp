/*
  ESP32 com Bluetooth Serial, DHT11 e LED

  Funcionalidades:
  - Controle de LED via Bluetooth
  - Monitoramento de temperatura e umidade
  - Comandos por texto via Bluetooth Serial Terminal

  Comandos disponíveis:
  - "led on" ou "1" = Liga o LED
  - "led off" ou "0" = Desliga o LED
  - "temp" ou "t" = Mostra temperatura
  - "umid" ou "u" = Mostra umidade
  - "dados" ou "d" = Mostra temperatura e umidade
  - "help" ou "h" = Lista comandos
  - "status" ou "s" = Status do sistema

  Conexões:
  - DHT11: Pino de dados no GPIO 4
  - LED: GPIO 2 (LED interno do ESP32)
  - Resistor de 10kΩ entre VCC e pino de dados do DHT11
*/

#include "BluetoothSerial.h"
#include "DHT.h"

// Definições dos pinos
#define DHT_PIN 4      // Pino do sensor DHT11
#define LED_PIN 2      // Pino do LED (LED interno do ESP32)
#define DHT_TYPE DHT11 // Tipo do sensor

// Objetos
BluetoothSerial SerialBT;
DHT dht(DHT_PIN, DHT_TYPE);

// Variáveis globais
bool ledStatus = false;
float temperatura = 0.0;
float umidade = 0.0;
unsigned long ultimaLeitura = 0;
const unsigned long INTERVALO_LEITURA = 2000; // 2 segundos
String comando = "";

void setup() {
  Serial.begin(115200);

  // Inicializar Bluetooth
  SerialBT.begin("ESP32_DHT11_LED"); // Nome do dispositivo Bluetooth
  Serial.println("ESP32 Bluetooth iniciado! Procure por 'ESP32_DHT11_LED'");

  // Configurar pinos
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Inicializar sensor DHT11
  dht.begin();

  // Mensagens iniciais
  delay(1000);
  Serial.println("Sistema iniciado!");
  Serial.println("Comandos disponíveis via Bluetooth:");
  Serial.println("- led on/off, temp, umid, dados, help, status");

  // Primeira leitura
  lerSensorDHT();

  // Enviar mensagem de boas-vindas via Bluetooth
  SerialBT.println("=== ESP32 DHT11 + LED ===");
  SerialBT.println("Sistema iniciado!");
  SerialBT.println("Digite 'help' para ver comandos");
  SerialBT.println("Temperatura: " + String(temperatura) + "°C");
  SerialBT.println("Umidade: " + String(umidade) + "%");
  SerialBT.println("LED: " + String(ledStatus ? "LIGADO" : "DESLIGADO"));
  SerialBT.println("========================");
}

void loop() {
  // Ler sensor DHT11 periodicamente
  if (millis() - ultimaLeitura >= INTERVALO_LEITURA) {
    lerSensorDHT();
    ultimaLeitura = millis();
  }

  // Processar comandos Bluetooth
  if (SerialBT.available()) {
    comando = SerialBT.readStringUntil('\n');
    comando.trim(); // Remove espaços extras
    comando.toLowerCase(); // Converte para minúsculas

    processarComando(comando);
  }

  // Esperar um pouco antes do próximo loop
  delay(100);
}

void lerSensorDHT() {
  float novaTemp = dht.readTemperature();
  float novaUmid = dht.readHumidity();

  // Verificar se as leituras são válidas
  if (isnan(novaTemp) || isnan(novaUmid)) {
    Serial.println("Erro ao ler sensor DHT11!");
    return;
  }

  temperatura = novaTemp;
  umidade = novaUmid;

  // Log no monitor serial
  Serial.println("Leitura DHT11 - Temp: " + String(temperatura) + "°C, Umid: " + String(umidade) + "%");
}

void processarComando(String cmd) {
  Serial.println("Comando recebido: " + cmd);

  // Comandos para controlar LED
  if (cmd == "led on" || cmd == "1" || cmd == "ligar") {
    ligarLED();
  }
  else if (cmd == "led off" || cmd == "0" || cmd == "desligar") {
    desligarLED();
  }

  // Comandos para leitura de sensores
  else if (cmd == "temp" || cmd == "t" || cmd == "temperatura") {
    mostrarTemperatura();
  }
  else if (cmd == "umid" || cmd == "u" || cmd == "umidade") {
    mostrarUmidade();
  }
  else if (cmd == "dados" || cmd == "d" || cmd == "sensores") {
    mostrarDados();
  }

  // Comandos de informação
  else if (cmd == "status" || cmd == "s") {
    mostrarStatus();
  }
  else if (cmd == "help" || cmd == "h" || cmd == "ajuda") {
    mostrarAjuda();
  }

  // Comando não reconhecido
  else {
    SerialBT.println("Comando não reconhecido: " + cmd);
    SerialBT.println("Digite 'help' para ver comandos disponíveis");
  }
}

void ligarLED() {
  digitalWrite(LED_PIN, HIGH);
  ledStatus = true;
  SerialBT.println("LED LIGADO");
  Serial.println("LED ligado via Bluetooth");
}

void desligarLED() {
  digitalWrite(LED_PIN, LOW);
  ledStatus = false;
  SerialBT.println("LED DESLIGADO");
  Serial.println("LED desligado via Bluetooth");
}

void mostrarTemperatura() {
  SerialBT.println("Temperatura: " + String(temperatura) + "°C");

  // Adicionar emoji baseado na temperatura
  if (temperatura < 20) {
    SerialBT.println("Status: Frio ❄️");
  }
  else if (temperatura < 25) {
    SerialBT.println("Status: Agradável 😊");
  }
  else if (temperatura < 30) {
    SerialBT.println("Status: Quente ☀️");
  }
  else {
    SerialBT.println("Status: Muito quente 🔥");
  }
}

void mostrarUmidade() {
  SerialBT.println("Umidade: " + String(umidade) + "%");

  // Adicionar status baseado na umidade
  if (umidade < 30) {
    SerialBT.println("Status: Muito seco");
  }
  else if (umidade < 50) {
    SerialBT.println("Status: Seco");
  }
  else if (umidade < 70) {
    SerialBT.println("Status: Ideal");
  }
  else {
    SerialBT.println("Status: Úmido");
  }
}

void mostrarDados() {
  SerialBT.println("=== DADOS DOS SENSORES ===");
  SerialBT.println("Temperatura: " + String(temperatura) + "°C");
  SerialBT.println("Umidade: " + String(umidade) + "%");

  // Calcular índice de conforto térmico simplificado
  float indiceConforto = temperatura - (umidade / 100.0) * 5;
  SerialBT.println("Índice de conforto: " + String(indiceConforto, 1));

  if (indiceConforto < 18) {
    SerialBT.println("Conforto: Desconfortável (frio)");
  }
  else if (indiceConforto < 24) {
    SerialBT.println("Conforto: Confortável");
  }
  else {
    SerialBT.println("Conforto: Desconfortável (quente)");
  }

  SerialBT.println("LED: " + String(ledStatus ? "LIGADO" : "DESLIGADO"));
  SerialBT.println("========================");
}

void mostrarStatus() {
  SerialBT.println("=== STATUS DO SISTEMA ===");
  SerialBT.println("Dispositivo: ESP32");
  SerialBT.println("Bluetooth: Conectado");
  SerialBT.println("Sensor DHT11: OK");
  SerialBT.println("LED: " + String(ledStatus ? "LIGADO" : "DESLIGADO"));
  SerialBT.println("Última leitura: " + String((millis() - ultimaLeitura) / 1000) + "s atrás");
  SerialBT.println("Uptime: " + String(millis() / 1000) + " segundos");
  SerialBT.println("Memória livre: " + String(ESP.getFreeHeap()) + " bytes");
  SerialBT.println("========================");
}

void mostrarAjuda() {
  SerialBT.println("=== COMANDOS DISPONÍVEIS ===");
  SerialBT.println("CONTROLE DO LED:");
  SerialBT.println("  led on, 1, ligar    - Liga o LED");
  SerialBT.println("  led off, 0, desligar - Desliga o LED");
  SerialBT.println("");
  SerialBT.println("SENSORES:");
  SerialBT.println("  temp, t    - Mostra temperatura");
  SerialBT.println("  umid, u    - Mostra umidade");
  SerialBT.println("  dados, d   - Mostra todos os dados");
  SerialBT.println("");
  SerialBT.println("INFORMAÇÕES:");
  SerialBT.println("  status, s  - Status do sistema");
  SerialBT.println("  help, h    - Esta ajuda");
  SerialBT.println("");
  SerialBT.println("Exemplo: digite 'temp' e pressione Enter");
  SerialBT.println("===========================");
}
