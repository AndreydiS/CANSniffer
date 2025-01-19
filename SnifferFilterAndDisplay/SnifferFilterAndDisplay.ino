//sniff all packets
#include <mcp_can.h>
#include <SPI.h>

#include <Wire.h>
  #include <Adafruit_GFX.h>
  #include <mySSD1306.h> 
  

//#define CanCrystal 16 //MHZ
#define CanCrystal 8 //MHZ

#define canidVol 0x17333110
#define canidWheelButton 0x5bf

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
  

  #define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
  #define SCREEN_ADDRESS 0x3C
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

INT32U canId = 0x0;
unsigned char len = 0;
unsigned char buf[8];
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
    
    CAN.init_Mask(0, 1, 0x1fffffff);
    CAN.init_Filt(0, 1, canidVol);//volume from head unit
    CAN.init_Filt(1, 1, canidVol);//volume from head unit


    CAN.init_Mask(1, 0, 0x07ff);
    CAN.init_Filt(2, 0, canidWheelButton);
    CAN.init_Filt(3, 0, canidWheelButton);

/*
    CAN.init_Mask(0, 0, 0x07ff);
    CAN.init_Filt(0, 0, canidWheelButton);

    CAN.init_Mask(1, 1, 0x1fffffff);
    CAN.init_Filt(2, 1, canidVol);//volume from head unit
//  CAN.init_Filt(3, 1, 0x17330b00);//cam display
*/
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { 
      Serial.println(F("oled fail")); //1160bytes for local variables is not enough for SSD to init. 1226-ok

  }
  //setOledBri(bytDisplayBri);
  display.clearDisplay();
  display.display(); 
  display.setCursor(0, 0);
  display.setTextColor(WHITE);
  display.println("Snif VOL and Wheel");
  
  display.display();
}

byte a=0;

void loop() {
  if (CAN_MSGAVAIL == CAN.checkReceive()) {
    a++;
    CAN.readMsgBuf(&len, buf);
    canId = CAN.getCanId();
    //display.print(canId,HEX);
    if (canidVol == canId) {
      display.print("v");
    }
    if (canidWheelButton == canId) {
      display.print("w");
    }
    
      for (int i = 0; i<len; i++) {
        display.print(",");
        display.print(buf[i],HEX);
      }

      INT8U eflg = CAN.mcp2515_readRegister(MCP_EFLG);//rx1ovr,rx0ovr
      //display.println();
      display.print("+");
      display.println(eflg,HEX);


      display.display(); 
 
	}
  if (a > 7) {
    a = 0;
    display.setCursor(0, 0);
    display.clearDisplay();
    display.display(); 
  }
}
