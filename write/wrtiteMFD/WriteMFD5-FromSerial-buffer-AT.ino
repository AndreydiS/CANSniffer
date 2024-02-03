#include <SPI.h>
#include "mcp_can.h"

MCP_CAN CAN(10); // Set CS to pin 10

#define DEFcanId 0x17333111
#define delayLastMessage 100

INT32U canId = 0x0;
INT32U tempCanId = 0x0;

byte incomingByte;
byte bitCount=0;
byte availableBytes = 0;
byte bufsize = 0;
unsigned long timeLastByteReceived = 0;

unsigned char buf[256];
unsigned char bufToSend[8];

void setup() {
  Serial.begin(9600);
	START_INIT:
	if(CAN_OK == CAN.begin(CAN_500KBPS)) {
        	Serial.println("CAN ok. CMD: at <lenOfCan><canid><payload>");
    	} else {
        	Serial.println("CAN BUS Shield init fail");
        	delay(100);
	        goto START_INIT;
    	}
}

void loop() {
  if (((millis() - timeLastByteReceived) > 1000) && (bitCount>5)) {
    Serial.println("No new bytes received. Analysing and sending. ");
    if (buf[0] == 0x61 && buf[1] == 0x74 && buf[2] == 0x20) {
      canId = 0x00;
      for (int i = 4; i <= buf[3]+3; i++) {
        tempCanId = buf[i];
        canId = canId*0x100 + tempCanId;
      }
      Serial.print("CAN ID: "); 
      Serial.println(canId, HEX); 
      byte j=0;
      for (int i = (buf[3]+4); i <= (bitCount-1); i++) {
        bufToSend[j] = buf[i];
        Serial.print(buf[i],HEX);  
        Serial.print(",");  
        if (j >=7) {
          Serial.println(" S"); 
          CAN.sendMsgBuf(canId, 1, 8, bufToSend);
          j=0;
        } else {
          j++;
        }
      }
      if (j >0) {
        Serial.print(" SL ");  
        Serial.println(j);            
        CAN.sendMsgBuf(canId, 1, j, bufToSend);
      }
    } else {
      Serial.println("wrong command, try again "); 
    }
    bitCount = 0;
  }
  
  if (Serial.available() > 0) { //populating buffer
    incomingByte = Serial.read();
    buf[bitCount] = incomingByte;
    timeLastByteReceived = millis();
    bitCount++;
  }
}
