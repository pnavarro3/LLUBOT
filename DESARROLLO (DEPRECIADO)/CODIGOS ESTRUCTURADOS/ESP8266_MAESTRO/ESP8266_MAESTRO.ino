//----- LIBRERIA -----//
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <WiFiUdp.h>

//----- WIFI -----//
#define ssid "OPPO A53"
#define password "611b10a883c5"

//----- VARIABLES UDP -----//
unsigned int localPort = 8888;
WiFiUDP Udp;
char packetBuffer[255];

//----- VARIABLES ESPNOW -----//
const int numRobots = 2;
uint8_t robotMACs[numRobots][6] = { 
  {0x5C,0xCF,0x7F,0xB5,0x64,0x10},  //ESP8266 prueba esclavo
  //{0x5C,0xCF,0x7F,0x01,0x65,0x6B},// Robot 0 (ID = 0)
  //{0xCC,0x50,0xE3,0x55,0x09,0x6A},// Robot 1 (ID = 1)
};

//----- CALLBACK ENVIO -----//
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.println(sendStatus == 0 ? "Mensaje enviado" : "Error envío");
}

//----- CALLBACK RECEPCION ESPNOW -----//
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  incomingData[len] = 0;
  String msg = String((char*)incomingData);

  Serial.print("Respuesta robot → ");
  Serial.println(msg);
  
  if (msg.startsWith("OK ")) {
    Udp.beginPacket("255.255.255.255", localPort);
    Udp.print(msg);
    Udp.endPacket();
    Serial.println("OK reenviado a Python");
  }
}

//----- WIFI -----//
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { Serial.print("."); delay(300); }
  Serial.println("\nWiFi conectado");
  Serial.print("MAC Maestro: "); Serial.println(WiFi.macAddress());
  Udp.begin(localPort);
}

//----- ESPNOW -----//
void initESPNOW() {
  if (esp_now_init() != 0) {
    Serial.println("Error ESPNOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_recv_cb(OnDataRecv);
  for (int i = 0; i < numRobots; i++) {
    esp_now_add_peer(robotMACs[i], ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  }
}

//----- RECEPTOR UDP DESDE PYTHON -----//
void readUDP() {
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    int len = Udp.read(packetBuffer, sizeof(packetBuffer) - 1);
    if (len > 0) packetBuffer[len] = '\0';

    Serial.print("UDP recibido: ");
    Serial.println(packetBuffer);

    // Obtener id=XX del mensaje
    int targetID;
    if (sscanf(packetBuffer, "id=%d", &targetID) == 1 && targetID < numRobots) {
      
      esp_now_send(robotMACs[targetID], (uint8_t*)packetBuffer, strlen(packetBuffer));
      Serial.printf("Mensaje enviado por ESP-NOW al Robot %d\n", targetID);
    }
  }
}

//----- SETUP -----//
void setup() {
  Serial.begin(115200);
  initWiFi();
  initESPNOW();
}

//----- LOOP -----//
void loop() {
  readUDP();
}