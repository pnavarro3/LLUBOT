#ifndef MASTERCOMM_H
#define MASTERCOMM_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <WiFiUdp.h>

class MasterComm {
public:
    MasterComm();

    bool begin(const char *ssid, const char *password, unsigned int udpPort);

    // Configuración
    void addRobotMAC(uint8_t mac[6]);
    void enableBroadcastIP(const char *ip) { _broadcastIP = ip; }

    // Lógica principal
    void readUDP();                // recibe mensajes desde Python
    void sendToRobot(int id, const char *msg);
    void processRobotResponse(const char *msg);  // llamado desde callback

private:
    WiFiUDP _udp;
    unsigned int _udpPort;
    String _broadcastIP;

    static const int MAX_ROBOTS = 10;
    uint8_t _robotMACs[MAX_ROBOTS][6];
    int _robotCount;
};

// ---- Callbacks globales requeridos por ESP-NOW ----
void Master_OnDataSent(uint8_t *mac_addr, uint8_t sendStatus);
void Master_OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len);

// puntero global a la instancia
extern MasterComm *MasterGlobal;

#endif
