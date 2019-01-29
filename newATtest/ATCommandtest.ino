/**
 * Copyright (c) 2009 Andrew Rapp. All rights reserved.
 *
 * This file is part of XBee-Arduino.
 *
 * XBee-Arduino is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * XBee-Arduino is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with XBee-Arduino.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <XBee.h>
#include <SoftwareSerial.h>

/*
This example is for Series 1 (10C8 or later firmware) or Series 2 XBee radios
Sends a few AT command queries to the radio and checks the status response for success
This example uses the SoftSerial library to view the XBee communication.  I am using a 
Modern Device USB BUB board (http://moderndevice.com/connect) and viewing the output
with the Arduino Serial Monitor.
*/

// Define SoftSerial TX/RX pins
// Connect Arduino pin 8 to TX of usb-serial device
uint8_t ssRX = 2;
// Connect Arduino pin 9 to RX of usb-serial device
uint8_t ssTX = 3;
// Remember to connect all devices to a common Ground: XBee, Arduino and USB-Serial device
SoftwareSerial nss(ssRX, ssTX);

XBee xbee = XBee();

uint8_t AkCmd[] = {'+','+','+'};

uint8_t MyCmd[] = {'M','Y'};

uint8_t WrCmd[] = {'W','R'};

uint8_t AcCmd[] = {'A','C'};

uint8_t CnCmd[] = {'C','N'};

uint8_t Value[] = {'2','2'};

AtCommandRequest atRequest = AtCommandRequest();

AtCommandResponse atResponse = AtCommandResponse();

void setup() { 
  Serial.begin(9600);
  xbee.begin(Serial);
  nss.begin(9600);
  delay(5000);
}

void loop() {
  
  atRequest.setCommand(AkCmd);  
  sendAtCommand();
  

  atRequest.setCommand(MyCmd);  
  atRequest.setCommandValue(Value);  
  sendAtCommand();

  atRequest.setCommand(WrCmd);  
  sendAtCommand();

  atRequest.setCommand(AcCmd);  
  sendAtCommand();

  atRequest.setCommand(CnCmd);  
  sendAtCommand();
  
 
  while (1) {};
}

void sendAtCommand() {
  nss.println("Sending command to the XBee");

  // send the command
  xbee.send(atRequest);

  // wait up to 5 seconds for the status response
  if (xbee.readPacket(5000)) {
    // got a response!

    // should be an AT command response
    if (xbee.getResponse().getApiId() == AT_COMMAND_RESPONSE) {
      xbee.getResponse().getAtCommandResponse(atResponse);

      if (atResponse.isOk()) {
        nss.print("Command [");
        nss.print(atResponse.getCommand()[0]);
        nss.print(atResponse.getCommand()[1]);
        nss.println("] was successful!");

        if (atResponse.getValueLength() > 0) {
          nss.print("Command value length is ");
          nss.println(atResponse.getValueLength(), DEC);

          nss.print("Command value: ");
          
          for (int i = 0; i < atResponse.getValueLength(); i++) {
            nss.print(atResponse.getValue()[i], HEX);
            nss.print(" ");
          }

          nss.println("");
        }
      } 
      else {
        nss.print("Command return error code: ");
        nss.println(atResponse.getStatus(), HEX);
      }
    } else {
      nss.print("Expected AT response but got ");
      nss.print(xbee.getResponse().getApiId(), HEX);
    }   
  } else {
    // at command failed
    if (xbee.getResponse().isError()) {
      nss.print("Error reading packet.  Error code: ");  
      nss.println(xbee.getResponse().getErrorCode());
    } 
    else {
      nss.print("No response from radio");  
    }
  }
}
