
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

#include "PowMeterNano.h"

// ****************************************************************
// DECLARATIONS
// ****************************************************************

	// DEBUG
	//volatile bool PowMeterNanoClass::mustDisplayIUSB = false;
	//volatile float PowMeterNanoClass::tampon = 0.0;


int i = 0;

byte PowMeterNanoClass::POW_METER_I2C_ADDRESS = 0x25; 						// Shield's MCU I2C address

byte PowMeterNanoClass::readEEPROMinit = 0;                      	// to store the value read from EEPROM at address eeAddress

// These offsets can be modified by an I2C command
float PowMeterNanoClass::USBCurrentOffset = 0.0;									// USB current offset @ 5V. INA226 has a typical offset of +/- 0.0025 mV that gives +/- 0.025 mA error on a 0.1 Ohm current sense resistor
float PowMeterNanoClass::VINCurrentOffset = 0.0;									// VIn current offset @ 12V

float PowMeterNanoClass::VUSB = 0.0;                              // USB Input voltage in V
float PowMeterNanoClass::IUSB = 0.0;                           		// USB Measured current in mA

float PowMeterNanoClass::VIn = 0.0;                              	// VIN Input voltage in V
float PowMeterNanoClass::IIn = 0.0;                           		// VIN Measured current in mA

float PowMeterNanoClass::previousVUSB = 0.0;											// Stores previous VUSB value;
float PowMeterNanoClass::previousIUSB = 0.0;											// Stores previous IUSB value;
float PowMeterNanoClass::previousVIn = 0.0;												// Stores previous VIn value;
float PowMeterNanoClass::previousIIn = 0.0;												// Stores previous IIn value;

byte PowMeterNanoClass::display = BOTH;														// Defines the display layout

byte PowMeterNanoClass::command = 0;                     					// Command received by I2C
int PowMeterNanoClass::data = 0;                         					// Data associated to the command, received by I2C.

//////////////////////////////////////
// All these parameters can be modified by an I2C command
volatile int PowMeterNanoClass::period =  SLEEP_1S;               // Measurement period defined by LowPower library's typeDef
volatile byte PowMeterNanoClass::displayMode =  AUTO;             // Defines the display mode

volatile bool PowMeterNanoClass::USBAlert = false ;              	// Using the LCD to display USB alerts or not
volatile bool PowMeterNanoClass::VINAlert = false ;              	// Using the LCD to display VIN alerts or not

float PowMeterNanoClass::USBMinVoltage = -1;											// USB minimum voltage that will trigger an alert if in alert mode
float PowMeterNanoClass::USBMaxVoltage = 27;											// USB maximum voltage that will trigger an alert if in alert mode
int PowMeterNanoClass::USBMinCurrent = -1;												// USB minimum current that will trigger an alert if in alert mode
int PowMeterNanoClass::USBMaxCurrent = 3000;											// USB maximum current that will trigger an alert if in alert mode

float PowMeterNanoClass::VINMinVoltage = -1;											// VIN minimum voltage that will trigger an alert if in alert mode
float PowMeterNanoClass::VINMaxVoltage = 27;											// VIN maximum voltage that will trigger an alert if in alert mode
int PowMeterNanoClass::VINMinCurrent = -1;												// VIN minimum current that will trigger an alert if in alert mode
int PowMeterNanoClass::VINMaxCurrent = 3000;											// VIN maximum current that will trigger an alert if in alert mode

volatile bool PowMeterNanoClass::oledON =  true;			      			// OLED on / off
volatile bool PowMeterNanoClass::deviceOFF = false;								// When putting the device into sleep

bool PowMeterNanoClass::halted = false ;                        	// When "1" the program runs but measurements are suspended
bool PowMeterNanoClass::wasHalted = false ;                       // When "1" measurements were halted during the previous loop

//////////////////////////////////////

