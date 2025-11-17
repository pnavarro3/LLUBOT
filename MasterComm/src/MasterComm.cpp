#include "MasterComm.h"

MasterComm *MasterGlobal = nullptr;


// CALLBACK: Envío
void Master_OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
    Serial.println(sendStatus == 0 ? "ESPNOW enviado" : "Error envío");
}


//  CALLBACK: Recepción ESPNOW
void Master_OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
    if (!MasterGlobal) return;

    char msg[200];
    memcpy(msg, incomingData, len);
    msg[len] = 0;

    Serial.print("Respuesta robot → ");
    Serial.println(msg);

    MasterGlobal->processRobotResponse(msg);
}


// CONSTRUCTOR
MasterComm::MasterComm() {
    _robotCount = 0;
    _udpPort = 0;
    _broadcastIP = "255.255.255.255";
}


// INICIO
bool MasterComm::begin(const char *ssid, const char *password, unsigned int udpPort) {

    MasterGlobal = this;
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) delay(100);

    Serial.print("MAC Maestro: ");
    Serial.println(WiFi.macAddress());

    _udpPort = udpPort;
    _udp.begin(_udpPort);

    if (esp_now_init() != 0) return false;

    esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
    esp_now_register_send_cb(Master_OnDataSent);
    esp_now_register_recv_cb(Master_OnDataRecv);

    for (int i = 0; i < _robotCount; i++) {
        esp_now_add_peer(_robotMACs[i], ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
    }

    return true;
}


// Registrar MAC de robots esclavos
void MasterComm::addRobotMAC(uint8_t mac[6]) {
    if (_robotCount >= MAX_ROBOTS) return;
    memcpy(_robotMACs[_robotCount], mac, 6);
    _robotCount++;
}


// Enviar al robot correspondiente por ID
void MasterComm::sendToRobot(int id, const char *msg) {
    if (id < 0 || id >= _robotCount) return;

    esp_now_send(_robotMACs[id], (uint8_t*)msg, strlen(msg));
}


// Procesar respuestas de los robots
void MasterComm::processRobotResponse(const char *msg) {
    _udp.beginPacket(_broadcastIP.c_str(), _udpPort);
    _udp.print(msg);
    _udp.endPacket();

    Serial.println("OK reenviado a Python");
}


// Leer mensajes UDP y mandarlos al robot
void MasterComm::readUDP() {
    char buffer[256];

    int packetSize = _udp.parsePacket();
    if (!packetSize) return;

    int len = _udp.read(buffer, sizeof(buffer) - 1);
    if (len > 0) buffer[len] = '\0';

    Serial.print("UDP recibido: ");
    Serial.println(buffer);

    int targetID;
    if (sscanf(buffer, "id=%d", &targetID) == 1) {
        sendToRobot(targetID, buffer);
        Serial.printf("Enviado por ESPNOW al robot %d\n", targetID);
    }
}
