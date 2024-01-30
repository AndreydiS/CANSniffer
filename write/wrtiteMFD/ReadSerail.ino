byte incomingByte;
byte bitCount=0;
byte availableBytes = 0;
byte bufsize = 0;

unsigned char buf[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

void setup() {
  Serial.begin(115200);
  Serial.println("boot");
 // put your setup code here, to run once:

}

void loop() {
  availableBytes = Serial.available();
  if (availableBytes > 0) {
    if (availableBytes >=8) { //8 or more bytes received - reading
      Serial.print("bytes read by8 ");  
      Serial.println(availableBytes);  
      bufsize = 8;
    } else {
      delay(100);  
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
      bufsize = 0;
    }
  }
  //CAN.sendMsgBuf(canId, 1, bufsize, buf);

/*  
  if (Serial.available() > 0) {
      //incomingByte = Serial.read();

      Serial.readBytes(buf, 8);

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
      } else {
        bitCount++;
      }

      //Serial.println(incomingByte,HEX);
  }
  */
}
