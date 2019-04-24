/*  PowMeterConsole library
 *  Version: 1.0
 *  By Pandauino.com / Thierry GUENNOU
 *  April 2019
 *
 *  This software is licensed under https://creativecommons.org/licenses/by-nc-sa/4.0/
 *
 *  This code is based on Arduino code and is provided with the same warning that:
 *  THIS SOFTWARE IS PROVIDED TO YOU "AS IS" AND WE MAKE NO EXPRESS OR IMPLIED WARRANTIES WHATSOEVER WITH RESPECT TO ITS FUNCTIONALITY, OPERABILITY, OR USE,
 *  INCLUDING, WITHOUT LIMITATION, ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR INFRINGEMENT.
 *  WE EXPRESSLY DISCLAIM ANY LIABILITY WHATSOEVER FOR ANY DIRECT, INDIRECT, CONSEQUENTIAL, INCIDENTAL OR SPECIAL DAMAGES, INCLUDING, WITHOUT LIMITATION, LOST REVENUES,
 *  LOST PROFITS, LOSSES RESULTING FROM BUSINESS INTERRUPTION OR LOSS OF DATA, REGARDLESS OF THE FORM OF ACTION OR LEGAL THEORY UNDER WHICH THE LIABILITY MAY BE ASSERTED,
 *  EVEN IF ADVISED OF THE POSSIBILITY OR LIKELIHOOD OF SUCH DAMAGES.
 */

#include <PowMeterNano_console.h>

byte sendCmdToPowMeter (const byte myAddress, const byte powMeter_address, const byte cmd, const int data){
	byte dummy[1];

	return sendCmdToPowMeter( myAddress, powMeter_address, cmd, data, dummy, 1);
}

byte sendCmdToPowMeter (const byte myAddress, const byte powMeter_address, const byte cmd, const int data, byte * response, const byte responseSize){

  byte result = 0;

  Wire.beginTransmission (powMeter_address);
  Wire.write (cmd);
 	Wire.write ((byte) (data & 0xFF));
 	Wire.write ((byte) ((data >> 8) & 0xFF));
  result = Wire.endTransmission();

  if (result > 0) {
  	Serial.print(F("Wire.endTransmission error: "));
  	Serial.println(result);
  	// We re-start I2C whenever a fatal error happened
  	Wire.end();
  	delay(1);
  	Wire.begin(myAddress);
  	return result;
  }

	delay(100); // Keep it to a minimum of 100 to ms give some time for the shield to treat the event

  int counter = 0;

  Wire.requestFrom (powMeter_address, responseSize);  // will not return before "completion"

  while (Wire.available()) {
    response [counter] = Wire.read ();
// 		DEBUG
//    Serial.println(response [counter]);
		counter++;
  }

	return result;

/*
  if (counter == responseSize) {
    return 0;
  } else {
	Serial.print(F("I2C request to PowMeter failed. Wrong response size = "));
	Serial.print(counter);
	Serial.println(" bytes");
	// We re-start I2C whenever a fatal error happened
  	Wire.end();
  	delay(1);
  	Wire.begin(myAddress);
    return 10;
  }
*/
}
