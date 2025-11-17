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

    void addRobotMAC(uint8_t mac[6]);
    void enableBroadcastIP(const char *ip) { _broadcastIP = ip; }

    void readUDP();
    void sendToRobot(int id, const char *msg);
    void processRobotResponse(const char *msg);

private:
    WiFiUDP _udp;
    unsigned int _udpPort;
    String _broadcastIP;

    static const int MAX_ROBOTS = 10;
    uint8_t _robotMACs[MAX_ROBOTS][6];
    int _robotCount;
};

void Master_OnDataSent(uint8_t *mac_addr, uint8_t sendStatus);
void Master_OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len);

extern MasterComm *MasterGlobal;

#endif
