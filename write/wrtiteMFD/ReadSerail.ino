byte incomingByte;
byte bitCount=0;

unsigned char buf[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

void setup() {
  Serial.begin(115200);
  Serial.println("boot");
 // put your setup code here, to run once:

}

void loop() {
  if (Serial.available() > 0) {
      incomingByte = Serial.read();
      buf[bitCount] = incomingByte;
      if (bitCount >= 7) {
        Serial.print("Sending CAN Message: ");  
        for (int i = 0; i <= 7; i++) {
          Serial.print(buf[i],HEX);  
          Serial.print(",");  
        }
        Serial.println("");  

        //CAN.sendMsgBuf(canId, 1, 8, buf);
        bitCount = 0;
      }
      bitCount++;

      //Serial.println(incomingByte,HEX);

  }
}
