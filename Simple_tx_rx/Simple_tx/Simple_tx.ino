#include <Printers.h>
#include <XBee.h>
#include <SoftwareSerial.h>

SoftwareSerial XBeeSerialPort(2, 3); //define a new software serial for zigbee module.
XBee node = XBee(); //define new xbee object.
Tx16Request tx16;
Rx16Response rx16;
Tx64Request tx64;
uint8_t payload[6] = {0xAA, 0x00, 0x01, 0x02, 0x03, 0x04};

void setup() {
  XBeeSerialPort.begin(9600);

  Serial.begin(9600);

  node.begin(XBeeSerialPort);

  delay(500);
}

void loop() {

  tx16 = Tx16Request(0x0000, payload, sizeof(payload));//try to transmit the payload to the receiver, which 16-bit address is 0000.

  node.send(tx16);

  delay(5000);
  
}
