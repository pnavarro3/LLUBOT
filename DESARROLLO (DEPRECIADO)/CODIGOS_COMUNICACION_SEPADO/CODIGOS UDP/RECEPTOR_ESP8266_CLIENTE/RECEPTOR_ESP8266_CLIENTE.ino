// LOLIN(WeMos) D1 R2 & mini

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define ssid "OPPO A53"
#define password "611b10a883c5"

unsigned int localPort = 8888;

char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1];
WiFiUDP Udp;

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA),
  WiFi.begin(ssid,password);

  while (WiFi.status() != WL_CONNECTED) {
  Serial.print('.');
  delay(500);
  }

  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  Udp.begin(localPort);
}

void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    int n = Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    packetBuffer[n] = 0;
    Serial.println(packetBuffer);
  }
  delay(1000);
}
