// LOLIN(WeMos) D1 R2 & mini

#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

#define ssid "OPPO A53"
#define password "611b10a883c5"
unsigned int localPort = 8888;

WiFiUDP Udp;


void setup() {
  Serial.begin(115200);
  while(!Serial){;}

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  Serial.printf("UDP server on port %d\n", localPort);
  Udp.begin(localPort);
}



void loop() {
  Udp.beginPacket("10.251.249.211", 8888);
  Udp.write("Hola desde emisor UDP");
  Udp.endPacket();
  Serial.println("Mensaje UDP enviado");
  delay(1000);
}