// These are flags used to run a procedure after calling the I2C event request handler
volatile bool PowMeterNanoClass::mustUpdateDisplay = false;       // Boolean to update display contextual infos
volatile bool PowMeterNanoClass::mustUpdateOledON = false;       	// Boolean to change OLED on / off state after executeCommand was called to change power
volatile bool PowMeterNanoClass::mustUpdateEEPROM = false;       	// Boolean to update EEPROM after executeCommand was called
volatile bool PowMeterNanoClass::mustUpdateAlertHardUSB = false;  // Boolean to update hardware alert after executeCommand was called
volatile bool PowMeterNanoClass::mustUpdateAlertHardVIN = false;  // Boolean to update hardware alert after executeCommand was called

bool PowMeterNanoClass::alert = false;														// An alert is triggered/not
bool PowMeterNanoClass::formerAlert = false;											// An alert was triggered/not in the previous loop

INA226 INA226_USB;																								// INA226 USB instance
INA226 INA226_VIN;																								// INA226 VIN instance
SSD1306AsciiWire oled;																						// OLED instance

// ****************************************************************
// ACCESSOR FUNCTIONS
// ****************************************************************

float PowMeterNanoClass::getUSBVoltage(){ return VUSB;}
float PowMeterNanoClass::getUSBCurrent(){ return IUSB;}
float PowMeterNanoClass::getVINVoltage(){ return VIn;}
float PowMeterNanoClass::getVINCurrent(){ return IIn;}


// ****************************************************************
// INA226 ALERT FUNCTION
// ****************************************************************

// ****************************************************************
// Sets an INA226 hardware alert.
void PowMeterNanoClass::setAlert(INA226 &ic, float value, uint16_t alertWord) {

	ic.setMaskEnable(alertWord);
	if ((alertWord == INA226_BIT_BUL) || (alertWord == INA226_BIT_BOL))  ic.setBusVoltageLimit(value);
	if ((alertWord == INA226_BIT_SUL) || (alertWord == INA226_BIT_SOL))  ic.setShuntVoltageLimit(value);

}


// ****************************************************************
// I2C / COMMAND FUNCTIONS
// ****************************************************************

// I2C is implemented in multi master mode here.
// I.e. the MCU has an I2C address and will act as master toward INA226 and OLED parts
// and as a slave when queried from the host Nano board.
//
// The shield configuration can be modified by a console program on the host board
// that will send through I2C
// - one commande byte
// - followed by some bytes of data
//
// The Shield will answer with some data with variable length.
// If there is no data to be returned, the Shield will answer with a "0" for succesful command or an error code enumerated as "error" enum type.
//
// If needed the MCU address can be modified with the command SET_I2C_ADDRESS

// ****************************************************************
// Receives a command as a slave. A command is made of a command byte and a couple of data bytes
void PowMeterNanoClass::receiveEvent (int howMany)
 {
 	byte d1,d2;

 // we are expecting 3 bytes, so check we got them
  if (howMany == 3) {
    command = Wire.read ();
    d1 = Wire.read ();
    d2 = Wire.read ();
 		data = d1 | (d2 << 8);
   }

  // throw away any garbage
  while (Wire.available () > 0)  Wire.read ();

}

