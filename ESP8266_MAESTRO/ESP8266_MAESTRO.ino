//----- LIBRERIA -----//
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <WiFiUdp.h>


//----- WIFI -----//
#define ssid "OPPO A53"
#define password "611b10a883c5"

//----- UDP -----//
unsigned int localPort = 8888;
WiFiUDP Udp;
char packetBuffer[255];

//----- ESPNOW -----//
const int numRobots = 2;
int currentRobot = 0;
uint8_t robotMACs[numRobots][6] = {
  {0xCC,0x50,0xE3,0x55,0x09,0x6A}, // Robot 0
  {0x5C,0xCF,0x7F,0x01,0x65,0x6B}, // Robot 1
};

//----- CALLBACK ENVIO ESPNOW -----//
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.println(sendStatus == 0 ? "Turno enviado correctamente" : "Error enviando turno");
}

//----- CALLBACK RECEPCION ESPNOW -----//
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  incomingData[len] = 0;
  Serial.printf("Robot %d → ", currentRobot);
  Serial.println((char*)incomingData);
  sendPC((char*)incomingData);
}

//----- INICIAR WIFI Y UDP -----//
void initWiFi() {
  Serial.println("Conectando WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("\nWiFi conectado");
  Serial.print("IP: "); Serial.println(WiFi.localIP());
  Serial.print("MAC Maestro: "); Serial.println(WiFi.macAddress());
  Udp.begin(localPort);
  Serial.printf("UDP escuchando en puerto %d\n", localPort);
}

//----- INICIAR ESPNOW Y CONFIGURAR MACS -----//
void initESPNOW() {
  if (esp_now_init() != 0) {
    Serial.println("Error iniciando ESP-NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);
  for (int i = 0; i < numRobots; i++) {
    esp_now_add_peer(robotMACs[i], ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  }
  Serial.println("ESP-NOW inicializado y robots registrados");
}

//----- LEER UDP-----//
void readUDP() {
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    int len = Udp.read(packetBuffer, sizeof(packetBuffer) - 1);
    if (len > 0) packetBuffer[len] = '\0';
    Serial.print("UDP recibido: ");
    Serial.println(packetBuffer);
  }
}

//----- ENVIAR UDP -----//
void sendPC(char* msg) {
  Udp.beginPacket("255.255.255.255", localPort);
  Udp.printf("%s", msg);
  Udp.endPacket();
}

//----- ENVIAR ESPNOW-----//
void sendTurn() {
  char turnMsg[30];
  sprintf(turnMsg, "TURN %d", currentRobot);
  esp_now_send(robotMACs[currentRobot], (uint8_t*)turnMsg, strlen(turnMsg));
  Serial.printf("Turno enviado al Robot %d\n", currentRobot);
}



//----- SETUP -----//
void setup() {
  Serial.begin(115200);
  while(!Serial){;}
  initWiFi();
  initESPNOW();
}

//----- LOOP -----//
void loop() {
  readUDP();
  sendTurn();
  delay(100);
  currentRobot++;
  if (currentRobot >= numRobots) {
    currentRobot = 0;
  }
  delay(100);
}