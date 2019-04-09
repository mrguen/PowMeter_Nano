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


#ifndef POWERMETER_CONSOLE_h
#define POWERMETER_CONSOLE_h

#include <Wire.h>
#include <PowMeterNano_typedefs.h>
#include "LowPower.h"



/* Sends a command to PowMeter using I2C

*************************
Parameters:

	* I2C adress of host (byte)
	* I2C address of PowMeter (byte)
	* Command : one member of enum "Command"  (byte)
	* Data (int)

	If expecting data in return:
	* Response byte array pointer
	* Response expected size (byte)

A command is the association of a command byte and a data int
When data are expected in return they will be placed in the response byte array that has its size specified

*/

byte sendCmdToPowMeter (const byte, const byte, const byte, const int);

byte sendCmdToPowMeter (const byte, const byte, const byte, const int, byte *, const byte);

/*
*************************
Possible commands / datas

	SET_PERIOD / SLEEP_120MS, SLEEP_250MS, SLEEP_500MS, SLEEP_1S, SLEEP_2S, SLEEP_4S, SLEEP_8S

	SET_DISPLAY / AUTO, USB, VIN, BOTH

	SET_USB_SOFT_ALERT / false, true

	SET_VIN_SOFT_ALERT / false, true

	SET_USB_MIN_VOLTAGE / an int calculated as 100 * the voltage in volts. Example: 4.5V => 450

	SET_USB_MAX_VOLTAGE	/ an int calculated as 100 * the voltage in volts.

	SET_USB_MIN_CURRENT / an int calculated as the current in mA . Example: 1.2 A => 1200

	SET_USB_MAX_CURRENT	/ an int calculated as the current in mA

	SET_VIN_MIN_VOLTAGE / an int calculated as 100 * the voltage in volts.

	SET_VIN_MAX_VOLTAGE / an int calculated as 100 * the voltage in volts.

	SET_VIN_MIN_CURRENT	/ an int calculated as the current in mA.

	SET_VIN_MAX_CURRENT / an int calculated as the current in mA.

	SET_USB_HARD_ALERT / ALERT_OFF, UNDER_VOLTAGE, OVER_VOLTAGE, UNDER_CURRENT, OVER_CURRENT

	SET_VIN_HARD_ALERT / ALERT_OFF, UNDER_VOLTAGE, OVER_VOLTAGE, UNDER_CURRENT, OVER_CURRENT

	GET_USB_VOLTAGE / Nothing. Returns usb voltage as a float.

	GET_USB_CURRENT / Nothing. Returns usb current in mA as a float.

	GET_VIN_VOLTAGE / Nothing. Returns vin voltage as a float.

	GET_VIN_CURRENT / Nothing. Returns vin current in mA as a float.

  SET_POWER / POWER_OFF, POWER_ON, MEASUREMENT_HALT, OLED_OFF, OLED_ON
  		Turns off / on Powmeter but does not change the power path, so the host board and peripherals are still powered.
  		Measurement halt keeps the device active but does not run measurements
  		OLED can be turned off so the user might still query measurements but the Shield will consumme less.

  SET_I2C_ADDRESS / a byte to set a new I2C address for PowMeter. After the status is returned any other new I2C com must used the new address.

	SET_USB_CURRENT_OFFSET / an int to set the USB current offset. Expressed in uA. Example : data = -150 ->  USBCurrentOffset = -0.15 mA

	SET_VIN_CURRENT_OFFSET / an int to set the USB current offset. Expressed in uA.


*************************
Technical error status:

sendCmdToPowMeter(...) exits with a return code:

	0: no error, I2C communication completed including sending command and receiving back the requested data

	1: data too long to fit in transmit buffer (forwarded Wire.endTransmission() error)

	2: received NACK on transmit of address (forwarded Wire.endTransmission() error)

	3: received NACK on transmit of data (forwarded Wire.endTransmission() error)

	4: other error (forwarded Wire.endTransmission() error)

	10: I2C request to PowMeter did not return the expected number of bytes


Functional errors:

If sendCmdToPowMeter(...) is successful, data returned is either the data expected or one of these values
PERIOD_UNKNOWN, DISPLAY_UNKNOWN, ALERT_UNKNOWN, POWER_UNKNOWN, INVALID_I2C_ADDRESS, UNKNOWN_COMMAND

*/


#endif