// ****************************************************************
// Receives a request from the Master: effectively executes the command and sends back one byte, that is the error status
void PowMeterNanoClass::requestEvent () {

	mustUpdateOledON = false;
	mustUpdateEEPROM = false;
	mustUpdateDisplay = false;

	mustUpdateAlertHardUSB = false;
	mustUpdateAlertHardVIN = false;


  switch(command) {

    case SET_PERIOD:

      if ((data == SLEEP_120MS) || (data == SLEEP_250MS) || (data == SLEEP_500MS) || (data == SLEEP_1S) || (data == SLEEP_2S) || (data == SLEEP_4S) || (data == SLEEP_8S)) {
        period = data;
				mustUpdateEEPROM = true;
        Wire.write(0);
      } else {
        Wire.write(PERIOD_UNKNOWN);
      }
      break;

    case SET_DISPLAY:

      if ((data == AUTO) || (data == USB) || (data == VIN) || (data == BOTH)) {
				displayMode = data;
				mustUpdateDisplay = true;
				mustUpdateEEPROM = true;
        Wire.write(0);
      }
      else {
        Wire.write(DISPLAY_UNKNOWN);
      }
      break;

    case SET_USB_SOFT_ALERT:
			USBAlert = (bool)data;
		  Wire.write(0);
			break;

    case SET_VIN_SOFT_ALERT:
			VINAlert = (bool)data;
	    Wire.write(0);
	    break;

    case SET_USB_MIN_VOLTAGE:

			USBMinVoltage = (float)data / 100.0; // expressed as LSB = 0.1 V, (max 25.5 volts)
		  Wire.write(0);
			mustUpdateEEPROM = true;
      break;

    case SET_USB_MAX_VOLTAGE:

			USBMaxVoltage = (float)data / 100.0;
		  Wire.write(0);
			mustUpdateEEPROM = true;
      break;

    case SET_USB_MIN_CURRENT:

			USBMinCurrent = data;
		  Wire.write(0);
			mustUpdateEEPROM = true;
      break;

    case SET_USB_MAX_CURRENT:

			USBMaxCurrent = data;
		  Wire.write(0);
			mustUpdateEEPROM = true;
      break;

    case SET_VIN_MIN_VOLTAGE:

			VINMinVoltage = (float)data / 100.0;
		  Wire.write(0);
			mustUpdateEEPROM = true;
      break;

    case SET_VIN_MAX_VOLTAGE:

			VINMaxVoltage = (float)data / 100.0;
		  Wire.write(0);
			mustUpdateEEPROM = true;
      break;

    case SET_VIN_MIN_CURRENT:

			VINMinCurrent = data;
		  Wire.write(0);
			mustUpdateEEPROM = true;
      break;

    case SET_VIN_MAX_CURRENT:

			VINMaxCurrent = data;
		  Wire.write(0);
			mustUpdateEEPROM = true;
      break;

    case SET_USB_HARD_ALERT:

    	if ((data == ALERT_OFF) ||  (data == UNDER_VOLTAGE) || (data == OVER_VOLTAGE) || (data == UNDER_CURRENT) || (data == OVER_CURRENT))
    	{
				mustUpdateAlertHardUSB = true;
		    Wire.write(0);
		  }
		  else {
		  	Wire.write(ALERT_UNKNOWN);
      }
	    break;

    case SET_VIN_HARD_ALERT:

    	if ((data == ALERT_OFF) ||  (data == UNDER_VOLTAGE) || (data == OVER_VOLTAGE) || (data == UNDER_CURRENT) || (data == OVER_CURRENT))
    	{
				mustUpdateAlertHardVIN = true;
		    Wire.write(0);
		  }
		  else {
		  	Wire.write(ALERT_UNKNOWN);
      }
	    break;

    case GET_USB_VOLTAGE:
    	Wire.write((byte *) &VUSB, sizeof (VUSB));
      break;

    case GET_USB_CURRENT:

    	Wire.write((byte *) &IUSB, sizeof (IUSB));

      break;

    case GET_VIN_VOLTAGE:
    	Wire.write((byte *) &VIn, sizeof (VIn));
      break;

    case GET_VIN_CURRENT:
    	Wire.write((byte *) &IIn, sizeof (IIn));
      break;

    case SET_POWER:

      if (data == POWER_OFF) {
        deviceOFF = true;
        Wire.write(0);

      } else if (data == POWER_ON) {
				oledON = true;
				halted = false;
      	mustUpdateOledON = true;
        Wire.write(0);

      } else if (data == MEASUREMENT_HALT) {
        halted = true;
        Wire.write(0);

      } else if (data == OLED_OFF) {
				oledON = false;
      	mustUpdateOledON = true;
        Wire.write(0);

      } else if (data == OLED_ON) {
				oledON = true;
      	mustUpdateOledON = true;
        Wire.write(0);

      } else {
        Wire.write(POWER_UNKNOWN);
      }
      break;


    case SET_I2C_ADDRESS:

      // Setting an I2C address, valid range: 8 - 119
      if ((7 < data) and (data < 120)) {
        POW_METER_I2C_ADDRESS = data;
        Wire.write(0);
        Wire.begin(POW_METER_I2C_ADDRESS);
   			mustUpdateEEPROM = true;
      }
      else {
        Wire.write(INVALID_I2C_ADDRESS);
      }
      break;

    case SET_USB_CURRENT_OFFSET:

			USBCurrentOffset = data / 1000 ; // Expressed in uA. Example : data = -150 ->  USBCurrentOffset = -0.15 mA
		  Wire.write(0);
			mustUpdateEEPROM = true;
      break;

    case SET_VIN_CURRENT_OFFSET:

			VINCurrentOffset = data / 1000 ;
		  Wire.write(0);
			mustUpdateEEPROM = true;
      break;

    default:
      Wire.write(UNKNOWN_COMMAND);
      break;

  }


}


