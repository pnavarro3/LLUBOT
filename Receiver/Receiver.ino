#include <ESP8266WiFi.h>
#include <espnow.h>
#include <WiFiUdp.h>

#ifndef STASSID
#define STASSID "yiyiyi"
#define STAPSK "xabicrack"
#endif

unsigned int localPort = 8888;

char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1];
WiFiUDP Udp;

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

  Serial.print("Receptor listo. Mi MAC: \n");
  Serial.println(WiFi.macAddress());

  WiFi.mode(WIFI_STA);  // obligatorio para ESP-NOW
  WiFi.begin(STASSID,STAPSK);

  while (WiFi.status() != WL_CONNECTED) {
  Serial.print('.');
  delay(500);
  }

  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  Udp.begin(localPort);

  if (esp_now_init() != 0) {
    Serial.println("Error al iniciar ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE); // modo receptor
  esp_now_register_recv_cb(onDataRecv);

}

void loop() {
   // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    //Serial.printf("Received packet of size %d from %s:%d\n    (to %s:%d, free heap = %d B)\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort(), Udp.destinationIP().toString().c_str(), Udp.localPort(), ESP.getFreeHeap());

    // read the packet into packetBufffer
    int n = Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    packetBuffer[n] = 0;
    Serial.println("Contents:");
    Serial.println(packetBuffer);
  }
}
