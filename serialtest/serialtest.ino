byte incomingByte =0;

void setup() {
  Serial.begin(9600);
    Serial.println("CAN ok. CMD send: 61 74 20 <canid_4b> <payloadlen_1b> <payload>");
    Serial.println("CAN ok. CMD sniff: 61 73 20 <canid_4b>");
}

void loop() {  
  if (Serial.available() > 0) { //populating buffer
    incomingByte = Serial.read();
    Serial.println(incomingByte,HEX);
  }
}
