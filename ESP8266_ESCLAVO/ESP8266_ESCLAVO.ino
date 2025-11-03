//----- LIBRERIA -----//
#include <ESP8266WiFi.h>
#include <espnow.h>

//----- WIFI -----//
#define ssid "OPPO A53"
#define password "611b10a883c5"

//----- ID Y MAC ROBOT -----//
#define robotID 1
uint8_t masterMAC[] = {0x68,0xC6,0x3A,0x9F,0x98,0x65};

//----- MENSAJE ESPNOW -----//
char incomingMsg[50];
int expectedTurn = -1;


//----- CALLBACK RECEPCION ESPNOW -----//
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  Serial.println("REcibido");
  memcpy(incomingMsg, incomingData, len);
  incomingMsg[len] = 0;
  if (sscanf(incomingMsg, "TURN %d", &expectedTurn) == 1) {
    if (expectedTurn == robotID) {

      float ang = 112.4767753210715;
      float dist = 0.09923435538142777;
      int Out = 0;
      char msgOut[100];
      sprintf(msgOut, "id=%d, ang=%.13f, dist=%.14f, Out=%d", robotID, ang, dist, Out);
      esp_now_send(masterMAC, (uint8_t *)msgOut, strlen(msgOut));
      Serial.println("Datos enviados al Maestro");
    }
  }
}

//----- CALLBACK ENVIO ESPNOW -----//
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.println(sendStatus == 0 ? "OK enviado" : "Error envío");
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
  esp_now_register_send_cb(OnDataSent);
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
}
