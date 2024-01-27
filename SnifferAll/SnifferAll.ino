//sniff all packets
#include <mcp_can.h>
#include <SPI.h>

#define CanCrystal 16 //MHZ 8 or 16

INT32U canId = 0x000;
//INT8U canRtr = 0x0;
//INT8U canExF = 0x0;
unsigned char len = 0;
unsigned char buf[8];
String stringOut = "";
MCP_CAN CAN(10);// Set CS to pin 10

void setup() {    
    Serial.begin(115200);
    START_INIT:
#if CanCrystal == 8
    if(CAN_OK == CAN.begin(CAN_500KBPS,MCP_8MHz)) {
#endif
#if CanCrystal == 16
    if(CAN_OK == CAN.begin(CAN_500KBPS)) {
#endif
        Serial.println("CAN BUS Shield init ok!");
    } else {
        Serial.println("CAN BUS Shield init fail");
        delay(100);
        goto START_INIT;
    }
}

void loop() {
    if (CAN_MSGAVAIL == CAN.checkReceive()) {
        CAN.readMsgBuf(&len, buf);
        canId = CAN.getCanId();
        //canRtr = CAN.isRemoteRequest();
        //canExF = CAN.isExtendedFrame();
        //stringOut = String(canRtr,HEX);
        //stringOut.concat(",");
        //stringOut.concat(String(canExF,HEX));
        //stringOut.concat(",n,,");
        stringOut = String(canId,HEX);
        //stringOut.concat(String(canId,HEX));
        stringOut.concat(",");
        for (int i = 0; i<len; i++) {
            stringOut.concat(String(buf[i],HEX));
            stringOut.concat(",");
        }
        Serial.println(stringOut);
	}
}