// ****************************************************************
// EEPROM FUNCTIONS
// ****************************************************************

// ****************************************************************
// Reads parameters in EEPROM and writes defaults to it if empty
void PowMeterNanoClass::readAllFromEEPROM() {

  noInterrupts(); // To avoid datra being corrupted by I2C interrupt

  readEEPROMinit = EEPROM.readByte(EEPROMbaseAddress);

  if (readEEPROMinit == EEPROMInit) {

    period = EEPROM.readInt(EEPROMbaseAddress+1);
    displayMode =  EEPROM.readByte(EEPROMbaseAddress+5);

    USBAlert = (bool)EEPROM.readByte(EEPROMbaseAddress+6);
    VINAlert = (bool)EEPROM.readByte(EEPROMbaseAddress+7);

    USBMinVoltage = EEPROM.readFloat(EEPROMbaseAddress+8);
    USBMaxVoltage = EEPROM.readFloat(EEPROMbaseAddress+12);
    USBMinCurrent = EEPROM.readInt(EEPROMbaseAddress+16);
    USBMaxCurrent = EEPROM.readInt(EEPROMbaseAddress+20);

    VINMinVoltage = EEPROM.readFloat(EEPROMbaseAddress+24);
    VINMaxVoltage = EEPROM.readFloat(EEPROMbaseAddress+28);
    VINMinCurrent = EEPROM.readInt(EEPROMbaseAddress+33);
    VINMaxCurrent = EEPROM.readInt(EEPROMbaseAddress+36);

		POW_METER_I2C_ADDRESS =  EEPROM.readByte(EEPROMbaseAddress+40);

		USBCurrentOffset = EEPROM.readFloat(EEPROMbaseAddress+41);
		VINCurrentOffset = EEPROM.readFloat(EEPROMbaseAddress+45);

  } else {
    updateAllInEEPROM();
  }

	interrupts();

}

// ****************************************************************
// Writes all parameters to EEPROM
void PowMeterNanoClass::updateAllInEEPROM() {

 	noInterrupts(); // To avoid datra being corrupted by I2C interrupt

  EEPROM.updateByte(EEPROMbaseAddress,EEPROMInit);

  EEPROM.updateInt(EEPROMbaseAddress+1,period);
  EEPROM.updateByte(EEPROMbaseAddress+5, displayMode);

  EEPROM.updateByte(EEPROMbaseAddress+6, USBAlert);
  EEPROM.updateByte(EEPROMbaseAddress+7, VINAlert);

  EEPROM.updateFloat(EEPROMbaseAddress+8, USBMinVoltage);
  EEPROM.updateFloat(EEPROMbaseAddress+12, USBMaxVoltage);
  EEPROM.updateInt(EEPROMbaseAddress+16, USBMinCurrent);
  EEPROM.updateInt(EEPROMbaseAddress+20, USBMaxCurrent);

  EEPROM.updateFloat(EEPROMbaseAddress+24, VINMinVoltage);
  EEPROM.updateFloat(EEPROMbaseAddress+28, VINMaxVoltage);
  EEPROM.updateInt(EEPROMbaseAddress+32, VINMinCurrent);
  EEPROM.updateInt(EEPROMbaseAddress+36, VINMaxCurrent);

  EEPROM.updateByte(EEPROMbaseAddress+40, POW_METER_I2C_ADDRESS);

  EEPROM.updateFloat(EEPROMbaseAddress+41, USBCurrentOffset);
  EEPROM.updateFloat(EEPROMbaseAddress+45, VINCurrentOffset);

	interrupts();
}


