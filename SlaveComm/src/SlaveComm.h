#ifndef SLAVECOMM_h
#define SLAVECOMM_h

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>

class SlaveComm {
  private:
    int _id;
    float _angle;
    float _distance;
    int _out;

    uint8_t _masterMACAddress[6];
    uint8_t _myMACAddress[6];
    bool _masterMACSet;

  public:
    SlaveComm();

    bool begin(const char *ssid, const char *password);

    int getID()        { return _id; }
    float getAngle()   { return _angle; }
    float getDistance(){ return _distance; }
    int getOut()       { return _out; }

    void setID(int id) { _id = id; }
    void setMasterMACAddress(uint8_t mac[6]);

    void sendOK();

    void processIncoming(const char *msg);
};

void ESPNOWReceiveCallback(uint8_t *mac, uint8_t *incomingMsg, uint8_t len);

extern SlaveComm *globalSlave;

#endif
