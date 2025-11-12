//#include "Arduino.h"
//#include "ESP8266WiFi.h"
#include <espnow.h>

class Comunication {
    private:
        int _id;
        float _angle;
        float _distance;
        bool _out;
        bool _status;
        bool _used;
        unsigned int _MAC;

    public:
        Comunication();
        void init(const char* ip, const char* psw);
        float getAngle();
        float getDistance();
        bool getStatus();
        bool isUsed();        
        unsigned int getMAC();
        void setMAC(unsigned int mac);
};