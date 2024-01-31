#include <SPI.h>
#include "mcp_can.h"

MCP_CAN CAN(10); // Set CS to pin 10

#define canId 0x17333111
#define delayLastMessage 100


byte incomingByte;
byte bitCount=0;
byte availableBytes = 0;
byte bufsize = 0;
unsigned long timeLastByteReceived = 0;

//unsigned char buf[256] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char buf[256];
unsigned char bufToSend[8];

void setup() {
  Serial.begin(9600);
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

void loop() {

if (((millis() - timeLastByteReceived) > 1000) && (bitCount>0)) {
  Serial.println("No new bytes received. Sending. ");
  byte j=0;
  for (int i = 0; i <= (bitCount-1); i++) {
    bufToSend[j] = buf[i];
    Serial.print(buf[i],HEX);  
    Serial.print(",");  
    if (j >=7) {
      Serial.println(); 
      Serial.println("Sending 8 bytes: "); 
      CAN.sendMsgBuf(canId, 1, 8, bufToSend);
      j=0;
    } else {
      j++;
    }
  }
  if (j >0) {
      Serial.print("Sending last chunk of bytes: ");  
      Serial.println(j);  
      CAN.sendMsgBuf(canId, 1, j, bufToSend);
  }
  bitCount = 0;
}

if (Serial.available() > 0) {
  incomingByte = Serial.read();
  buf[bitCount] = incomingByte;
  timeLastByteReceived = millis();
  bitCount++;
}
/*
  availableBytes = Serial.available();
  if (availableBytes > 0) {

    if (availableBytes >=8) { //8 or more bytes received - reading
      Serial.print("bytes read by8 ");  
      Serial.println(availableBytes);  
      bitCount+=8;
      bufsize = 8;
    } else {
      delay(delayLastMessage);  
      if (availableBytes == Serial.available()) { //no new bytes received in 100ms - reading as is
        bufsize = availableBytes;
        Serial.println("no new bytes received in 100ms - reading as is. bytes: ");  
        Serial.println(availableBytes);  
      }
    }
    if (bufsize > 0) {
      Serial.readBytes(buf, bufsize);
      Serial.print("Sending CAN Message: ");  
      for (int i = 0; i <= (bufsize-1); i++) {
        Serial.print(buf[i],HEX);  
        Serial.print(",");  
      }
      Serial.println("");  
      CAN.sendMsgBuf(canId, 1, bufsize, buf);
      bufsize = 0;
    }
    */

}
