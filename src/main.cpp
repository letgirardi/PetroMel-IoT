#include <WiFi.h>
#include <PubSubClient.h>
#include "DHTesp.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- Configurações de rede e MQTT ---
const char* ssid = "Redmi 13C";
const char* password = "Echidna12";
const char* mqtt_server = "broker.hivemq.com";

// --- Definições de pinos ---
#define DHT_PIN 27
#define WIFI_LED_PIN 14
#define MQTT_LED_PIN 33

// --- Configurações do Display OLED ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// --- Objetos globais ---
DHTesp dhtSensor;
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// --- Conexão WiFi ---
void conectarWiFi() {
  Serial.print("Conectando ao WiFi: ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// --- Conexão MQTT ---
void conectarMQTT() {
  if (!mqttClient.connected()) {
    Serial.print("Conectando ao broker MQTT...");
    if (mqttClient.connect("ESP32Client")) {
      Serial.println("Conectado! Código: ");
      Serial.print(mqttClient.state());
      Serial.println(" - Tentando novamente em 3s...");
      delay(3000);
    }
  }
}

// --- Atualiza LEDs de status ---
void atualizarStatusLEDs() {
  digitalWrite(WIFI_LED_PIN, WiFi.status() == WL_CONNECTED ? HIGH : LOW);
  digitalWrite(MQTT_LED_PIN, mqttClient.connected() ? HIGH : LOW);
}

// --- Publica dados e atualiza display ---
void publicarDadosSensor() {
  TempAndHumidity dados = dhtSensor.getTempAndHumidity();
  float temperatura = dados.temperature;
  float umidade = dados.humidity;

  mqttClient.publish("minha_temp_jpsv", String(temperatura, 2).c_str());
  mqttClient.publish("minha_umid_jpsv", String(umidade, 1).c_str());

  Serial.printf("Temperatura: %.2f °C\n", temperatura);
  Serial.printf("Umidade: %.1f %%\n", umidade);

  // Atualiza display OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.print("Temp: ");
  display.print(temperatura, 2);
  display.println(" C");

  display.print("Umid: ");
  display.print(umidade, 1);
  display.println(" %");

  display.display();
}

// --- Setup ---
void setup() {
  Serial.begin(9600);
  pinMode(WIFI_LED_PIN, OUTPUT);
  pinMode(MQTT_LED_PIN, OUTPUT);

  // Inicializa Display OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Falha ao iniciar o display OLED"));
    while (true); // Para execução
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Inicializando...");
  display.display();
  delay(1000);

  conectarWiFi();
  mqttClient.setServer(mqtt_server, 1883);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
}

// --- Loop principal ---
void loop() {
  mqttClient.loop();
  conectarMQTT();
  atualizarStatusLEDs();

  static unsigned long ultimoTempoLeitura = 0;
  if (millis() - ultimoTempoLeitura >= 5000) {
    publicarDadosSensor();
    ultimoTempoLeitura = millis();
  }
}