// ****************************************************************
// FUNDAMENTAL FUNCTIONS
// ****************************************************************

// ****************************************************************
// Computes the number of decimals to display on the OLED (attended values between 0.01 and 10000)
byte PowMeterNanoClass::nbDecimals(float value) {

  if (value < 10.0) {
    return 2;
  } else if (value < 100.0) {
    return 1;
  }

	return 0;

}

// ****************************************************************
// Request current and voltage values from INA chips
void PowMeterNanoClass::getValues() {

  // Flush any garbage (might be from I2C commands sent from the host).
	// If you get apparently wrong values this can come from I2C collision.
	// Collisions are not managed. The following flush is to have at least a clean buffer.
	// It might be necessary to tweak it if you have more I2C devices or for any other reason.
	while (Wire.available () > 0)  Wire.read ();
  delay(10); // keep it

  IUSB = 1000 * INA226_USB.readShuntVoltage() / rsenseUSB + USBCurrentOffset;
  IIn = 1000 * INA226_VIN.readShuntVoltage() / rsenseVIN + VINCurrentOffset;

  VUSB = INA226_USB.readBusVoltage();
  VIn = INA226_VIN.readBusVoltage();

	// Discarding meaningless values - negative values due to wrong calibration
  if (IUSB < 0.025){
    IUSB = 0.0;
  }

  if (IIn < 0.25) {
		IIn = 0.0;
  }

}

// ****************************************************************
// Triggers a measurement since we are asking for on demand measurements
// These parameters can be changed as you like. See INA226 library's and datasheet documents.
void PowMeterNanoClass::launchMeasurement() {

	// Best compromise
  INA226_USB.configure(INA226_AVERAGES_16, INA226_BUS_CONV_TIME_1100US, INA226_SHUNT_CONV_TIME_1100US, INA226_MODE_SHUNT_BUS_TRIG);
  INA226_VIN.configure(INA226_AVERAGES_16, INA226_BUS_CONV_TIME_1100US, INA226_SHUNT_CONV_TIME_1100US, INA226_MODE_SHUNT_BUS_TRIG);

}

// ****************************************************************
// Displays contextual information
// depending on "display" parameter value
void PowMeterNanoClass::displayContext() {

	oled.clear();

	if (display == USB) {
  	oled.setCursor(0,0);
		oled.print("U");
		oled.setCursor(0,1);
		oled.print("S");
		oled.setCursor(0,2);
		oled.print("B");
	}
	else if (display == VIN) {
		oled.setCursor(0,0);
		oled.print("V");
		oled.setCursor(0,1);
		oled.print("I");
		oled.setCursor(0,2);
		oled.print("N");
	}
	else {
		oled.setCursor(0,0);
		oled.print("=USB=");
		oled.setCursor(32,0);
		oled.print("=VIN=");
	}

	delay(10); // to let I2C finish
}


// ****************************************************************
// Displays an alert
// on white background
// with bigger font
void PowMeterNanoClass::displayAlert(String msgLine1, float number ) {

  if (!formerAlert) {
		oled.ssd1306WriteCmd(SSD1306_INVERTDISPLAY);
		oled.clear();
		oled.set2X();
		formerAlert = true;
	}

	oled.setCursor(4,0);
	oled.println(msgLine1);
	oled.setCursor(4,2);
	oled.println(number,nbDecimals(number));

}

// ****************************************************************
// Displays only USB voltage and current values
// with bigger font
void PowMeterNanoClass::displayUSB() {

	oled.set2X();
	oled.setCursor(10,0);
  oled.print(VUSB, nbDecimals(VUSB));
	oled.setCursor(10,2);
  oled.print(IUSB, nbDecimals(IUSB));
  oled.println("  ");
	oled.set1X();

}

// ****************************************************************
// Displays only VIN voltage and current values
// with bigger font
void PowMeterNanoClass::displayVIN() {

	oled.set2X();
	oled.setCursor(10,0);
  oled.print(VIn, nbDecimals(VIn));
	oled.setCursor(10,2);
  oled.print(IIn, nbDecimals(IIn));
  oled.print("  ");
	oled.set1X();

}


