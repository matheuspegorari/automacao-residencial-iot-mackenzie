// Bibliotecas
#include <Adafruit_PN532.h> //biblioteca do módulo RFID
#include <WiFi.h> //biblioteca para utilizar o WiFi
#include "ThingSpeak.h" //biblioteca do broker MQTT ThingSpeak
#include "DHT.h" //biblioteca do sensor de temperatura/umidade DHT22

// Constantes
#define SDA_PIN 21 //Utilizado pino D21 do ESP32
#define SCL_PIN 22 //Utilizado pino D22 do ESP32
#define DHTPIN 18 //Utilizado pino D18 do ESP32
#define DHTTYPE DHT22 //Definindo tipo do sensor DHT
#define LDR 34 //Utilizado pino D34 do ESP32
#define RELAY_LIGHT 27 //Utilizado pino D27 do ESP32
#define RELAY_AC 26 //Utilizado pino D26 do ESP32
#define RELAY_DOOR 25 //Utilizado pino D25 do ESP32

// Configuração do ThingSpeak
const char* myWriteAPIKey = "TS_WRITE_API_KEY";
unsigned long myChannelNumber = TS_CHANELL_NUM;

// Configuração WiFi
const char ssid[] = "NOME_REDE_WIFI";
const char pass[] = "SENHA_WIFI";
WiFiClient client;

// Configuração RFID
Adafruit_PN532 nfc(SDA_PIN, SCL_PIN);

// Sensor DHT
DHT dht(DHTPIN, DHTTYPE);

// Sensor LDR
int sensorValue = 0; // Sensor de Luminosidade

// Condições
float tempThreshold = 25.0;  // Temperatura mínima
float humiThreshold = 30.0;  // Umidade mínima
int lightThreshold = 1500;    // Intensidade de luz mínima

void setup() {
  // Iniciar a comunicação serial
  Serial.begin(115200);
  delay(5000);
  Serial.println("Serial começou");

  // Configurar WiFi
  WiFi.begin(ssid, pass);
  Serial.println("WiFi iniciado!");

  // Iniciar ThingSpeak
  ThingSpeak.begin(client);
  Serial.println("ThingSpeak iniciado!");

  // Configurar sensor DHT
  dht.begin();
  Serial.println("DHT iniciado!");

  // Configurar leitor NFC
  nfc.begin();
  nfc.SAMConfig();
  Serial.println("NFC iniciado!");

  // Configurar pinos do relé como saída
  pinMode(RELAY_LIGHT, OUTPUT);
  pinMode(RELAY_AC, OUTPUT);
  pinMode(RELAY_DOOR, OUTPUT);

  // Inicialmente desligar os relés
  digitalWrite(RELAY_LIGHT, LOW);
  digitalWrite(RELAY_AC, LOW);
  digitalWrite(RELAY_DOOR, LOW);

  Serial.println("Iniciado com Sucesso!");
}



void loop(void) {

  // Ler valores do sensor DHT
  float humi = dht.readHumidity();
  float temp = dht.readTemperature();
  // Ler valor do sensor LDR
  sensorValue = analogRead(LDR);

  Serial.println("Luminosidade: " + String(sensorValue));
  Serial.println("Temperatura: " + String(temp));
  Serial.println("Umidade: " + String(humi));

  Serial.println("Aguardando leiturada Tag...");

  // Verificar tag RFID
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  String tagID = String(uid[0], HEX) + String(uid[1], HEX) + String(uid[2], HEX) + String(uid[3], HEX);

  if (success) {
    // Se a tag RFID for lida, iniciar a sequência
    Serial.println("Acesso de: " + String(tagID) + " ao ambiente");

    // Marcando o tempo inicial para o sensor DHT
    unsigned long startTimeDHT = millis();

    // Ler valores do sensor DHT
    float humi = dht.readHumidity();
    float temp = dht.readTemperature();

    // Calcule o tempo gasto para ler os valores do sensor DHT
    unsigned long readTimeDHT = millis() - startTimeDHT;
    Serial.println("Tempo para ler o sensor DHT: " + String(readTimeDHT) + " ms");
    Serial.println("Temperatura: " + String(temp));
    Serial.println("Umidade: " + String(humi));

    // Marcando o tempo inicial para o sensor LDR
    unsigned long startTimeLDR = millis();

    // Ler valor do sensor LDR
    sensorValue = analogRead(LDR);

    // Calcule o tempo gasto para ler o valor do sensor LDR
    unsigned long readTimeLDR = millis() - startTimeLDR;
    Serial.println("Tempo para ler o sensor LDR: " + String(readTimeLDR) + " ms");
    Serial.println("Luminosidade: " + String(sensorValue));

    // Marcar o tempo antes de escrever para ThingSpeak
    unsigned long startTimeTS = millis();

    // Escrever valores no ThingSpeak
    ThingSpeak.setField(1, temp);
    ThingSpeak.setField(2, humi);
    ThingSpeak.setField(3, sensorValue);
    ThingSpeak.setField(4, tagID);
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

    // Calcule o tempo gasto para escrever no ThingSpeak
    unsigned long writeTimeTS = millis() - startTimeTS;
    Serial.println("Tempo para escrever no ThingSpeak: " + String(writeTimeTS) + " ms");

    if (x == 200) {
      Serial.println("Channel update successful.");
    }
    else {
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }

    // Marcando o tempo inicial para o atuador
    unsigned long startTimeR = millis();

    if (temp > tempThreshold) {
      digitalWrite(RELAY_AC, HIGH);
      Serial.println("Temperatura acima do limiar, ativando ar condicionado.");
    }
    else {
      digitalWrite(RELAY_AC, LOW);
    }

    if (sensorValue > lightThreshold) {
      digitalWrite(RELAY_LIGHT, HIGH);
      Serial.println("Intensidade de luz abaixo do limiar, ativando luzes.");
    }
    else {
      digitalWrite(RELAY_LIGHT, LOW);
    }

    // Abrir porta
    digitalWrite(RELAY_DOOR, HIGH);
    Serial.println("Abrindo a porta.");
    delay(1000); // A porta fica aberta por 1 segundo
    digitalWrite(RELAY_DOOR, LOW);

    // Marcando o tempo final para o atuador
    unsigned long endTimeR = millis();

    // Calculando o tempo de resposta do atuador
    unsigned long actuatorResponseTimeR = endTimeR - startTimeR;
    Serial.println("Tempo de resposta do atuador: " + String(actuatorResponseTimeR) + " ms");
  }

  delay(15000);
}
