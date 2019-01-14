#include <Printers.h>
#include <XBee.h>
#include <SoftwareSerial.h>
#include "Config.h"


SoftwareSerial XBeeSerialPort(2, 3); //define a new software serial for zigbee module.
XBee node = XBee(); //define new xbee object.
XBeeAddress64 super_node_address;
XBeeAddress64 response_address;
TxStatusResponse txStatus = TxStatusResponse();
Tx16Request tx16;
Rx16Response rx16;
Tx64Request tx64;
Rx64Response rx64;

void setup() {

  XBeeSerialPort.begin(9600);

  Serial.begin(9600);

  node.begin(XBeeSerialPort);

  delay(500);

}

void loop()
{
  if (no_recieve_count == 5)
  {
    current_capacity = 2;
    no_recieve_count = 0;
  }
  if (discounted_count == 50)
  {
    current_capacity = 1;
    discounted_count = 0;
  }
  if (level == 0)
  {
    initalize();
  }
  if (level == 1)
  {
    listen_from_response();
    no_recieve_count++;
  }
  if (level == 2 || current_capacity == 2)
  {
    send_id();
  }
}

void initalize()
{
  uint8_t request_msg[5] = {ID_REQUEST_HEADER, SL_seg1, SL_seg2, SL_seg3, SL_seg4};

  access_gate(request_msg);

  delay(50);

  node.readPacket(5000);

  if (node.getResponse().isAvailable()) {
    Serial.println(node.getResponse().getApiId(), HEX);
    if (node.getResponse().getApiId() == RX_64_RESPONSE)
    {
      node.getResponse().getRx64Response(rx64);

      if (rx64.getData(0) == ACCEPT_REQUEST_HEADER)
      {
        level = 1;

        super_node_address = XBeeAddress64(SH, combine_SL(rx64.getData(1), rx64.getData(2), rx64.getData(3), rx64.getData(4)));

        setMYaddress(level1);

        Serial.println(level);

      }
      else
      {
//        while (level == 0)
//        {
//          access_level1(request_msg);
            
//          node.readPacket(5000);
//          if (node.getResponse().isAvailable())
//          {
//            if (node.getResponse().getApiId() == RX_64_RESPONSE)
//            {
//              node.getResponse().getRx64Response(rx64);
//
//              if (rx64.getData(0) == ACCEPT_REQUEST_HEADER)
//              {
//                level = 2;
//
//                super_node_address = XBeeAddress64(SH, combine_SL(rx64.getData(1), rx64.getData(2), rx64.getData(3), rx64.getData(4)));
//
//                setMYaddress(level2);
//
//              }
//            }
//          }
//          delay(5000);
//        }
      }
    }
  }
}


void access_gate(uint8_t *level_request_msg)
{
  tx16 = Tx16Request(0x0000, level_request_msg, 5);

  tx16.setFrameId(0x00);

  node.send(tx16);
}

void access_level1(uint8_t *level_request_msg)
{
  tx16 = Tx16Request(0x0001, level_request_msg, 5);

  tx16.setFrameId(0x00);

  node.send(tx16);
}

uint32_t combine_SL(uint8_t sl1, uint8_t sl2, uint8_t sl3, uint8_t sl4)
{
  uint32_t temp_SL;
  temp_SL |= sl1 << 8 << 8 << 8;
  temp_SL |= sl2 << 8 << 8;
  temp_SL |= sl3 << 8;
  temp_SL |= sl4;
  return temp_SL;
}

void setMYaddress(uint8_t *value)
{
  uint8_t cmd1[] = {'M', 'Y'};
  uint8_t cmd2[] = {'W', 'R'};
  uint8_t cmd3[] = {'A', 'C'};
  AtCommandRequest atRequest1 = AtCommandRequest(cmd1, value, sizeof(value));
  AtCommandRequest atRequest2 = AtCommandRequest(cmd2);
  AtCommandRequest atRequest3 = AtCommandRequest(cmd3);
  node.send(atRequest1);
  node.send(atRequest2);
  node.send(atRequest3);
}

void listen_from_response()
{
  node.readPacket(1000);

  if (node.getResponse().isAvailable())
  {
    if (node.getResponse().getApiId() == RX_16_RESPONSE)
    {
      node.getResponse().getRx16Response(rx16);

      uint8_t header = rx16.getData(0);

      if (header == ID_REQUEST_HEADER)
      {
        if (current_capacity == 0)
        {
          response_address = XBeeAddress64(SH, combine_SL(rx16.getData(1), rx16.getData(2), rx16.getData(3), rx16.getData(4)));
          uint8_t level_payload[5] = { REFUSE_REQUEST_HEADER, 0x00, 0x00, 0x00, 0x00 };
          tx64 = Tx64Request(response_address, level_payload, sizeof(level_payload));
          tx64.setFrameId(0x00);
          node.send(tx64);
        }
        else
        {
          response_address = XBeeAddress64(SH, combine_SL(rx16.getData(1), rx16.getData(2), rx16.getData(3), rx16.getData(4)));
          uint8_t level_payload[5] = { ACCEPT_REQUEST_HEADER, SL_seg1, SL_seg2, SL_seg3, SL_seg4 };
          tx64 = Tx64Request(response_address, level_payload, sizeof(level_payload));
          tx64.setFrameId(0x00);
          node.send(tx64);
          current_capacity = current_capacity - 1;
        }
      }
      if (header == MSG_HEADER)
      {
        if ( temp_check_identifier == rx64.getData(1))
        {
          discounted_count++;
        }
        else
        {
          temp_check_identifier = rx64.getData(1);
          discounted_count = 0;
        }
        uint8_t msg_payload[5] = { MSG_HEADER, ID_IDENTIFIER, 0x01, rx64.getData(1), rx64.getData(2) };
        tx64 = Tx64Request(super_node_address, msg_payload, sizeof(msg_payload));
        tx64.setFrameId(0x00);
        node.send(tx64);
      }
    }
  }
}

void send_id()
{
  uint8_t msg_payload[5] = { MSG_HEADER, ID_IDENTIFIER, level };
  tx64 = Tx64Request(super_node_address, msg_payload, sizeof(msg_payload));
  tx64.setFrameId(0x00);
  node.send(tx64);
  delay(5000);
}
