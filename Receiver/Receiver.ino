#include <ESP8266WiFi.h>
#include <espnow.h>


struct nombreStruct {
  float angle;
  float distance;
  int id;
  
};



// Callback cuando llegan datos
void onDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  Serial.print("Datos recibidos: ");
  for (int i = 0; i < len; i++) {
    Serial.print((char)incomingData[i]);
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);  // obligatorio para ESP-NOW
  WiFi.disconnect();

  if (esp_now_init() != 0) {
    Serial.println("Error al iniciar ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE); // modo receptor
  esp_now_register_recv_cb(onDataRecv);

  Serial.print("Receptor listo. Mi MAC: ");
  Serial.println(WiFi.macAddress());
}

void loop() {
  // nada que hacer aquí
}
