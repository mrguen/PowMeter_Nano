
/*  PowMeterNano library
 *  Version: 1.0
 *  By Pandauino.com / Thierry GUENNOU
 *  April 2019
 *
 * It uses these libraries:
 *  - PowMeterNano_typedefs.h : defines a couple of enums used to pass I2C commands to the shield
 *  - INA226.h library from https://github.com/jarzebski/Arduino-INA226
 *  - "SSD1306Ascii.h", "SSD1306AsciiWire.h": these libraries are part of https://github.com/greiman/SSD1306Ascii
 *  - LowPower library from https://github.com/rocketscream/Low-Power
 *  - EEPROMex.h library from http://thijs.elenbaas.net/2012/07/extended-eeprom-library-for-arduino
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

#ifndef PowMeterNano_h
#define PowMeterNano_h

#include "INA226.h"
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include "LowPower.h"
#include <avr/power.h>
#include <avr/wdt.h>
#include <EEPROMex.h>

#include "PowMeterNano_typedefs.h"

const byte OLED_ADDRESS = 0x3C;													// Hardware set address of OLED module
const byte INA_USB_ADDRESS = 0x42;											// Hardware set address of INA226 on USB channel
const byte INA_VIN_ADDRESS = 0x40;											// Hardware set address of INA226 on VIN channel

const float rsenseUSB = 0.1;                   					// Value of USB current sense resistor
const float rsenseVIN = 0.01;                   				// Value of VIN current sense resistor
const int EEPROMbaseAddress = 0;              					// EEPROM start address where to store parameters.
const byte EEPROMInit = 8;                    					// A number that should be present at EEPROMbaseAddress

class PowMeterNanoClass
{

  public:

	static 	void begin();
	static 	void measure();
 	static float getUSBVoltage();
 	static float getUSBCurrent();
 	static float getVINVoltage();
 	static float getVINCurrent();

  private:

	static float VUSB;
	static float IUSB;
	static float VIn;
	static float IIn;

	static 	void launchMeasurement();
	static 	void getValues();

	static  byte nbDecimals(float);

  static void receiveEvent (int);
	static void requestEvent ();

	static void setAlert(INA226 &, float, uint16_t);

	static void readAllFromEEPROM();
	static void updateAllInEEPROM();

	static void displayContext();
	static void displayAlert(String, float);
	static void displayUSB();
	static void displayVIN();
	static void displayBoth();

	static byte POW_METER_I2C_ADDRESS;

  static float USBCurrentOffset;
  static float VINCurrentOffset;

	static byte readEEPROMinit;

	static byte command;									// Not volatiles since not modified outside the handler
	static int data;

	static volatile int period;
	static volatile byte displayMode;

  static volatile bool USBAlert;
  static volatile bool VINAlert;

  static float USBMinVoltage;						// Not volatiles since not modified outside the handler
  static float USBMaxVoltage;
  static int USBMinCurrent;
  static int USBMaxCurrent;

  static float VINMinVoltage;
  static float VINMaxVoltage;
  static int VINMinCurrent;
  static int VINMaxCurrent;

	static bool halted;
  static bool wasHalted;
  static bool alert;
  static bool formerAlert;

  static volatile bool oledON;				// All volatiles since updated in the handler and other methods.
	static volatile bool deviceOFF;
	static volatile bool mustUpdateDisplay;
	static volatile bool mustUpdateOledON;
	static volatile bool mustUpdateEEPROM;
	static volatile bool mustUpdateAlertHardUSB;
	static volatile bool mustUpdateAlertHardVIN;

	static float previousVUSB;
	static float previousIUSB;

	static float previousVIn;
	static float previousIIn;

	static byte display;
};

extern PowMeterNanoClass PowMeterNano;

#endif

