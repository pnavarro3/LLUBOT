//----- LIBRERIA -----//
#include <ESP8266WiFi.h>
#include <espnow.h>

//----- WIFI -----//
#define ssid "OPPO A53"
#define password "611b10a883c5"

//----- ID Y MAC ROBOT -----//
#define robotID 0
//uint8_t masterMAC[] = {0x68,0xC6,0x3A,0x9F,0x98,0x65}; //MAC LLUBOT
uint8_t masterMAC[] = {0xA4,0xCF,0x12,0xF5,0x20,0x53};   //MAC ESP8266 MAESTRO PRUEBA

//----- MENSAJE ESPNOW -----//
char incomingMsg[50];
int expectedTurn = -1;

int rxID;
float ang, dist;
int out;

//----- CALLBACK RECEPCION ESPNOW -----//
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  Serial.println("Recibido");
  memcpy(incomingMsg, incomingData, len);
  incomingMsg[len] = 0;
  
  Serial.print("Mensaje: ");
  Serial.println(incomingMsg);

  if (sscanf(incomingMsg, "id=%d, ang=%f, dist=%f, Out=%d", &rxID, &ang, &dist, &out) == 4) {
    Serial.print("ID recibido: ");
    Serial.println(rxID);

    if (rxID == robotID) {
      char msgOut[50];
      sprintf(msgOut, "OK id=%d", robotID);
      
      esp_now_send(masterMAC, (uint8_t *)msgOut, strlen(msgOut));
      Serial.println("-> OK enviado al maestro");
    }
  }
}

//----- INICIAR WIFI -----//
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("\nWiFi conectado");
  Serial.print("MAC Robot: "); Serial.println(WiFi.macAddress());
  Serial.print("ID Robot: "); Serial.println(robotID);
}

//----- INICIAR ESPNOW -----//
void initESPNOW() {
  if (esp_now_init() != 0) {
    Serial.println("Error ESP-NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_recv_cb(OnDataRecv);
  esp_now_add_peer(masterMAC, ESP_NOW_ROLE_CONTROLLER, 1, NULL, 0);
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
  //tu codigo
}