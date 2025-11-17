#include <ESP8266WiFi.h>
#include <espnow.h>

#define ssid "OPPO A53"
#define password "611b10a883c5"

uint8_t peerAddress[] = {0x5C, 0xCF, 0x7F, 0xB5, 0x64, 0x10}; // MAC del otro ESP

// Callback envío
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Estado del envío: ");
  Serial.println(sendStatus == 0 ? "OK" : "Fallo");
}

// Callback recepción
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  Serial.print("Datos recibidos: ");
  for (int i = 0; i < len; i++) Serial.print((char)incomingData[i]);
  Serial.println();
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nIP: ");
  Serial.println(WiFi.localIP());
  Serial.print("Mi MAC: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != 0) {
    Serial.println("Error ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);


  esp_now_add_peer(peerAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
}

void loop() {
  const char *msg = "Hola desde el ESP8266 emisor + receptor 2!";
  esp_now_send(peerAddress, (uint8_t *)msg, strlen(msg));
  Serial.println("Mensaje enviado");
  delay(1000);
}
