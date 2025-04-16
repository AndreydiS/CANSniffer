#include <SPI.h>
#include <mcp_can.h>

#define CanCrystal 16 //MHZ 8(on my controllers) or 16(on UNO)

MCP_CAN CAN(10); // Set CS to pin 10

INT32U canId = 0x0;
INT32U tempCanId = 0x0;

byte sniffingMode = 0;
byte stdFilterCount = 0;
byte extFilterCount = 2;
byte extFrame = 0;
String stringOut = "";
unsigned char len = 0;

byte incomingByte;
byte bitCount=0;
byte availableBytes = 0;
byte bufsize = 0;
unsigned long timeLastByteReceived = 0;
unsigned long timeLastSent = 0;

//unsigned char buf[256];
unsigned char buf[16];
unsigned char bufs[8];
unsigned char bufToSend[8];

void setup() {
  //Serial.begin(115200);
  Serial.begin(19200);
	START_INIT:
    #if CanCrystal == 8
      if(CAN_OK == CAN.begin(CAN_500KBPS,MCP_8MHz)) {
    #endif
    #if CanCrystal == 16
      if(CAN_OK == CAN.begin(CAN_500KBPS)) {
    #endif
        Serial.println("CAN ok. CMD send: 74 20 <canid_4b> <payloadlen_1b> <payload>");
        Serial.println("CAN ok. CMD sniff: 73 20 <canid_4b>");
      } else {
          Serial.println("CAN BUS Shield init fail");
          delay(1000);
          //goto START_INIT;
      }
}

void loop() {
  if (((millis() - timeLastByteReceived) > 100) && (bitCount>0)) {
    bitCount=0;
    Serial.println("reset wrong input");
  }

  if (bitCount>5) {
    if (bitCount == 6) {
      canId = 0x00;
      for (byte i = 2; i <= 5; i++) {
        //Serial.print(buf[i],HEX);
        //Serial.print("~");
        //Serial.println();
        tempCanId = buf[i];
        canId = canId*0x100 + tempCanId;
      }
      if (canId > 0x7ff) {extFrame = 1;} else {extFrame = 0;}
      Serial.print("CAN ID: "); Serial.print(canId, HEX); 
      Serial.print(" Ext: ");   Serial.println(extFrame, HEX); 
    }
    if ((bitCount>5)&&(buf[0] == 0x73)) { //sniffing
      Serial.println("Sniffing ");
      sniffingMode = 1;
      if (canId == 0x0) {
        Serial.print("Sniffing all");
      } else {
        if (extFrame == 1) {
          CAN.init_Mask(1, 1, 0x1fffffff);
          CAN.init_Filt(extFilterCount, 1, canId);
          Serial.print("ext ");
          Serial.println(extFilterCount);
          extFilterCount++;
          if (extFilterCount > 5) {
            extFilterCount = 2;
          }
        } else {
          CAN.init_Mask(0, 0, 0x07ff);
          CAN.init_Filt(stdFilterCount, 0, canId);
          Serial.print("std ");
          Serial.println(stdFilterCount);
          stdFilterCount++;
          if (stdFilterCount > 1) {
            stdFilterCount = 0;
          }
        }
      }
      bitCount = 0;
    }
    if ((bitCount>6)&&(buf[0] == 0x74)&&(bitCount>=(buf[6]+7))) { //sending
      sniffingMode = 0;
      Serial.println("Sending ");
      sniffingMode = 0;
      byte payloadLength = 1;
      payloadLength = buf[6];
      Serial.print("payload len: ");   Serial.println(payloadLength, HEX); 
      byte j=0;
      Serial.print("payload: ");
      for (byte i = 7; i <= (bitCount-1); i++) {
          bufToSend[j] = buf[i];
          j++;
          Serial.print(buf[i],HEX); Serial.print(",");  
      }
      CAN.sendMsgBuf(canId, extFrame, payloadLength, bufToSend);
      Serial.println("");
      bitCount = 0;
    }    
  }

  if (sniffingMode == 1) {
    if (CAN_MSGAVAIL == CAN.checkReceive()) {
      CAN.readMsgBuf(&len, bufs);
      canId = CAN.getCanId();
      stringOut = "<";
      stringOut.concat(millis());
        stringOut.concat(String(canId,HEX));
        for (byte i=0; i<len; i++) {
          stringOut.concat(",");
          if (bufs[i] < 0x10) {
            stringOut.concat(" ");
          }
          stringOut.concat(String(bufs[i],HEX));
        }
        stringOut.concat(">"); 
        Serial.println(stringOut);
        //Serial.println(">");
    }
  }
  
  if (Serial.available() > 0) { //populating buffer
    incomingByte = Serial.read();
    //Serial.print(incomingByte,HEX);
    //Serial.print(",");
    //Serial.println();

    buf[bitCount] = incomingByte;
    timeLastByteReceived = millis();
    bitCount++;
  }
}
