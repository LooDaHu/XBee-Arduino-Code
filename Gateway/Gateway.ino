#include <Printers.h>
#include <XBee.h>
#include <SoftwareSerial.h>

SoftwareSerial XBeeSerialPort(2, 3); //define a new software serial for zigbee module.
XBee node =XBee(); //define new xbee object.
Tx16Request tx = Tx16Request();
//Rx16Response rx16 = Rx16Response();
uint8_t Refuse = 'F';
uint8_t Accept = 'E';


uint8_t self_id = 'F';
uint8_t data = 0;
//uint8_t payload[] = { self_id, data ,'\n'};
uint8_t payload[] = { self_id };

void setup() {
  
  XBeeSerialPort.begin(9600);
  
  Serial.begin(9600);
  
  node.begin(XBeeSerialPort);
  
  delay(500);

}

void loop() {

//  node.readPacket(50);
//
//  if(node.getResponse().isAvailable())
//  {
//      if (node.getResponse().getApiId() == RX_16_RESPONSE) 
//      {
//        node.getResponse().getRx16Response(rx16);
//        //option = rx16.getOption();
//        data = rx16.getData(0);
//      } 
//  }
  
//  if(data != 0)
//  {
//     payload = { self_id, data };
//  }
//  else
//  {
//     payload = { self_id };
//  }
  
 tx = Tx16Request(0xAAA2, payload, sizeof(payload));

  node.send(tx);

  //data = 0;
  
  delay(1000);
}