// ****************************************************************
// Default display of both USB and VIN values on one screen
void PowMeterNanoClass::displayBoth() {

  oled.setCursor(0,2);
  oled.print(VUSB, nbDecimals(VUSB));
  oled.setCursor(0,3);
  oled.print(IUSB, nbDecimals(IUSB));
  oled.print("  ");

  oled.setCursor(32,2);
  oled.print(VIn, nbDecimals(VIn));
  oled.setCursor(32,3);
  oled.print(IIn, nbDecimals(IIn));
  oled.print("  ");

}

// ****************************************************************
// SETUP FUNCTION
// ****************************************************************
void PowMeterNanoClass::begin() {

  bool reset = false;

	// Keep this sleep period used in some regular cases to avoid multiple partial execution of begin()
	// For example when powering the Nano + shield on the Nano USB connector, the USB com establishing
	// triggers a couple of resets that will be masked by this sleep period.
	LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_ON);

	// Saving as much power as possible
	power_adc_disable();
	power_usart0_disable();
	power_spi_disable();
	power_timer1_disable();
	power_timer2_disable();

	// All unused pins stick to vcc
	for (int i = 0; i <= 10; i++){
		pinMode(i, INPUT_PULLUP);
	}

	for (int i = 14; i <= 17; i++){
		pinMode(i, INPUT_PULLUP);
	}

	// Hardware reset: If PC2 is low the device must be reset to default parameters.
	if (digitalRead(16) == LOW) {
		updateAllInEEPROM();
		reset = true;
	}
	else {
		// Loads saved parameters
	  readAllFromEEPROM();
	}

  // These attachements of handlers must be specified before calling begin(address)
  // Because begin(address) does the attachment

  Wire.onReceive (receiveEvent);  		// interrupt handler for incoming command
  Wire.onRequest (requestEvent);  		// interrupt handler for data request

  Wire.begin(POW_METER_I2C_ADDRESS);	// Begins in multi master mode
  //Wire.setClock(400000L);						// Could try in I2C high speed to increase OLED frame rate but seemed to generate too many faults.

	// INA226 INIT
	INA226_USB.begin(INA_USB_ADDRESS);
	INA226_VIN.begin(INA_VIN_ADDRESS);

	launchMeasurement();

	// Useless when we query the shunt voltage - would be used if querying values computed by the INA chips
	// INA226_USB.calibrate(rsenseUSB, 0.5);
	// INA226_VIN.calibrate(rsenseVIN, 8);

	oled.begin(&SSD1306_64x32, OLED_ADDRESS);
	oled.setFont(lcd5x7);

	if (reset) {
		oled.setCursor(0,0);
		oled.println("HARD RST");
		delay(2000);
	}

  displayContext();

}

