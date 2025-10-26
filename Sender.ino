#include <ESP8266WiFi.h>
#include <espnow.h>

// Dirección MAC del receptor (modifícala)
uint8_t receiverAddress[] = {0x5C, 0xCF, 0x7F, 0xB5, 0x64, 0x10};

// Callback para confirmar envío
void onDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Estado del envío: ");
  Serial.println(sendStatus == 0 ? "Éxito" : "Fallo");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != 0) {
    Serial.println("Error al iniciar ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER); // modo emisor
  esp_now_register_send_cb(onDataSent);

  esp_now_add_peer(receiverAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  Serial.println("Emisor listo.");
}

void loop() {
  const char *mensaje = "Hola desde el ESP8266 emisor!";
  esp_now_send(receiverAddress, (uint8_t *)mensaje, strlen(mensaje));
  delay(2000);
}
