// UID do cartão:  E7 9A 50 73

#include <SPI.h>
#include <MFRC522.h>

// Define os pinos para o módulo RC522
#define SS_PIN  5
#define RST_PIN 22

MFRC522 mfrc522(SS_PIN, RST_PIN); // Cria uma instância da classe MFRC522


void setup() {
 
  Serial.begin(9600); // Inicializa a comunicação serial
  SPI.begin();        // Inicializa o barramento SPI
  mfrc522.PCD_Init(); // Inicializa o módulo MFRC522
  Serial.println(F("Aproxime o cartão RFID do leitor..."));


}

void loop() {
  // Procura por novos cartões
  if (mfrc522.PICC_IsNewCardPresent()) {
    // Seleciona um dos cartões
    if (mfrc522.PICC_ReadCardSerial()) {
      Serial.print(F("UID do cartão: "));
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
      }
      Serial.println();

      // Para a leitura do cartão atual
      mfrc522.PICC_HaltA();
    }
  }
  // Evita leituras repetidas do mesmo cartão
  mfrc522.PCD_StopCrypto1();
}
