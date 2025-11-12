#include "comunication.h"
//include...

Comunication::Comunication() {
    ;
}

void Comunication::init(const char* ip, const char* psw) {
    ;
}

float Comunication::getAngle() {
    return this->_angle;
}

float Comunication::getDistance() {
    return this->_distance;
}

bool Comunication::getStatus() {
    return this->_status;
}

bool Comunication::isUsed() {
    return this->_used;
}

unsigned int Comunication::getMAC() {
    return this->_MAC;
}

void Comunication::setMAC(unsigned int mac) {
    this->_MAC = mac;
}