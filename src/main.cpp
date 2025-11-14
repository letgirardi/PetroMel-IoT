#include "BluetoothSerial.h"

// Cria o objeto BluetoothSerial
BluetoothSerial ESP_BT;

#define LED_PIN 2  // GPIO2
const int LED_BT = 4; //GPIO4
#define LED_3  5 //GPIO4
#define LED_4  18//GPIO4


void setup() {
  Serial.begin(115200);
  
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_BT, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  if (ESP_BT.begin("incel")) {
    Serial.println("✅ Bluetooth iniciado!");
    digitalWrite(LED_BT, HIGH);   // Liga o LED no pino 4
  } else {
    Serial.println("❌ Falha ao iniciar Bluetooth!");
    digitalWrite(LED_BT, LOW);    // LED apagado
  }

}

void BLE(){
  if (ESP_BT.available()) {
    String command = ESP_BT.readStringUntil('\n');
    command.trim(); // remove espaços e quebras

    Serial.println("Recebido: " + command);

    if (command == "ON") {
      digitalWrite(LED_PIN, HIGH);
      ESP_BT.println("LED ligado!");
    }
    else if (command == "OFF") {
      digitalWrite(LED_PIN, LOW);
      ESP_BT.println("LED desligado!");
    }
    else if(command == "ON3") {
      digitalWrite(LED_3, HIGH);
      ESP_BT.println("LED_3 Ligado");
    }
    else if (command == "OFF3") {
        digitalWrite(LED_3, LOW);
        ESP_BT.println("LED_3 desligado!");
      }
      else if(command == "ON4") {
      digitalWrite(LED_4, HIGH);
      ESP_BT.println("LED_4 Ligado");
    }
    else if (command == "OFF4") {
        digitalWrite(LED_4, LOW);
        ESP_BT.println("LED_4 desligado!");
      }
    else {
      ESP_BT.println("Comando inválido!");
    }
  }

}


void loop() {
 BLE();
}


