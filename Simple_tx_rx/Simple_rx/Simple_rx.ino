#include <Printers.h>
#include <XBee.h>
#include <SoftwareSerial.h>

SoftwareSerial XBeeSerialPort(2, 3); //define a new software serial for zigbee module.
XBee node = XBee(); //define new xbee object.
Tx16Request tx16;
Rx16Response rx16;
Rx64Response rx64;

void setup() {
  XBeeSerialPort.begin(9600);

  Serial.begin(9600);

  node.begin(XBeeSerialPort);

  delay(500);
}

void loop() {

  node.readPacket(5000); //keep listenning in a cycle of 5000ms
  if (node.getResponse().isAvailable())
  {
    //Serial.println(node.getResponse().getApiId(),HEX);
    //Serial.println(RX_64_RESPONSE,HEX);
    if (node.getResponse().getApiId() == RX_16_RESPONSE)
    {
      node.getResponse().getRx16Response(rx16);
      {
        Serial.println(rx16.getData(0));
        Serial.println(rx16.getData(1));
        Serial.println(rx16.getData(2));
        Serial.println(rx16.getData(3));
        Serial.println(rx16.getData(4));
      }
    }
  }
}
