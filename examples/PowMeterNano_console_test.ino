/* 
 * This sketch shows the use of most of PowMeterNano I2C commands. 
 * 
 *  Version: 1.0
 *  By Pandauino.com / Thierry GUENNOU 
 *  April 2019
 *    
 * Load it on the host board and it will send commands to PowMeter shield through I2C.
 * This way you can dynamically configure the shield and get voltage and current measures.
 * 
 * See PowMeterNano_console.h for a full list of commands and parameters.
 * 
 * If needed the I2C address of PowMeter can be changed from its default "0x25" value.
 * You can also use these commands once to configure PowMeter, then charge your regular sketch on the host board. The configuration will be saved in PowMeter's EEPROM. 
 * 
 * Program the Nano host with this sketch then power the shield + Nano host from the USB plug of the shield. You can use the special split cable to power the shield
 * and program the board at the same time. See the user's manual tio check how to connect the Nano host to the shield.

 *  This software is licensed under https://creativecommons.org/licenses/by-nc-sa/4.0/
 *  You are free to:
 *  Share a copy and redistribute the material in any medium or format
 *  Adapt a remix, transform, and build upon the material
 *  The licensor cannot revoke these freedoms as long as you follow the license terms.
 *  Under the following terms:
 *  Attribution: You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
 *  NonCommercial: You may not use the material for commercial purposes.
 *  ShareAlike: If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
 *  No additional restrictions: You may not apply legal terms or technological measures that legally restrict others from doing anything the license permits.
 *  
 *  This code is based on Arduino code and is provided with the same warning that:  
 *  THIS SOFTWARE IS PROVIDED TO YOU "AS IS" AND WE MAKE NO EXPRESS OR IMPLIED WARRANTIES WHATSOEVER WITH RESPECT TO ITS FUNCTIONALITY, OPERABILITY, OR USE, 
 *  INCLUDING, WITHOUT LIMITATION, ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR INFRINGEMENT. 
 *  WE EXPRESSLY DISCLAIM ANY LIABILITY WHATSOEVER FOR ANY DIRECT, INDIRECT, CONSEQUENTIAL, INCIDENTAL OR SPECIAL DAMAGES, INCLUDING, WITHOUT LIMITATION, LOST REVENUES, 
 *  LOST PROFITS, LOSSES RESULTING FROM BUSINESS INTERRUPTION OR LOSS OF DATA, REGARDLESS OF THE FORM OF ACTION OR LEGAL THEORY UNDER WHICH THE LIABILITY MAY BE ASSERTED, 
 *  EVEN IF ADVISED OF THE POSSIBILITY OR LIKELIHOOD OF SUCH DAMAGES. 
 *  
 */ 
 

#include <PowMeterNano_console.h>

const int POWER_METER_ADDRESS = 0x25;
const int MY_ADDRESS = 0x24;

  
void setup() {

  byte error = 0; 	// To test the return status
  
  union value_tag {   // To buffer returned 4 bytes values and "convert" them to float
    byte b[4];
    float fval;
  } value;
  
  Serial.begin(57600);
  Serial.println("Begin setup");    
  Wire.begin (MY_ADDRESS);   
  
 // =============================================================================
  // QUERIES VALUES 

  sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, GET_USB_VOLTAGE, 0, value.b, 4); 
  Serial.print("Last USB voltage value is : ");
  Serial.println(value.fval, 2);
  
  sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, GET_USB_CURRENT, 0, value.b, 4); 
  Serial.print("Last USB current value is : ");
  Serial.println(value.fval, 2);

  sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, GET_VIN_VOLTAGE, 0, value.b, 4); 
  Serial.print("Last VIN voltage value is : ");
  Serial.println(value.fval, 2);

  sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, GET_VIN_CURRENT, 0, value.b, 4); 
  Serial.print("Last VIN current value is : ");
  Serial.println(value.fval, 2);

  
  // =============================================================================
  // PERIOD
  
  Serial.println("MEASUREMENT DELAY 0.25 SECONDS");

  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_PERIOD, SLEEP_250MS);
  if (error) {
    // Error message already sent to serial, but you can also do whathever is needed
    // Calls to sendCmdToPowMeter below do not test the error status 
  } 
   
  delay(4000);

  Serial.println("MEASUREMENT DELAY 1 SECOND");

  // Thereafter we don't test the return status. If one happened a message was sent to Serial.  
  sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_PERIOD, SLEEP_1S);

  
  // =============================================================================
  // DISPLAY 

  Serial.println("DISPLAY USB");
  sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_DISPLAY, USB);
  delay(4000);

  Serial.println("DISPLAY VIN");
  sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_DISPLAY, VIN);
  delay(4000);

  Serial.println("DISPLAY AUTO");
  sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_DISPLAY, AUTO);
  delay(4000);

  // Back to display both
  Serial.println("DISPLAY BOTH");
  sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_DISPLAY, BOTH);


  // =============================================================================  
  // SOFTWARE ALERT 

  Serial.println("USB SOFT ALERT MIN VOLTAGE 6V (UNREALISTIC)");

  // Will activate USB_SOFT_ALERT and for this dummy test throw an alert if the voltage is under 6V

  sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_MIN_VOLTAGE, 600);  // 6V * 100 
  sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_SOFT_ALERT, true);
  delay(4000);

  // Deactivate alert
  sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_SOFT_ALERT, false);


  // =============================================================================  
  // HARDWARE ALERT 

  Serial.println("USB HARD ALERT MAX VOLTAGE 4V (UNREALISTIC)");

  // Will activate OVER_VOLTAGE hardware alert on the USB channel. For this dummy test an alert is raised if the voltage is above 4V

  sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_MAX_VOLTAGE, 400);  // 4V * 100 
  sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_HARD_ALERT, OVER_VOLTAGE);

  // The "USB Alert" test point must be at low level (normally high level)
  delay(4000);

  // Deactivate alert
  sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_HARD_ALERT, ALERT_OFF);


  // =============================================================================
  // POWER OFF

  Serial.println("SHIELD OFF");
  sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_POWER, POWER_OFF);
  delay(4000);

  // Reactivate shield
  Serial.println("SHIELD ON");
  sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_POWER, POWER_ON);
  delay(4000);
   

  // =============================================================================
  // CUSTOM I2C ADDRESS

  Serial.println("CHANGE I2C ADDRESS TO 0x20");
  sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_I2C_ADDRESS, 0x20);


  // =============================================================================
  // OLED ON/OFF 

  Serial.println("TURN OLED OFF");
  sendCmdToPowMeter(MY_ADDRESS, 0x20, SET_POWER, OLED_OFF);
  delay(4000);
  
  Serial.println("TURN OLED ON");
  sendCmdToPowMeter(MY_ADDRESS, 0x20, SET_POWER, OLED_ON);
  delay(4000);


  // =============================================================================
  // BACK TO DEFAULT I2C ADDRESS  

  Serial.println("CHANGE I2C ADDRESS BACK TO DEFAULT");
  sendCmdToPowMeter(MY_ADDRESS, 0x20, SET_I2C_ADDRESS, POWER_METER_ADDRESS);
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
