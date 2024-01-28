#include <SPI.h>
#include "mcp_can.h"
INT32U canId = 0x0;
unsigned char len = 0;
unsigned char buf[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char bu6[6] = {0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char bu4[4] = {0x00,0x00,0x00,0x00};
unsigned char bu3[3] = {0x00,0x00,0x00};

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


byte SendStringAsCan(INT32U cId, String sOut){
    //unsigned char buf[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    byte byteIn;
    byte byteOut;
    unsigned int sLength;
    unsigned int byteCount;
    unsigned int j;
    byte canExtended;
    byteCount = 0;
    sOut.replace(",","");
    sLength = sOut.length();
    j=0;
    for (i=0;i < sLength;i=i+1) {
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
/*        Serial.println(byteCount,HEX);
        Serial.println(cId,HEX);
        Serial.print("buff: ");

    for (i=0;i<byteCount;i++){
        Serial.print(buf[i],HEX);
    }
        Serial.println();
*/  
}


void loop(){
if (Serial.available() > 0) {
    incomingByte = Serial.read();

	if (incomingByte == 0x61) {
      canId = 0x17333111;
      Serial.println("sending for A, test");
      Serial.println(millis());
      SendStringAsCan(canId,"80,31,4c,55,0d,56,65,72");
      SendStringAsCan(canId,"c0,70,69,73,73,27,20,44");
      SendStringAsCan(canId,"c1,69,63,68,48,00,00,0b");
      SendStringAsCan(canId,"c2,54,69,63,20,54,61,63");
      SendStringAsCan(canId,"c3,20,54,6f,65,49,0b,54");
      SendStringAsCan(canId,"c4,69,63,20,54,61,63,20");
      SendStringAsCan(canId,"c5,54,6f,65,4a,00,00,01");
      SendStringAsCan(canId,"c6,00,00,00");
      Serial.println(millis());
	}

	if (incomingByte == 0x30) {
		canId = 0x17333111;
    		Serial.println("sending message for Audio. 0x80 0xc0");
    		buf[0]=0x80; buf[1]=0x31; buf[2]=0x4c; buf[3]=0x55; buf[4]=0x0d; buf[5]=0x30; buf[6]=0x78; buf[7]=0x38; CAN.sendMsgBuf(canId, 1, 8, buf);
    		buf[0]=0xc0; buf[1]=0x30; buf[2]=0x20; buf[3]=0x30; buf[4]=0x78; buf[5]=0x63; buf[6]=0x30; buf[7]=0x20; CAN.sendMsgBuf(canId, 1, 8, buf);
    		buf[0]=0xc1; buf[1]=0x45; buf[2]=0x53; buf[3]=0x53; buf[4]=0x48; buf[5]=0x00; buf[6]=0x00; buf[7]=0x0b; CAN.sendMsgBuf(canId, 1, 8, buf);
    		buf[0]=0xc2; buf[1]=0x41; buf[2]=0x4e; buf[3]=0x44; buf[4]=0x52; buf[5]=0x45; buf[6]=0x59; buf[7]=0x20; CAN.sendMsgBuf(canId, 1, 8, buf);
    		buf[0]=0xc3; buf[1]=0x20; buf[2]=0x21; buf[3]=0x22; buf[4]=0x23; buf[5]=0x49; buf[6]=0x0b; buf[7]=0x24; CAN.sendMsgBuf(canId, 1, 8, buf);
    		buf[0]=0xc4; buf[1]=0x25; buf[2]=0x26; buf[3]=0x27; buf[4]=0x28; buf[5]=0x29; buf[6]=0x2a; buf[7]=0x2b; CAN.sendMsgBuf(canId, 1, 8, buf);
    		buf[0]=0xc5; buf[1]=0x2c; buf[2]=0x2e; buf[3]=0x2f; buf[4]=0x4a; buf[5]=0x00; buf[6]=0x00; buf[7]=0x01; CAN.sendMsgBuf(canId, 1, 8, buf);
    		buf[0]=0xc6; buf[1]=0x00; buf[2]=0x00; buf[3]=0x00;             CAN.sendMsgBuf(canId, 1, 4, buf);
    delay(900);
		Serial.println("sending message for Audio. 0x90 0xd0");
		buf[0]=0x90; buf[1]=0x31; buf[2]=0x4c; buf[3]=0x55; buf[4]=0x0d; buf[5]=0x30; buf[6]=0x78; buf[7]=0x39; CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xd0; buf[1]=0x30; buf[2]=0x20; buf[3]=0x30; buf[4]=0x78; buf[5]=0x64; buf[6]=0x30; buf[7]=0x20; CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xd1; buf[1]=0x45; buf[2]=0x53; buf[3]=0x53; buf[4]=0x48; buf[5]=0x00; buf[6]=0x00; buf[7]=0x0b; CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xd2; buf[1]=0x41; buf[2]=0x4e; buf[3]=0x44; buf[4]=0x52; buf[5]=0x45; buf[6]=0x59; buf[7]=0x20; CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xd3; buf[1]=0x20; buf[2]=0x21; buf[3]=0x22; buf[4]=0x23; buf[5]=0x49; buf[6]=0x0b; buf[7]=0x24; CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xd4; buf[1]=0x25; buf[2]=0x26; buf[3]=0x27; buf[4]=0x28; buf[5]=0x29; buf[6]=0x2a; buf[7]=0x2b; CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xd5; buf[1]=0x2c; buf[2]=0x2e; buf[3]=0x2f; buf[4]=0x4a; buf[5]=0x00; buf[6]=0x00; buf[7]=0x01; CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xd6; buf[1]=0x00; buf[2]=0x00; buf[3]=0x00;							CAN.sendMsgBuf(canId, 1, 4, buf);
		delay(900);
		Serial.println("sending message for Audio. 0xa0 0xe0");
		buf[0]=0xa0; buf[1]=0x31; buf[2]=0x4c; buf[3]=0x55; buf[4]=0x0d; buf[5]=0x30; buf[6]=0x78; buf[7]=0x61; CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xe0; buf[1]=0x30; buf[2]=0x20; buf[3]=0x30; buf[4]=0x78; buf[5]=0x65; buf[6]=0x30; buf[7]=0x20; CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xe1; buf[1]=0x45; buf[2]=0x53; buf[3]=0x53; buf[4]=0x48; buf[5]=0x00; buf[6]=0x00; buf[7]=0x0b; CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xe2; buf[1]=0x41; buf[2]=0x4e; buf[3]=0x44; buf[4]=0x52; buf[5]=0x45; buf[6]=0x59; buf[7]=0x20;	CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xe3; buf[1]=0x20; buf[2]=0x21; buf[3]=0x22; buf[4]=0x23; buf[5]=0x49; buf[6]=0x0b; buf[7]=0x24;	CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xe4; buf[1]=0x25; buf[2]=0x26; buf[3]=0x27; buf[4]=0x28; buf[5]=0x29; buf[6]=0x2a; buf[7]=0x2b;	CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xe5; buf[1]=0x2c; buf[2]=0x2e; buf[3]=0x2f; buf[4]=0x4a; buf[5]=0x00; buf[6]=0x00; buf[7]=0x01;	CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xe6; buf[1]=0x00; buf[2]=0x00; buf[3]=0x00;							CAN.sendMsgBuf(canId, 1, 4, buf);
		delay(900);
		Serial.println("sending message for Audio. 0xb0 0xf0");
		buf[0]=0xb0; buf[1]=0x31; buf[2]=0x4c; buf[3]=0x55; buf[4]=0x0d; buf[5]=0x30; buf[6]=0x78; buf[7]=0x62; CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xf0; buf[1]=0x30; buf[2]=0x20; buf[3]=0x30; buf[4]=0x78; buf[5]=0x66; buf[6]=0x30; buf[7]=0x20; CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xf1; buf[1]=0x45; buf[2]=0x53; buf[3]=0x53; buf[4]=0x48; buf[5]=0x00; buf[6]=0x00; buf[7]=0x0b; CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xf2; buf[1]=0x41; buf[2]=0x4e; buf[3]=0x44; buf[4]=0x52; buf[5]=0x45; buf[6]=0x59; buf[7]=0x20;	CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xf3; buf[1]=0x20; buf[2]=0x21; buf[3]=0x22; buf[4]=0x23; buf[5]=0x49; buf[6]=0x0b; buf[7]=0x24;	CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xf4; buf[1]=0x25; buf[2]=0x26; buf[3]=0x27; buf[4]=0x28; buf[5]=0x29; buf[6]=0x2a; buf[7]=0x2b;	CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xf5; buf[1]=0x2c; buf[2]=0x2e; buf[3]=0x2f; buf[4]=0x4a; buf[5]=0x00; buf[6]=0x00; buf[7]=0x01;	CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xf6; buf[1]=0x00; buf[2]=0x00; buf[3]=0x00;							CAN.sendMsgBuf(canId, 1, 4, buf);
		delay(900);


	}

	if (incomingByte == 0x31) {
   		canId = 0x17330b10;
      	Serial.println("1. b10 enable cam");
    	Serial.println(canId, HEX);
     SendStringAsCan(0x6af,"70,00,35,32,1e,00,00,00");
    	bu3[0]=0x42; bu3[1]=0xd6; bu3[2]=0x38; CAN.sendMsgBuf(canId, 1, 3, bu3); 
		delay(45);
    	bu6[0]=0x42; bu6[1]=0xda; bu6[2]=0xf1; bu6[3]=0x01; bu6[4]=0x01; bu6[5]=0x01; CAN.sendMsgBuf(canId, 1, 6, bu6);
      	delay(45);
    	bu4[0]=0x42; bu4[1]=0xe2; bu4[2]=0x01; bu4[3]=0x01; CAN.sendMsgBuf(canId, 1, 4, bu4);
      	delay(45);
    	bu6[0]=0x42; bu6[1]=0xda; bu6[2]=0x11; bu6[3]=0x01; bu6[4]=0x01; bu6[5]=0x01; CAN.sendMsgBuf(canId, 1, 6, bu6);
    SendStringAsCan(0x6af,"71,00,35,32,1e,00,00,00");
      	delay(1000);
      	bu3[0]=0x32; bu3[1]=0xd0; bu3[2]=0x00; CAN.sendMsgBuf(canId, 1, 3, bu3); 
		delay(45);
	}

	if (incomingByte == 0x32) {
   		canId = 0x17330b00;
      	Serial.println("2. b00 enable cam");
    	Serial.println(canId, HEX);
    	bu6[0]=0x22; bu6[1]=0xda; bu6[2]=0x11; bu6[3]=0x01; bu6[4]=0x01; bu6[5]=0x01; CAN.sendMsgBuf(canId, 1, 8, bu6);
      	delay(45);
	}


	if (incomingByte == 0x33) {
    canId = 0x17330a10;
      Serial.println("3. a10 enable parktronic");
    Serial.println(canId, HEX);
    bu4[0]=0x42; bu4[1]=0x95; bu4[2]=0xf1; bu4[3]=0x01; CAN.sendMsgBuf(canId, 1, 4, bu4);
      delay(45);
    buf[0]=0x42; buf[1]=0x92; buf[2]=0xff; buf[3]=0xff; buf[4]=0xff; buf[5]=0xff; buf[6]=0x00; buf[7]=0x00; CAN.sendMsgBuf(canId, 1, 8, buf);
      delay(45);
    bu4[0]=0x42; bu4[1]=0x95; bu4[2]=0x11; bu4[3]=0x01; CAN.sendMsgBuf(canId, 1, 4, bu4);
      delay(45);
    buf[0]=0x42; buf[1]=0x93; buf[2]=0xff; buf[3]=0xff; buf[4]=0xff; buf[5]=0xff; buf[6]=0x00; buf[7]=0x00; CAN.sendMsgBuf(canId, 1, 8, buf);
      delay(45);
    buf[0]=0x42; buf[1]=0x9f; buf[2]=0x00; buf[3]=0x20; buf[4]=0x5d; buf[5]=0x02; buf[6]=0x50; buf[7]=0x03; CAN.sendMsgBuf(canId, 1, 8, buf);
      delay(45);
    bu4[0]=0x42; bu4[1]=0x94; bu4[2]=0x00; bu4[3]=0x01; CAN.sendMsgBuf(canId, 1, 4, bu4);
      delay(45);
    buf[0]=0x42; buf[1]=0x9f; buf[2]=0x00; buf[3]=0x20; buf[4]=0x5d; buf[5]=0x02; buf[6]=0x50; buf[7]=0x03; CAN.sendMsgBuf(canId, 1, 8, buf);
      delay(45);
	CAN.sendMsgBuf(canId, 1, 8, buf);
      delay(45);
	CAN.sendMsgBuf(canId, 1, 8, buf);
      delay(45);
	CAN.sendMsgBuf(canId, 1, 8, buf);
      delay(45);
	CAN.sendMsgBuf(canId, 1, 8, buf);
      delay(45);
	CAN.sendMsgBuf(canId, 1, 8, buf);
      delay(45);
	CAN.sendMsgBuf(canId, 1, 8, buf);
      delay(45);
	CAN.sendMsgBuf(canId, 1, 8, buf);
      delay(45);
	CAN.sendMsgBuf(canId, 1, 8, buf);
    buf[0]=0x42; buf[1]=0x93; buf[2]=0xff; buf[3]=0xff; buf[4]=0xff; buf[5]=0xff; buf[6]=0x00; buf[7]=0x00; CAN.sendMsgBuf(canId, 1, 8, buf);
      delay(45);
	buf[0]=0x42; buf[1]=0x9f; buf[2]=0x00; buf[3]=0x20; buf[4]=0x5d; buf[5]=0x02; buf[6]=0x50; buf[7]=0x03; CAN.sendMsgBuf(canId, 1, 8, buf);
      delay(45);
	CAN.sendMsgBuf(canId, 1, 8, buf);
      delay(45);
	CAN.sendMsgBuf(canId, 1, 8, buf);
      delay(45);
	CAN.sendMsgBuf(canId, 1, 8, buf);
      delay(45);
	CAN.sendMsgBuf(canId, 1, 8, buf);
      delay(45);
	CAN.sendMsgBuf(canId, 1, 8, buf);
      delay(45);
	CAN.sendMsgBuf(canId, 1, 8, buf);
      delay(45);
	CAN.sendMsgBuf(canId, 1, 8, buf);
      delay(45);
	CAN.sendMsgBuf(canId, 1, 8, buf);

	}

	if (incomingByte == 0x34) {
   		canId = 0x17330a00;
      	Serial.println("4. a00 enable parktronic");
    	Serial.println(canId, HEX);
    	bu4[0]=0x22; bu4[1]=0x95; bu4[2]=0x01; bu4[3]=0x00; CAN.sendMsgBuf(canId, 1, 4, bu4);
      	delay(45);
	}



	if (incomingByte == 0x35) {
		canId = 0x17333111;
		Serial.println("sending for 4. aux icon?");
		Serial.println(canId, HEX);
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
	}

	if (incomingByte == 0x36) {
		canId = 0x17330b10;
    	Serial.println("sending for 6");
		Serial.println(canId, HEX);
		bu6[0]=0x42; bu6[1]=0xda; bu6[2]=0x10; bu6[3]=0x00; bu6[4]=0x00; bu6[5]=0x00; CAN.sendMsgBuf(canId, 1, 6, bu6);
   		delay(45);
		bu4[0]=0x42; bu4[1]=0xe2; bu4[2]=0x00; bu4[3]=0x01; CAN.sendMsgBuf(canId, 1, 4, bu4);
   		delay(45);
		bu3[0]=0x42; bu3[1]=0xd6; bu3[2]=0x08; CAN.sendMsgBuf(canId, 1, 3, bu3);
   		delay(45);
		bu6[0]=0x42; bu6[1]=0xda; bu6[2]=0x00; bu6[3]=0x00; bu6[4]=0x00; bu6[5]=0x00; CAN.sendMsgBuf(canId, 1, 6, bu6);
		Serial.println("waiting 6 sec");
		delay(6000);
	  Serial.println("sending b00");
  	canId = 0x17330b00;
		bu6[0]=0x22; bu6[1]=0xda; bu6[2]=0x00; bu6[3]=0x00; bu6[4]=0x00; bu6[5]=0x00; CAN.sendMsgBuf(canId, 1, 6, bu6);
		delay(45);
  	}

	if (incomingByte == 0x37) {
		canId = 0x17330a10;
    	Serial.println("7. attempt 1");
		Serial.println(canId, HEX);
		bu4[0]=0x42; bu4[1]=0x95; bu4[2]=0x10; bu4[3]=0x00; CAN.sendMsgBuf(canId, 1, 4, bu4);
   		delay(45);
		bu4[0]=0x42; bu4[1]=0x94; bu4[2]=0x00; bu4[3]=0x00; CAN.sendMsgBuf(canId, 1, 4, bu4);
   		delay(45);
		bu4[0]=0x42; bu4[1]=0x95; bu4[2]=0x00; bu4[3]=0x00; CAN.sendMsgBuf(canId, 1, 4, bu4);
   		delay(45);
		bu4[0]=0x42; bu4[1]=0x95; bu4[2]=0x00; bu4[3]=0x00; CAN.sendMsgBuf(canId, 1, 4, bu4);
		Serial.println("waiting 3 sec");
    delay(6000);
    Serial.println("sending a00");
		canId = 0x17330a00;
		bu4[0]=0x22; bu4[1]=0x95; bu4[2]=0x01; bu4[3]=0x00; CAN.sendMsgBuf(canId, 1, 4, bu4);
   		delay(80);
		bu4[0]=0x22; bu4[1]=0x95; bu4[2]=0x00; bu4[3]=0x00; CAN.sendMsgBuf(canId, 1, 4, bu4);
  	}


	if (incomingByte == 0x38) {
		canId = 0x17330b10;
    		Serial.println("sending for 8");
		Serial.println(canId, HEX);
		unsigned char buf883[3] = {0x42,0xd6,0x38};
    		CAN.sendMsgBuf(canId, 1, 3, buf883);
    		delay(50);
		unsigned char buf88[6] = {0x42,0xda,0xf1,0x01,0x01,0x01};
    		CAN.sendMsgBuf(canId, 1, 6, buf88);
    		delay(50);
		unsigned char buf882[4] = {0x42,0xe2,0x01,0x01};
    		CAN.sendMsgBuf(canId, 1, 4, buf882);
    		delay(50);
		buf88[2]=0x11;
    		CAN.sendMsgBuf(canId, 1, 6, buf88);
		delay(50);
  	}

	if (incomingByte == 0x39) {
		canId = 0x17333111;
    Serial.println("sending message for Audio. 0x80 0xc0");
    buf[0]=0x80; buf[1]=0x31; buf[2]=0x4c; buf[3]=0x55; buf[4]=0x0d; buf[5]=0x30; buf[6]=0x78; buf[7]=0x38; CAN.sendMsgBuf(canId, 1, 8, buf);
    buf[0]=0xc0; buf[1]=0x30; buf[2]=0x20; buf[3]=0x30; buf[4]=0x78; buf[5]=0x63; buf[6]=0x30; buf[7]=0x20; CAN.sendMsgBuf(canId, 1, 8, buf);
    buf[0]=0xc1; buf[1]=0x45; buf[2]=0x53; buf[3]=0x53; buf[4]=0x48; buf[5]=0x00; buf[6]=0x00; buf[7]=0x0b; CAN.sendMsgBuf(canId, 1, 8, buf);
    buf[0]=0xc2; buf[1]=0x41; buf[2]=0x4e; buf[3]=0x44; buf[4]=0x52; buf[5]=0x45; buf[6]=0x59; buf[7]=0x20; CAN.sendMsgBuf(canId, 1, 8, buf);
    buf[0]=0xc3; buf[1]=0x20; buf[2]=0x21; buf[3]=0x22; buf[4]=0x23; buf[5]=0x49; buf[6]=0x0b; buf[7]=0x24; CAN.sendMsgBuf(canId, 1, 8, buf);
    buf[0]=0xc4; buf[1]=0x25; buf[2]=0x26; buf[3]=0x27; buf[4]=0x28; buf[5]=0x29; buf[6]=0x2a; buf[7]=0x2b; CAN.sendMsgBuf(canId, 1, 8, buf);
    buf[0]=0xc5; buf[1]=0x2c; buf[2]=0x2e; buf[3]=0x2f; buf[4]=0x4a; buf[5]=0x00; buf[6]=0x00; buf[7]=0x01; CAN.sendMsgBuf(canId, 1, 8, buf);
    bu4[0]=0xc6; bu4[1]=0x00; bu4[2]=0x00; bu4[3]=0x00;             CAN.sendMsgBuf(canId, 1, 4, bu4);
    delay(900);
		Serial.println("sending message for Audio. 0x90 0xd0");
		buf[0]=0x90; buf[1]=0x31; buf[2]=0x4c; buf[3]=0x55; buf[4]=0x0d; buf[5]=0x30; buf[6]=0x78; buf[7]=0x39; CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xd0; buf[1]=0x30; buf[2]=0x20; buf[3]=0x30; buf[4]=0x78; buf[5]=0x64; buf[6]=0x30; buf[7]=0x20; CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xd1; buf[1]=0x45; buf[2]=0x53; buf[3]=0x53; buf[4]=0x48; buf[5]=0x00; buf[6]=0x00; buf[7]=0x0b; CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xd2; buf[1]=0x41; buf[2]=0x4e; buf[3]=0x44; buf[4]=0x52; buf[5]=0x45; buf[6]=0x59; buf[7]=0x20;	CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xd3; buf[1]=0x20; buf[2]=0x21; buf[3]=0x22; buf[4]=0x23; buf[5]=0x49; buf[6]=0x0b; buf[7]=0x24;	CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xd4; buf[1]=0x25; buf[2]=0x26; buf[3]=0x27; buf[4]=0x28; buf[5]=0x29; buf[6]=0x2a; buf[7]=0x2b;	CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xd5; buf[1]=0x2c; buf[2]=0x2e; buf[3]=0x2f; buf[4]=0x4a; buf[5]=0x00; buf[6]=0x00; buf[7]=0x01;	CAN.sendMsgBuf(canId, 1, 8, buf);
		bu4[0]=0xd6; bu4[1]=0x00; bu4[2]=0x00; bu4[3]=0x00;							CAN.sendMsgBuf(canId, 1, 4, bu4);
		delay(900);
		Serial.println("sending message for Audio. 0xa0 0xe0");
		buf[0]=0xa0; buf[1]=0x31; buf[2]=0x4c; buf[3]=0x55; buf[4]=0x0d; buf[5]=0x30; buf[6]=0x78; buf[7]=0x61; CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xe0; buf[1]=0x30; buf[2]=0x20; buf[3]=0x30; buf[4]=0x78; buf[5]=0x65; buf[6]=0x30; buf[7]=0x20; CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xe1; buf[1]=0x45; buf[2]=0x53; buf[3]=0x53; buf[4]=0x48; buf[5]=0x00; buf[6]=0x00; buf[7]=0x0b; CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xe2; buf[1]=0x41; buf[2]=0x4e; buf[3]=0x44; buf[4]=0x52; buf[5]=0x45; buf[6]=0x59; buf[7]=0x20;	CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xe3; buf[1]=0x20; buf[2]=0x21; buf[3]=0x22; buf[4]=0x23; buf[5]=0x49; buf[6]=0x0b; buf[7]=0x24;	CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xe4; buf[1]=0x25; buf[2]=0x26; buf[3]=0x27; buf[4]=0x28; buf[5]=0x29; buf[6]=0x2a; buf[7]=0x2b;	CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xe5; buf[1]=0x2c; buf[2]=0x2e; buf[3]=0x2f; buf[4]=0x4a; buf[5]=0x00; buf[6]=0x00; buf[7]=0x01;	CAN.sendMsgBuf(canId, 1, 8, buf);
		bu4[0]=0xe6; bu4[1]=0x00; bu4[2]=0x00; bu4[3]=0x00;							CAN.sendMsgBuf(canId, 1, 4, bu4);
		delay(900);
		Serial.println("sending message for Audio. 0xb0 0xf0");
		buf[0]=0xb0; buf[1]=0x31; buf[2]=0x4c; buf[3]=0x55; buf[4]=0x0d; buf[5]=0x30; buf[6]=0x78; buf[7]=0x62; CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xf0; buf[1]=0x30; buf[2]=0x20; buf[3]=0x30; buf[4]=0x78; buf[5]=0x66; buf[6]=0x30; buf[7]=0x20; CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xf1; buf[1]=0x45; buf[2]=0x53; buf[3]=0x53; buf[4]=0x48; buf[5]=0x00; buf[6]=0x00; buf[7]=0x0b; CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xf2; buf[1]=0x41; buf[2]=0x4e; buf[3]=0x44; buf[4]=0x52; buf[5]=0x45; buf[6]=0x59; buf[7]=0x20;	CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xf3; buf[1]=0x20; buf[2]=0x21; buf[3]=0x22; buf[4]=0x23; buf[5]=0x49; buf[6]=0x0b; buf[7]=0x24;	CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xf4; buf[1]=0x25; buf[2]=0x26; buf[3]=0x27; buf[4]=0x28; buf[5]=0x29; buf[6]=0x2a; buf[7]=0x2b;	CAN.sendMsgBuf(canId, 1, 8, buf);
		buf[0]=0xf5; buf[1]=0x2c; buf[2]=0x2e; buf[3]=0x2f; buf[4]=0x4a; buf[5]=0x00; buf[6]=0x00; buf[7]=0x01;	CAN.sendMsgBuf(canId, 1, 8, buf);
		bu4[0]=0xf6; bu4[1]=0x00; bu4[2]=0x00; bu4[3]=0x00;							CAN.sendMsgBuf(canId, 1, 4, bu4);
		delay(900);


	} 
  

	delay(100);
}    
}
