#include "SlaveComm.h"

SlaveComm *globalSlave = nullptr;


// CALLBACK GLOBAL
void ESPNOWReceiveCallback(uint8_t *mac, uint8_t *incomingMsg, uint8_t len) {
  if (globalSlave == nullptr) return;

  char buffer[100];
  memcpy(buffer, incomingMsg, len);
  buffer[len] = 0;

  globalSlave->processIncoming(buffer);
}


// CONSTRUCTOR
SlaveComm::SlaveComm() {
  _id = 0;
  _angle = 0;
  _distance = 0;
  _out = 0;
  _masterMACSet = false;
}


// INICIALIZACIÓN
bool SlaveComm::begin(const char *ssid, const char *password) {

  globalSlave = this;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(100);

  sscanf(WiFi.macAddress().c_str(), "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
         &_myMACAddress[0], &_myMACAddress[1], &_myMACAddress[2],
         &_myMACAddress[3], &_myMACAddress[4], &_myMACAddress[5]);

  if (esp_now_init() != 0) return false;

  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_recv_cb(ESPNOWReceiveCallback);

  if (_masterMACSet)
    esp_now_add_peer(_masterMACAddress, ESP_NOW_ROLE_CONTROLLER, 1, NULL, 0);

  return true;
}


// GUARDAR MAC DEL MAESTRO
void SlaveComm::setMasterMACAddress(uint8_t mac[6]) {
  memcpy(_masterMACAddress, mac, 6);
  _masterMACSet = true;
}


// PROCESAR MENSAJE RECIBIDO
void SlaveComm::processIncoming(const char *msg) {
  int ID;
  float ang, dist;
  int out;

  if (sscanf(msg, "id=%d, ang=%f, dist=%f, Out=%d",
             &ID, &ang, &dist, &out) == 4) {

    if (ID == _id) {
      _angle = ang;
      _distance = dist;
      _out = out;
      sendOK();
    }
  }
}


// ENVIAR OK AL MAESTRO
void SlaveComm::sendOK() {
  char msgOut[20];
  sprintf(msgOut, "OK id=%d", _id);

  esp_now_send(_masterMACAddress, (uint8_t *)msgOut, strlen(msgOut));
}