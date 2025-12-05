
#!/usr/bin/env python3
import time
import random
import socket
import paho.mqtt.client as mqtt

MQTT_BROKER = "broker.hivemq.com"
MQTT_PORT = 1883
TOPIC_TEMP = "mel_temp"
TOPIC_UMID = "mel_umid"
CLIENT_ID = f"PythonClientSim-{socket.gethostname()}"
PUB_INTERVAL_SEC = 5

def criar_mqtt_client():
    client = mqtt.Client(client_id=CLIENT_ID, clean_session=True)
    return client

def conectar_mqtt_com_retry(client, wait_seconds=3):
    while True:
        try:
            client.connect(MQTT_BROKER, MQTT_PORT, keepalive=30)
            client.loop_start()
            return
        except Exception as e:
            print(f"[MQTT] Falha ao conectar: {e}. Tentando novamente em {wait_seconds}s...")
            time.sleep(wait_seconds)

def gerar_medidas():
    temperatura = round(random.uniform(20.0, 30.0), 2)
    umidade = round(random.uniform(40.0, 80.0), 1)
    return temperatura, umidade

def main():
    client = criar_mqtt_client()
    conectar_mqtt_com_retry(client)
    print("[APP] Publicando medidas simuladas a cada", PUB_INTERVAL_SEC, "segundos.")
    try:
        while True:
            temperatura, umidade = gerar_medidas()

            if not client.is_connected():
                print("[MQTT] Conexão perdida. Reconectando...")
                client.loop_stop()
                conectar_mqtt_com_retry(client)

            client.publish(TOPIC_TEMP, f"{temperatura:.2f}", qos=0, retain=False)
            client.publish(TOPIC_UMID, f"{umidade:.1f}", qos=0, retain=False)

            print(f"Temperatura: {temperatura:.2f} °C | Umidade: {umidade:.1f} %")
            time.sleep(PUB_INTERVAL_SEC)
    except KeyboardInterrupt:
        print("\n[APP] Encerrando...")
    finally:
        try:
            client.loop_stop()
            client.disconnect()
        except:
            pass

if __name__ == "__main__":
    main()
