#include <SPI.h>
#include "mcp_can.h"
INT32U canId = 0x0;
unsigned char len = 0;
byte incomingByte;

#define CANCRYSTAL 8 // 8 or 16 MHZ

MCP_CAN CAN(10);                                            // Set CS to pin 10

void setup() {   
	Serial.begin(115200);
	START_INIT:
#if CANCRYSTAL == 8
	if(CAN_OK == CAN.begin(CAN_500KBPS,MCP_8MHz)) {
#endif
#if CANCRYSTAL == 16
	if(CAN_OK == CAN.begin(CAN_500KBPS)) {
#endif
        	Serial.println("CAN BUS Shield init ok!");
    	} else {
        	Serial.println("CAN BUS Shield init fail");
	        Serial.println("Init CAN BUS Shield again");
        	delay(100);
	        goto START_INIT;
    	}
}


byte SendStringAsCan(INT32U cId, String sOut){
  unsigned char buf[8] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
    byte byteIn;
    byte byteOut;
    byte byteCount;
    byte j;
    byte i;
    byte canExtended;

    byteCount = 0;
    sOut.replace(",","");
    //sLength = sOut.length();
    j=0;
    for (i=0;i < sOut.length();i=i+1) {
      j++;
      byteIn = sOut.charAt(i);
      byteOut = byteOut<<4;
      if ((byteIn > 0x2f) && (byteIn < 0x3a)) {
          byteOut = byteOut | (byteIn-0x30);
      } else if ((byteIn > 0x60) && (byteIn < 0x67)) {
          byteOut = byteOut | (byteIn-0x57);
      }  
      if (j > 1) {
        buf[byteCount]=byteOut;
        byteCount++;
        byteOut=0;
        j=0;  
      }
    }
    canExtended = 0;
    if (cId > 0x7ff) {
      canExtended = 1;
    }
    CAN.sendMsgBuf(cId, canExtended, byteCount, buf); 
}


void loop(){
  String strOut;
if (Serial.available() > 0) {
    incomingByte = Serial.read();

	if (incomingByte == 0x61) {
      canId = 0x17333111;
      Serial.println("sending for A, test");
      Serial.println(millis());
      strOut="80,31,4c,55,0d,56,65,72";SendStringAsCan(canId,strOut);
      strOut="c0,70,69,73,73,27,20,44";SendStringAsCan(canId,strOut);
      strOut=String("c1,69,63,68,48,00,00,0b");SendStringAsCan(canId,strOut);
      SendStringAsCan(canId,String("c2,54,69,63,20,54,61,63"));
      SendStringAsCan(canId,"c3,20,54,6f,65,49,0b,54");
      SendStringAsCan(canId,"c4,69,63,20,54,61,63,20");
      SendStringAsCan(canId,"c5,54,6f,65,4a,00,00,01");
      SendStringAsCan(canId,"c6,00,00,00");
      Serial.println(millis());
	}

  
  
	if (incomingByte == 0x30) {
		canId = 0x17333111;
    	Serial.println("sending message for Audio. 0x80 0xc0");
      SendStringAsCan(canId,"80,31,4c,55,0d,30,78,38"); //0x8
		  SendStringAsCan(canId,"c0,30,20,30,78,63,30,20");
    	SendStringAsCan(canId,"c1,45,53,53,48,00,00,0b");
    	SendStringAsCan(canId,"c2,41,4e,44,52,45,59,20");
    	SendStringAsCan(canId,"c3,20,21,22,23,49,0b,24");
    	SendStringAsCan(canId,"c4,25,26,27,28,29,2a,2b");
    	SendStringAsCan(canId,"c5,2c,2e,2f,4a,00,00,01");
    	SendStringAsCan(canId,"c6,00,00,00");
		delay(1900);
		Serial.println("sending message for Audio. 0x90 0xd0");
		SendStringAsCan(canId,"90,31,4c,55,0d,30,78,39");
		SendStringAsCan(canId,"d0,30,20,30,78,64,30,20");
		SendStringAsCan(canId,"d1,45,53,53,48,00,00,0b");
		SendStringAsCan(canId,"d2,41,4e,44,52,45,59,20");
		SendStringAsCan(canId,"d3,20,21,22,23,49,0b,24");
		SendStringAsCan(canId,"d4,25,26,27,28,29,2a,2b");
		SendStringAsCan(canId,"d5,2c,2e,2f,4a,00,00,01");
		SendStringAsCan(canId,"d6,00,00,00");
		delay(1900);
		Serial.println("sending message for Audio. 0xa0 0xe0");
		SendStringAsCan(canId,"a0,31,4c,55,0d,30,78,61");
		SendStringAsCan(canId,"e0,30,20,30,78,65,30,20");
		SendStringAsCan(canId,"e1,45,53,53,48,00,00,0b");
		SendStringAsCan(canId,"e2,41,4e,44,52,45,59,20");
		SendStringAsCan(canId,"e3,20,21,22,23,49,0b,24");
		SendStringAsCan(canId,"e4,25,26,27,28,29,2a,2b");
		SendStringAsCan(canId,"e5,2c,2e,2f,4a,00,00,01");
		SendStringAsCan(canId,"e6,00,00,00");
		delay(1900);
		Serial.println("sending message for Audio. 0xb0 0xf0");
		SendStringAsCan(canId,"b0,31,4c,55,0d,30,78,62");
		SendStringAsCan(canId,"f0,30,20,30,78,66,30,20");
		SendStringAsCan(canId,"f1,45,53,53,48,00,00,0b");
		SendStringAsCan(canId,"f2,41,4e,44,52,45,59,20");
		SendStringAsCan(canId,"f3,20,21,22,23,49,0b,24");
		SendStringAsCan(canId,"f4,25,26,27,28,29,2a,2b");
		SendStringAsCan(canId,"f5,2c,2e,2f,4a,00,00,01");
		SendStringAsCan(canId,"f6,00,00,00");
		delay(1900);
	}

	if (incomingByte == 0x31) {
      	Serial.println("1. b10 enable cam");
	}

	if (incomingByte == 0x32) {
      	Serial.println("2. b00 enable cam");
//   		canId = 0x17330b00;

	}


	if (incomingByte == 0x33) {
		Serial.println("3. a10 enable parktronic");
//    	canId = 0x17330a10;
	}

	if (incomingByte == 0x34) {
		Serial.println("4.");
	}



	if (incomingByte == 0x35) {
		Serial.println("5. aux icon?");
   /*
		canId = 0x17333111;
		buf[0]=0x4c; buf[1]=0x50; buf[2]=0x0d; buf[3]=0x06; buf[4]=0x00; buf[5]=0x07; buf[6]=0x30; buf[7]=0x00; CAN.sendMsgBuf(canId, 1, 8, buf);
		delay(3000);
		Serial.println("sending for 4. aux2? not clear?");
		buf[0]=0x4c; buf[1]=0x50; buf[2]=0x0d; buf[3]=0x06; buf[4]=0x00; buf[5]=0x07; buf[6]=0x00; buf[7]=0x00; CAN.sendMsgBuf(canId, 1, 8, buf);
		delay(3000);
		Serial.println("sending for 4. usb");
		buf[0]=0x4c; buf[1]=0x50; buf[2]=0x13; buf[3]=0x04; buf[4]=0x00; buf[5]=0x07; buf[6]=0x00; buf[7]=0x00; CAN.sendMsgBuf(canId, 1, 8, buf);
		delay(3000);
		Serial.println("sending for 4. radio?");
		buf[0]=0x4c; buf[1]=0x50; buf[2]=0x16; buf[3]=0x05; buf[4]=0x00; buf[5]=0x07; buf[6]=0x10; buf[7]=0x00; CAN.sendMsgBuf(canId, 1, 8, buf);
	*/
	}

	if (incomingByte == 0x36) {
    	Serial.println("6. sending");
//		canId = 0x17330b10;
  	}

	if (incomingByte == 0x37) {
		Serial.println("7. attempt 1");
//		canId = 0x17330a10;

  	}

	if (incomingByte == 0x38) {
    	Serial.println("8. sending for 8");
		//canId = 0x17330b10;
  	}

	if (incomingByte == 0x39) {
    	Serial.println("9. Sending message for Audio");
	} 
  
	//delay(100);
}    
}
