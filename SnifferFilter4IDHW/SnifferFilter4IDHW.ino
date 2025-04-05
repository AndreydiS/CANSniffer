#include <SPI.h>
#include <mcp_can.h>

#define CanCrystal 16 //MHZ 8(on my controllers) or 16(on UNO)

#define arraySize 4

unsigned long arrCanID[arraySize];
int arrID = 0;
byte i = 0;
byte blnPrint = 0;

int incomingByte;
unsigned long lngWord=0;

INT32U canId = 0x0;
//INT8U canRtr = 0x0;
//INT8U canExF = 0x0;
byte stdFilterCount = 0;
byte extFilterCount = 2;
unsigned char len = 0;
unsigned char buf[8];
String stringOut = "";
MCP_CAN CAN(10);                                            // Set CS to pin 10 //3 for tests only

/*
to try to use mask and filters
  standard canId, in case we need only 1(or 2 canIDs)
    CAN.init_Mask(0, 0, 0x07ff); //bin 111 1111 1111    //1fffffff   
    CAN.init_Filt(0, 0, 0x740);  //canid1
    CAN.init_Filt(1, 0, 0x4c0);  //canid2

    CAN.init_Mask(1, 1, 0x1fffffff); //bin 111 1111 1111    //   
    CAN.init_Filt(0, 1, 0x17330b10);  //canid1
    CAN.init_Filt(1, 1, 0x17330a10);  //canid2


    CAN.init_Mask(0, 0, 0x0700); //bin 111 0000 0000
    CAN.init_Filt(0, 0, 0x700);  //all canids 0x7**
*/

void setup() {    
  Serial.begin(115200);
  START_INIT:
  #if CanCrystal == 8
    if(CAN_OK == CAN.begin(CAN_500KBPS,MCP_8MHz)) {
#endif
#if CanCrystal == 16
    if(CAN_OK == CAN.begin(CAN_500KBPS)) {
#endif
  //if(CAN_OK == CAN.begin(CAN_500KBPS,MCP_8MHz)) {
    Serial.println("CAN BUS Shield init ok!");
  } else {
        Serial.println("CAN BUS Shield init fail");
        delay(100);
        goto START_INIT;
    }
}

void loop(){
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    if (incomingByte == 0x0a) {
      if (lngWord > 0x07ff) {
          CAN.init_Mask(1, 1, 0x1fffffff);
          CAN.init_Filt(extFilterCount, 1, lngWord);
          Serial.println("ext");
          Serial.println(extFilterCount);
          extFilterCount++;
          if (extFilterCount > 5) {
            extFilterCount = 2;
          }

      } else {
          CAN.init_Mask(0, 0, 0x07ff);
          CAN.init_Filt(stdFilterCount, 0, lngWord);
          Serial.println("std");
          Serial.println(stdFilterCount);
          stdFilterCount++;
          if (stdFilterCount > 1) {
            stdFilterCount = 0;
          }
      }
      Serial.println(lngWord,HEX);
      arrCanID[arrID] = lngWord;  
      lngWord=0;
    } else {
      lngWord=lngWord<<4;
      if ((incomingByte > 0x2f) && (incomingByte < 0x3a)) {
          lngWord = lngWord | (incomingByte-0x30);
      } else if ((incomingByte > 0x60) && (incomingByte < 0x67)) {
          lngWord = lngWord | (incomingByte-0x57);
      }
    }
  }

  if (CAN_MSGAVAIL == CAN.checkReceive()) {
    CAN.readMsgBuf(&len, buf);
    canId = CAN.getCanId();
     stringOut = "";
     stringOut.concat(millis());
      //canRtr = CAN.isRemoteRequest();
      //canExF = CAN.isExtendedFrame();
      //stringOut = String(canRtr,HEX);
      //stringOut.concat(",-");
      //stringOut.concat(",");
      //stringOut.concat(String(canExF,HEX));
      if ((canId == 0x5bf)) {
        stringOut.concat(",");
      } else {
        stringOut.concat(",");  
      }
      stringOut.concat(String(canId,HEX));
      for (i=0; i<len; i++) {
        stringOut.concat(",");
        if (buf[i] < 0x10) {
          stringOut.concat(" ");
        }
        stringOut.concat(String(buf[i],HEX));
      }
    //  if ((canId == 0x6af)||(canId == 0x5bf)||(canId==0x17333111)||(canId==0x17330b10)) {
      Serial.print("<");  
      Serial.print(stringOut);
      Serial.println(">");
    //  }
      
    //  }
	}
}
