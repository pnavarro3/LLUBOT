// LOLIN(WeMos) D1 R2 & mini

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define ssid "OPPO A53"
#define password "611b10a883c5"

unsigned int localPort = 8888;
WiFiUDP Udp;

char packetBuffer[255];

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\nConectado!");
  Serial.print("IP ESP8266: ");
  Serial.println(WiFi.localIP());

  Udp.begin(localPort);
  Serial.print("Esperando UDP en puerto ");
  Serial.println(localPort);
}

void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    int len = Udp.read(packetBuffer, 254);
    if (len > 0) packetBuffer[len] = 0;

    Serial.print("Mensaje recibido: ");
    Serial.println(packetBuffer);
  }

  Udp.beginPacket("255.255.255.255", 8888);
  Udp.write("Hola desde ESP8266 1");
  Udp.endPacket();

  Serial.println("Mensaje enviado");

  delay(1000);
}
