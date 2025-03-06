#include <SPI.h>
#include "mcp_can.h"
INT32U canId = 0x0;
unsigned char len = 0;
unsigned char buf[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

String stringOut = "";

int incomingByte;
unsigned int i;

/*
  CAN_TxMsg.id=0x391;     
  CAN_TxMsg.header.rtr=0;
  CAN_TxMsg.header.length=3;

391 key fob   391,55,80,4 -from snif, 391, 55, 0, 0 test message
	0 - 0 nothing, AA Windows down, 55 Window up
*/


MCP_CAN CAN(10);                                            // Set CS to pin 10

void setup() {   
	Serial.begin(115200);
	START_INIT:
	if(CAN_OK == CAN.begin(CAN_500KBPS)) {
        	Serial.println("CAN BUS Shield init ok!");
    	} else {
        	Serial.println("CAN BUS Shield init fail");
	        Serial.println("Init CAN BUS Shield again");
        	delay(100);
	        goto START_INIT;
    	}
}


void loop(){
    canId = 0x17333111;

    //80,11,4c,55,01,66,48,00,
    //C0,00,01,65,49,01,66,4a,
    //C1,00,00,01,00,00,00,

    for (byte i = 0x30; i<=0x39; i++) {

    Serial.println("sending for 80, test");
    buf[0]=0x80; buf[1]=0x11; buf[2]=0x4c; buf[3]=0x55; buf[4]=0x01; buf[5]=0x38; buf[6]=0x48; buf[7]=0x00; CAN.sendMsgBuf(canId, 1, 8, buf);
    buf[0]=0xc0; buf[1]=0x00; buf[2]=0x01; buf[3]=0x65; buf[4]=0x49; buf[5]=0x01; buf[6]=i; buf[7]=0x4a; CAN.sendMsgBuf(canId, 1, 8, buf);
    buf[0]=0xc1; buf[1]=0x00; buf[2]=0x00; buf[3]=0x01; buf[4]=0x00; buf[5]=0x00; buf[6]=0x00; buf[7]=0x00; CAN.sendMsgBuf(canId, 1, 7, buf);
    delay(10000);

    Serial.println("sending for 90, test");
    buf[0]=0x90; buf[1]=0x11; buf[2]=0x4c; buf[3]=0x55; buf[4]=0x01; buf[5]=0x39; buf[6]=0x48; buf[7]=0x00; CAN.sendMsgBuf(canId, 1, 8, buf);
    buf[0]=0xd0; buf[1]=0x00; buf[2]=0x01; buf[3]=0x65; buf[4]=0x49; buf[5]=0x01; buf[6]=i; buf[7]=0x4a; CAN.sendMsgBuf(canId, 1, 8, buf);
    buf[0]=0xd1; buf[1]=0x00; buf[2]=0x00; buf[3]=0x01; buf[4]=0x00; buf[5]=0x00; buf[6]=0x00; buf[7]=0x00; CAN.sendMsgBuf(canId, 1, 7, buf);
    delay(10000);

    Serial.println("sending for a0, test");
    buf[0]=0xa0; buf[1]=0x11; buf[2]=0x4c; buf[3]=0x55; buf[4]=0x01; buf[5]=0x61; buf[6]=0x48; buf[7]=0x00; CAN.sendMsgBuf(canId, 1, 8, buf);
    buf[0]=0xe0; buf[1]=0x00; buf[2]=0x01; buf[3]=0x65; buf[4]=0x49; buf[5]=0x01; buf[6]=i; buf[7]=0x4a; CAN.sendMsgBuf(canId, 1, 8, buf);
    buf[0]=0xe1; buf[1]=0x00; buf[2]=0x00; buf[3]=0x01; buf[4]=0x00; buf[5]=0x00; buf[6]=0x00; buf[7]=0x00; CAN.sendMsgBuf(canId, 1, 7, buf);
    delay(10000);

    Serial.println("sending for b0, test");
    buf[0]=0xb0; buf[1]=0x11; buf[2]=0x4c; buf[3]=0x55; buf[4]=0x01; buf[5]=0x62; buf[6]=0x48; buf[7]=0x00; CAN.sendMsgBuf(canId, 1, 8, buf);
    buf[0]=0xf0; buf[1]=0x00; buf[2]=0x01; buf[3]=0x65; buf[4]=0x49; buf[5]=0x01; buf[6]=i; buf[7]=0x4a; CAN.sendMsgBuf(canId, 1, 8, buf);
    buf[0]=0xf1; buf[1]=0x00; buf[2]=0x00; buf[3]=0x01; buf[4]=0x00; buf[5]=0x00; buf[6]=0x00; buf[7]=0x00; CAN.sendMsgBuf(canId, 1, 7, buf);
    delay(10000);    
}

}
