#include "MasterComm.h"

MasterComm master;

uint8_t robotPrueba1[] = {0x5C,0xCF,0x7F,0x01,0x65,0x6B};
uint8_t robotPrueba2[] = {0x5C,0xCF,0x7F,0xB5,0x64,0x10};

void setup() {
    Serial.begin(115200);
    master.addRobotMAC(robotPrueba1);
    master.addRobotMAC(robotPrueba2);
    master.begin("OPPO A53", "611b10a883c5", 8888);
    master.enableBroadcastIP("255.255.255.255");
    Serial.println("MAESTRO listo");
}

void loop() {
    static unsigned long t = 0;
    if (millis() - t > 200) {
        t = millis();

        int id1 = 0;
        int id2 = 1;
        float ang = 0.00;
        int dist = 12.00;
        int out = 0;

        char msg1[64];
        snprintf(msg1, sizeof(msg1),
            "id=%d, ang=%.2f, dist=%d, Out=%d",
            id1, ang, dist, out
        );
        char msg2[64];
        snprintf(msg2, sizeof(msg2),
            "id=%d, ang=%.2f, dist=%d, Out=%d",
            id2, ang, dist, out
        );

        master.sendToRobot(id1, msg1);
        master.sendToRobot(id2, msg2);
    }
}
