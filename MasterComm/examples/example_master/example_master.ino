#include "MasterComm.h"

MasterComm master;

uint8_t robot0[] = {0x5C,0xCF,0x7F,0x01,0x65,0x6B};
uint8_t robot2[] = {0xCC,0x50,0xE3,0x55,0x09,0x6A};
uint8_t robotPrueba[] = {0x5C,0xCF,0x7F,0xB5,0x64,0x10};

void setup() {
  Serial.begin(115200);

  master.addRobotMAC(robot0);
  master.addRobotMAC(robot1);
  master.addRobotMAC(robotPrueba);

  master.begin("OPPO A53", "611b10a883c5", 8888);

  master.enableBroadcastIP("255.255.255.255");

  Serial.println("MAESTRO listo");
}

void loop() {
  master.readUDP();
}
