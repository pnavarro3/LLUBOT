// LOLIN(WeMos) D1 R2 & mini

#include <ESP8266WiFi.h>
#include <espnow.h>

#define ssid "OPPO A53"
#define password "611b10a883c5"


void onDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  Serial.print("Datos recibidos: ");
  for (int i = 0; i < len; i++) {
    Serial.print((char)incomingData[i]);
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);

  Serial.print("Receptor listo. Mi MAC: \n");
  Serial.println(WiFi.macAddress());

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);

  while (WiFi.status() != WL_CONNECTED) {
  Serial.print('.');
  delay(500);
  }

  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  if (esp_now_init() != 0) {
    Serial.println("Error al iniciar ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(onDataRecv);

}

void loop() {

}