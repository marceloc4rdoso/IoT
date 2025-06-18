#include <DHT.h>
#include <WiFi.h>
#include <WebServer.h>
#include <FS.h> // Para sistema de arquivos SPIFFS
#include <SPIFFS.h> // Para sistema de arquivos SPIFFS

// Configurações da rede WiFi
const char* ssid = "SEMB2";        // Substitua pelo nome da sua rede WiFi
const char* password = "12345678"; // Substitua pela senha da sua rede WiFi

// Definir pino do LED
const int ledPin = 2;
bool ledStatus = false;
bool isBlinking = false;
unsigned long blinkInterval = 500; // Intervalo de piscar em ms (500ms = 0.5s)
unsigned long lastBlinkTime = 0;

// Definir pino e tipo do sensor DHT
#define DHTPIN 18     // Pino onde o sensor DHT está conectado (use um pino GPIO disponível)
#define DHTTYPE DHT11 // DHT11 ou DHT22
DHT dht(DHTPIN, DHTTYPE);

// Variáveis para armazenar os valores de temperatura e umidade
float temperature = 0.0;
float humidity = 0.0;
unsigned long lastDHTReadTime = 0;
const long dhtReadInterval = 2000; // Intervalo de leitura do sensor (2 segundos)

// Criar objeto do servidor web na porta 80
WebServer server(80);

// Protótipos das funções
void handleRoot();
void handleLedOn();
void handleLedOff();
void handleLedBlink();
void handleSensorRead();
void handleNotFound();
void readDHTSensor();

void setup() {
  // Iniciar comunicação serial
  Serial.begin(115200);

  // Configurar o pino do LED como saída
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Iniciar com o LED desligado

  // Iniciar o sensor DHT
  dht.begin();

  // Iniciar SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("Ocorreu um erro ao montar o SPIFFS");
    return;
  }

  // Conectar ao WiFi
  Serial.print("Conectando ao WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // Aguardar conexão
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Exibir informações da conexão
  Serial.println("");
  Serial.println("WiFi conectado!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  // Definir rotas do servidor web
  server.on("/", HTTP_GET, handleRoot);
  server.on("/on", HTTP_GET, handleLedOn);
  server.on("/off", HTTP_GET, handleLedOff);
  server.on("/blink", HTTP_GET, handleLedBlink); // Nova rota para piscar
  server.on("/read", HTTP_GET, handleSensorRead);
  server.onNotFound(handleNotFound);

  // Iniciar o servidor web
  server.begin();
  Serial.println("Servidor web iniciado!");

  // Fazer primeira leitura do sensor
  readDHTSensor();
}

void loop() {
  // Processar requisições do cliente
  server.handleClient();

  // Lógica para piscar o LED
  if (isBlinking) {
    unsigned long currentMillis = millis();
    if (currentMillis - lastBlinkTime >= blinkInterval) {
      lastBlinkTime = currentMillis;
      ledStatus = !ledStatus; // Inverte o estado do LED
      digitalWrite(ledPin, ledStatus ? HIGH : LOW);
    }
  }

  // Verificar se é hora de ler o sensor novamente
  unsigned long currentMillis = millis();
  if (currentMillis - lastDHTReadTime >= dhtReadInterval) {
    readDHTSensor();
    lastDHTReadTime = currentMillis;
  }
}

// Função para ler o sensor DHT
void readDHTSensor() {
  // Leitura de temperatura e umidade
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  // Verificar se as leituras são válidas
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Falha ao ler o sensor DHT!");
    return;
  }

  Serial.print("Umidade: ");
  Serial.print(humidity);
  Serial.print("%, Temperatura: ");
  Serial.print(temperature);
  Serial.println("°C");
}

// Função para a página principal (servindo do SPIFFS)
void handleRoot() {
  File file = SPIFFS.open("/index.html", "r");
  if (!file) {
    Serial.println("Falha ao abrir index.html");
    server.send(500, "text/plain", "Falha ao carregar a página.");
    return;
  }
  server.streamFile(file, "text/html");
  file.close();
}

// Função para ligar o LED
void handleLedOn() {
  isBlinking = false; // Para de piscar se estiver piscando
  ledStatus = true;
  digitalWrite(ledPin, HIGH);
  server.sendHeader("Location", "/");
  server.send(303);
}

// Função para desligar o LED
void handleLedOff() {
  isBlinking = false; // Para de piscar se estiver piscando
  ledStatus = false;
  digitalWrite(ledPin, LOW);
  server.sendHeader("Location", "/");
  server.send(303);
}

// Função para piscar o LED
void handleLedBlink() {
  isBlinking = !isBlinking; // Alterna o estado de piscar
  if (!isBlinking) {
    digitalWrite(ledPin, LOW); // Garante que o LED esteja desligado ao parar de piscar
    ledStatus = false;
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

// Função para atualizar a leitura do sensor e redirecionar
void handleSensorRead() {
  readDHTSensor();
  server.sendHeader("Location", "/");
  server.send(303);
}

// Função para página não encontrada
void handleNotFound() {
  String message = "Página não encontrada\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMétodo: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArgumentos: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
}