// ****************************************************************
// LOOP FUNCTION
// ****************************************************************
// The method to call in the program loop to query, compute and display values
void PowMeterNanoClass::measure() {

	alert = false;

	if (mustUpdateOledON) {

		if (oledON) oled.ssd1306WriteCmd(SSD1306_DISPLAYON);
		else oled.ssd1306WriteCmd(SSD1306_DISPLAYOFF);

  	mustUpdateOledON = false;

  }

	if (mustUpdateAlertHardUSB) {

		switch(data) {
			case ALERT_OFF: 		setAlert(INA226_USB, 0, 0); break;
			case UNDER_VOLTAGE: setAlert(INA226_USB, USBMinVoltage, INA226_BIT_BUL); break;
			case OVER_VOLTAGE: 	setAlert(INA226_USB, USBMaxVoltage, INA226_BIT_BOL); break;
			case UNDER_CURRENT: setAlert(INA226_USB, USBMinCurrent * rsenseUSB * 0.001, INA226_BIT_SUL); break;
			case OVER_CURRENT: 	setAlert(INA226_USB, USBMaxCurrent * rsenseUSB * 0.001, INA226_BIT_SOL);
		}
 	  mustUpdateAlertHardUSB = false;
	}

  if (mustUpdateAlertHardVIN) {

		switch(data) {
			case ALERT_OFF: 		setAlert(INA226_VIN, 0, 0); break;
			case UNDER_VOLTAGE: setAlert(INA226_VIN, VINMinVoltage, INA226_BIT_BUL); break;
			case OVER_VOLTAGE: 	setAlert(INA226_VIN, VINMaxVoltage, INA226_BIT_BOL); break;
			case UNDER_CURRENT: setAlert(INA226_VIN, VINMinCurrent * rsenseVIN * 0.001, INA226_BIT_SUL); break;
			case OVER_CURRENT: 	setAlert(INA226_VIN, VINMaxCurrent * rsenseVIN * 0.001, INA226_BIT_SOL);
		}
    mustUpdateAlertHardVIN = false;
	}

  if (mustUpdateEEPROM) {
    updateAllInEEPROM();
    mustUpdateEEPROM = false;
  }

  if (halted) { // Does not measure anything while in HALT mode
		if (wasHalted == false) {wasHalted = true;}
	} else {

		if (wasHalted == true) {mustUpdateDisplay = true; wasHalted = false;}

	  getValues();

	  // If Alert mode active
	  if ((USBAlert) || (VINAlert)) {

			if ((VUSB < USBMinVoltage) || (VUSB > USBMaxVoltage)) {
				displayAlert("USB V !", VUSB);
				alert = true;
			}

			if ((IUSB < USBMinCurrent) || (IUSB > USBMaxCurrent)) {
				displayAlert("USB I !", IUSB);
				alert = true;
			}

			if ((VIn < VINMinVoltage) || (VIn > VINMaxVoltage)) {
				displayAlert("VIN V !", VIn);
				alert = true;
			}

			if ((IIn < VINMinCurrent) || (IIn > VINMaxCurrent)) {
				displayAlert("VIN I !", IIn);
				alert = true;
			}

		} // Alert


		if (!alert) {

  		if (formerAlert) {
				oled.ssd1306WriteCmd(SSD1306_NORMALDISPLAY);
				oled.clear();
				oled.set1X();
				mustUpdateDisplay = true;
				formerAlert = false;
			}

			if (displayMode == AUTO) {
				if ((IUSB > 0.02) and (IIn > 0.2)) {
					if (display != BOTH) mustUpdateDisplay = true;
					display = BOTH;
				}
				else if (IIn > 0.2) {
					if (display != VIN) mustUpdateDisplay = true;
					display = VIN;
				}
				else {
					if (display != USB) mustUpdateDisplay = true;
					display = USB;
				}
			}
			else display = displayMode;

  		if (mustUpdateDisplay) {
				displayContext();
				mustUpdateDisplay = false;
			}

			if (display == USB) displayUSB();
			else if (display == VIN) displayVIN();
			else if (display == BOTH) displayBoth();

			previousVUSB = VUSB;
			previousIUSB = IUSB;
			previousVIn = VIn;
			previousIIn = IIn;

		}

	}

	// Puts the device into SLEEP until it receives another I2C command
  if (deviceOFF) {
		INA226_USB.configure(INA226_AVERAGES_16, INA226_BUS_CONV_TIME_1100US, INA226_SHUNT_CONV_TIME_1100US, INA226_MODE_POWER_DOWN);
	  INA226_VIN.configure(INA226_AVERAGES_16, INA226_BUS_CONV_TIME_1100US, INA226_SHUNT_CONV_TIME_1100US, INA226_MODE_POWER_DOWN);
		oled.ssd1306WriteCmd(SSD1306_DISPLAYOFF);

		wdt_disable(); // Probably should be called in LowPower so might be removed if LowPower library is modified.
		LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_ON);

		// Wakes up here

		oled.ssd1306WriteCmd(SSD1306_DISPLAYON);
		deviceOFF = false;
	}
  else {
		LowPower.powerDown((period_t)period, ADC_OFF, BOD_ON);
	}

  // Triggering another measurement.
  launchMeasurement();
}


PowMeterNanoClass PowMeterNano;