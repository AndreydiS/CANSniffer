#include <mcp_can.h>
#include <SPI.h>

#define canidWheelButton 0x5bf
#define canidVoltage 0x663
#define canidVol 0x1733311
#define pinCANShield 10 //Chip Select pin for CAN module MCP2515
//#define defCanCrystal MCP_8MHz //MCP_16MHz | MCP_8MHz
#define defCanCrystal MCP_16MHz //MCP_16MHz | MCP_8MHz

MCP_CAN CAN(pinCANShield);                                      // Set CS to pin 10

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  Serial.begin(115200);
  // init can bus, baudrate: 500k
  if(CAN.begin(CAN_500KBPS,defCanCrystal) == CAN_OK) Serial.print("can init ok!!\r\n");
  else Serial.print("Can init fail!!\r\n");
}

unsigned char stmpWheelButton[8] = {0x0c, 1, 2, 3, 4, 5, 6, 7};
unsigned char stmpVoltage[8] = {0x00, 1, 2, 3, 4, 0x28, 6, 7};
unsigned char stmpVolume[8] = {0x3C, 0x52, 0x04, 3, 4, 5, 6, 7};
unsigned char stmpVolume2[8] = {0x3C, 0x6F, 2, 3, 4, 0x10, 6, 7};
void loop()
{
  // send data:  id = 0x00, standrad flame, data len = 8, stmp: data buf
  if (digitalRead(2)) {
    CAN.sendMsgBuf(canidWheelButton, 0, 8, stmpWheelButton);  
    Serial.println("Wheel button message sent");
    delay(50);
    CAN.sendMsgBuf(canidVoltage, 0, 8, stmpVoltage);  
    Serial.println("voltage button message sent");
    delay(50);
  }
  if (digitalRead(3)) {
    CAN.sendMsgBuf(canidVol, 1, 8, stmpVolume);  
    Serial.println("volume message sent");
    delay(50);
    CAN.sendMsgBuf(canidVol, 1, 8, stmpVolume2);
    Serial.println("volume2 message sent");
  }

  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);                       // send data per 100ms
}
