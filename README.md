
<img width=100% src="https://raw.githubusercontent.com/letgirardi/PetroMel-IoT/refs/heads/main/images/banner.jpg"/>

-----

# 🍕 PetroMel-IoT

Este projeto é uma solução completa de IoT para monitoramento ambiental híbrido. Ele integra sensores físicos (ESP32) e dados simulados (Python), comunicando-se via protocolo MQTT e orquestrando a visualização/lógica através do Node-RED rodando em Docker.

## 📋 Visão Geral

O sistema coleta dados de luminosidade de um ambiente físico e simula dados de temperatura e umidade para fins de teste de carga e visualização.

O projeto é composto por três módulos principais:

1.  **Firmware (ESP32):** Leitura de sensor LDR e publicação MQTT via WiFi.
2.  **Simulador (Python):** Gera dados aleatórios de Temperatura e Umidade.
3.  **Backend (Node-RED/Docker):** Container Docker configurado para receber os dados e fornecer dashboard/lógica.

## 🗂 Estrutura do Projeto

```text
C:.
├───.pio                   # Arquivos de build e bibliotecas do PlatformIO
│   ├───build
│   └───libdeps
├───.vscode                # Configurações do editor
├───images                 # Diagramas e imagens do projeto
├───include                # Headers C++
├───lib                    # Bibliotecas locais do projeto
├───node-red               # Arquivos de fluxo e configurações do Node-RED
├───src                    # Código fonte do firmware (main.cpp)
├───test                   # Testes unitários
├───Dockerfile             # Configuração da imagem Docker do Node-RED
└───info_gen.py            # Script Python gerador de dados simulados
```

## 🛠 Hardware Necessário

  * **Placa:** ESP32 DevKit V1
  * **Sensores:**
      * LDR (Light Dependent Resistor) - Conectado ao pino `GPIO 34`.
      * Resistor de 10kΩ (para divisor de tensão do LDR).
  * **Atuadores/Indicadores:**
      * LED WiFi (Status de Conexão) - `GPIO 17`
      * LED MQTT (Status do Broker) - `GPIO 18`

## ⚙️ Configurações e Tópicos MQTT

O sistema utiliza os seguintes tópicos para comunicação:

| Origem | Variável | Tópico MQTT | Broker Configurado |
| :--- | :--- | :--- | :--- |
| **ESP32** | Luminosidade (LDR) | `mel/LDR/Luminosidade` | `broker.hivemq.com` |
| **Python** | Temperatura | `mel_temp` | `*.ngrok-free.dev` (Ver nota) |
| **Python** | Umidade | `mel_umid` | `*.ngrok-free.dev` (Ver nota) |

> **Nota:** O código atual aponta para brokers diferentes (`hivemq` público no ESP32 e um túnel `ngrok` no Python). Certifique-se de unificar os endereços ou configurar o Node-RED para ouvir ambas as fontes (Bridge).

## 🚀 Como Executar

### 1\. Firmware (ESP32)

Este projeto utiliza **PlatformIO**.

1.  Abra a pasta raiz no VSCode.
2.  Edite o arquivo `src/main.cpp` com suas credenciais WiFi:
    ```cpp
    const char* ssid = "SUA_REDE_WIFI";
    const char* password = "SUA_SENHA";
    ```
3.  Conecte o ESP32 via USB.
4.  Clique em **Upload** (seta na barra inferior do PlatformIO).

### 2\. Simulador de Dados (Python)

Necessário Python 3 instalado.

1.  Instale a dependência MQTT:
    ```bash
    pip install paho-mqtt
    ```
2.  Edite o `info_gen.py` se precisar alterar o endereço do Broker.
3.  Execute o script:
    ```bash
    python info_gen.py
    ```

### 3\. Dashboard (Node-RED via Docker)

Certifique-se de ter o Docker instalado e rodando.

1.  Construa a imagem personalizada:
    ```bash
    docker build -t bdeyvid/my-iot-nodered .
    ```
2.  Inicie o container:
    ```bash
    docker run -d -p 1880:1880 --name node-red-iot bdeyvid/my-iot-nodered
    ```
3.  Acesse `http://localhost:1880` no seu navegador.

## 📦 Dependências

  * **Firmware:**
      * `PubSubClient` (Nick O'Leary)
      * `WiFi` (Built-in ESP32)
  * **Python:**
      * `paho-mqtt`
  * **Docker:**
      * Base Image: `nodered/node-red:latest`

## 🤝 Contribuição

1.  Faça um Fork do projeto
2.  Crie sua Feature Branch (`git checkout -b feature/AmazingFeature`)
3.  Commit suas mudanças (`git commit -m 'Add some AmazingFeature'`)
4.  Push para a Branch (`git push origin feature/AmazingFeature`)
5.  Abra um Pull Request
