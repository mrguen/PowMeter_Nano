/* 
 *  This sketch shows the use of all PowMeter Nano v1.0 I2C commands. 
 *  See PowMeterNano_console_test for basic explanation.
 * 
 *  Version: 1.0
 *  By Pandauino.com / Thierry GUENNOU 
 *  April 2019
 *    
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

byte error = 0;   // To test the return status

union value_tag {   // To buffer returned 4 bytes values and "convert" them to float
  byte b[4];
  float fval;
} value;

void setup() {

  Serial.begin(57600);
  Serial.println("Begin setup");    
  Wire.begin (MY_ADDRESS);   

  // =============================================================================
  // PERIOD

  Serial.println("MEASUREMENT DELAY 0.12 SECONDS");
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_PERIOD, SLEEP_120MS);
  if (error > 0) Serial.println(error);
  delay(4000);

  Serial.println("MEASUREMENT DELAY 0.25 SECONDS");
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_PERIOD, SLEEP_250MS);
  if (error > 0) Serial.println(error);
  delay(4000);

  Serial.println("MEASUREMENT DELAY 0.5 SECONDS");
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_PERIOD, SLEEP_500MS);
  if (error > 0) Serial.println(error);
  delay(4000);

  Serial.println("MEASUREMENT DELAY 2 SECONDS");
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_PERIOD, SLEEP_2S);
  if (error > 0) Serial.println(error);
  delay(4000);

  Serial.println("MEASUREMENT DELAY 4 SECONDS");
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_PERIOD, SLEEP_4S);
  if (error > 0) Serial.println(error);
  delay(4000);

  Serial.println("MEASUREMENT DELAY 8 SECONDS");
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_PERIOD, SLEEP_8S);
  if (error > 0) Serial.println(error);
  delay(10000);

  Serial.println("MEASUREMENT DELAY 1 SECOND");
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_PERIOD, SLEEP_1S);
  if (error > 0) Serial.println(error);

  // =============================================================================
  // DISPLAY 

  Serial.println("DISPLAY USB");
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_DISPLAY, USB);
  if (error > 0) Serial.println(error);
  delay(4000);

  Serial.println("DISPLAY VIN");
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_DISPLAY, VIN);
  if (error > 0) Serial.println(error);
  delay(4000);

  // Back to display both
  Serial.println("DISPLAY BOTH");
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_DISPLAY, BOTH);
  if (error > 0) Serial.println(error);
  delay(4000);

  Serial.println("DISPLAY AUTO");
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_DISPLAY, AUTO);
  if (error > 0) Serial.println(error);
  delay(4000);


  // =============================================================================  
  // SOFTWARE ALERT 

  // *************** USB
  Serial.println("USB SOFT ALERT MIN VOLTAGE 6V");
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_MIN_VOLTAGE, 600);  // 6V * 100 
  if (error > 0) Serial.println(error);
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_SOFT_ALERT, true);
  if (error > 0) Serial.println(error);
  delay(4000);

  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_SOFT_ALERT, false);
  if (error > 0) Serial.println(error);
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_MIN_VOLTAGE, -1); 
  if (error > 0) Serial.println(error);
  delay(2000);

  Serial.println("USB SOFT ALERT MAX VOLTAGE 4V");
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_MAX_VOLTAGE, 400);
  if (error > 0) Serial.println(error);
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_SOFT_ALERT, true);
  if (error > 0) Serial.println(error);
  delay(4000);

  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_SOFT_ALERT, false);
  if (error > 0) Serial.println(error);
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_MAX_VOLTAGE, 2400);
  if (error > 0) Serial.println(error);
  delay(2000);

  Serial.println("USB SOFT ALERT MIN CURRENT 100 mA");
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_MIN_CURRENT, 100);
  if (error > 0) Serial.println(error);
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_SOFT_ALERT, true);
  if (error > 0) Serial.println(error);
  delay(4000);

  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_SOFT_ALERT, false);
  if (error > 0) Serial.println(error);
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_MIN_CURRENT, -1);
  if (error > 0) Serial.println(error);
  delay(2000);

  Serial.println("USB SOFT ALERT MAX CURRENT 10 mA");
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_MAX_CURRENT, 10);
  if (error > 0) Serial.println(error);
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_SOFT_ALERT, true);
  if (error > 0) Serial.println(error);
  delay(4000);

  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_SOFT_ALERT, false);
  if (error > 0) Serial.println(error);
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_MAX_CURRENT, 10000);
  if (error > 0) Serial.println(error);
  delay(2000);

  
  // *************** VIN
  
  Serial.println("VIN SOFT ALERT MIN VOLTAGE 24V ");
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_VIN_MIN_VOLTAGE, 2400);
  if (error > 0) Serial.println(error);
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_VIN_SOFT_ALERT, true);
  if (error > 0) Serial.println(error);
  delay(4000);
  
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_VIN_SOFT_ALERT, false);
  if (error > 0) Serial.println(error);
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_VIN_MIN_VOLTAGE, -1);
  if (error > 0) Serial.println(error);
  delay(2000);

  Serial.println("VIN SOFT ALERT MAX VOLTAGE -1V ");
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_VIN_MAX_VOLTAGE, -100);
  if (error > 0) Serial.println(error);
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_VIN_SOFT_ALERT, true);
  if (error > 0) Serial.println(error);
  delay(4000);
  
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_VIN_SOFT_ALERT, false);
  if (error > 0) Serial.println(error);
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_VIN_MAX_VOLTAGE, 2400);
  if (error > 0) Serial.println(error);
  delay(2000);

  Serial.println("VIN SOFT ALERT MIN CURRENT 100 mA ");
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_VIN_MIN_CURRENT, 100);
  if (error > 0) Serial.println(error);
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_VIN_SOFT_ALERT, true);
  if (error > 0) Serial.println(error);
  delay(4000);
  
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_VIN_SOFT_ALERT, false);
  if (error > 0) Serial.println(error);
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_VIN_MIN_CURRENT, -1);
  if (error > 0) Serial.println(error);
  delay(2000);

  Serial.println("VIN SOFT ALERT MAX CURRENT -1 mA ");
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_VIN_MAX_CURRENT, -1);
  if (error > 0) Serial.println(error);
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_VIN_SOFT_ALERT, true);
  if (error > 0) Serial.println(error);
  delay(4000);

  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_VIN_SOFT_ALERT, false);
  if (error > 0) Serial.println(error);
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_VIN_MAX_CURRENT, 10000);
  if (error > 0) Serial.println(error);
  delay(2000);


  // =============================================================================  
  // HARDWARE ALERT 

 // *************** USB
 
  Serial.println("USB HARD ALERT MIN VOLTAGE 6V");

  // Will activate UNDER_VOLTAGE hardware alert on the USB channel. For this dummy test an alert is raised if the voltage is under 6V

  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_MIN_VOLTAGE, 600);   
  if (error > 0) Serial.println(error);
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_HARD_ALERT, UNDER_VOLTAGE);
  if (error > 0) Serial.println(error);

  // The "USB Alert" test point must be at low level (normally high level)
  delay(4000);

  // Deactivate alert
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_HARD_ALERT, ALERT_OFF);
  if (error > 0) Serial.println(error);
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_MIN_VOLTAGE, -1);  
  if (error > 0) Serial.println(error);
  delay(2000);

  Serial.println("USB HARD ALERT MAX VOLTAGE 4V");
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_MAX_VOLTAGE, 400);  
  if (error > 0) Serial.println(error);
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_HARD_ALERT, OVER_VOLTAGE);
  if (error > 0) Serial.println(error);
  delay(4000);

  // Deactivate alert
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_HARD_ALERT, ALERT_OFF);
  if (error > 0) Serial.println(error);
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_MAX_VOLTAGE, 550);  
  if (error > 0) Serial.println(error);
  delay(2000);

  Serial.println("USB HARD ALERT MIN CURRENT 100 mA");
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_MIN_CURRENT, 100);  
  if (error > 0) Serial.println(error);
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_HARD_ALERT, UNDER_CURRENT);
  if (error > 0) Serial.println(error);
  delay(4000);

  // Deactivate alert
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_HARD_ALERT, ALERT_OFF);
  if (error > 0) Serial.println(error);
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_MIN_CURRENT, -1);  
  if (error > 0) Serial.println(error);
  delay(2000);

  Serial.println("USB HARD ALERT MAX CURRENT 10 mA");
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_MAX_CURRENT, 10);  
  if (error > 0) Serial.println(error);
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_HARD_ALERT, OVER_CURRENT);
  if (error > 0) Serial.println(error);
  delay(4000);

  // Deactivate alert
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_HARD_ALERT, ALERT_OFF);
  if (error > 0) Serial.println(error);
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_MAX_CURRENT, 2400);  
  if (error > 0) Serial.println(error);
  delay(2000);


 // *************** VIN

  Serial.println("VIN HARD ALERT MIN VOLTAGE 24V");

  // Will activate UNDER_VOLTAGE hardware alert on the VIN channel. For this dummy test an alert is raised if the voltage is under 24V

  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_VIN_MIN_VOLTAGE, 2400);   
  if (error > 0) Serial.println(error);
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_VIN_HARD_ALERT, UNDER_VOLTAGE);

  // The "VIN Alert" test point must be at low level (normally high level)
  if (error > 0) Serial.println(error);
  delay(4000);

  // Deactivate alert
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_VIN_HARD_ALERT, ALERT_OFF);
  if (error > 0) Serial.println(error);
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_VIN_MIN_VOLTAGE, -1);  
  if (error > 0) Serial.println(error);
  delay(2000);

  Serial.println("VIN HARD ALERT MAX VOLTAGE 0 V");
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_VIN_MAX_VOLTAGE, 0);   
  if (error > 0) Serial.println(error);
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_VIN_HARD_ALERT, OVER_VOLTAGE);
  if (error > 0) Serial.println(error);
  delay(4000);

  // Deactivate alert
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_VIN_HARD_ALERT, ALERT_OFF);
  if (error > 0) Serial.println(error);
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_VIN_MAX_VOLTAGE, 2400);  
  if (error > 0) Serial.println(error);
  delay(2000);

  Serial.println("VIN HARD ALERT MIN CURRENT 100 mA");
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_VIN_MIN_CURRENT, 100);  
  if (error > 0) Serial.println(error);
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_VIN_HARD_ALERT, UNDER_CURRENT);
  if (error > 0) Serial.println(error);
  delay(4000);

  // Deactivate alert
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_VIN_HARD_ALERT, ALERT_OFF);
  if (error > 0) Serial.println(error);
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_VIN_MIN_CURRENT, -1);  
  if (error > 0) Serial.println(error);
  delay(2000);

  Serial.println("VIN HARD ALERT MAX CURRENT 0 mA ");
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_VIN_MAX_CURRENT, 0);  
  if (error > 0) Serial.println(error);
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_VIN_HARD_ALERT, OVER_CURRENT);
  if (error > 0) Serial.println(error);
  delay(4000);

  // Deactivate alert
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_VIN_HARD_ALERT, ALERT_OFF);
  if (error > 0) Serial.println(error);
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_VIN_MAX_CURRENT, 10000);  
  if (error > 0) Serial.println(error);
  delay(2000);


  // =============================================================================
  // POWER OFF

  Serial.println("SHIELD OFF");
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_POWER, POWER_OFF);
  if (error > 0) Serial.println(error);
  delay(10000);

  // Reactivate shield
  Serial.println("SHIELD ON");
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_POWER, POWER_ON);
  if (error > 0) Serial.println(error);
  delay(4000);

  Serial.println("MEASUREMENT HALT");
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_POWER, MEASUREMENT_HALT);
  if (error > 0) Serial.println(error);
  delay(4000);
 
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_POWER, POWER_ON);
  if (error > 0) Serial.println(error);
  delay(4000);
   
  // =============================================================================
  // CUSTOM I2C ADDRESS

  Serial.println("CHANGE I2C ADDRESS TO 0x20");
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_I2C_ADDRESS, 0x20);
  if (error > 0) Serial.println(error);


  // =============================================================================
  // OLED ON/OFF 

  Serial.println("TURN OLED OFF");
  error = sendCmdToPowMeter(MY_ADDRESS, 0x20, SET_POWER, OLED_OFF);
  if (error > 0) Serial.println(error);
  delay(4000);
  
  Serial.println("TURN OLED ON");
  error = sendCmdToPowMeter(MY_ADDRESS, 0x20, SET_POWER, OLED_ON);
  if (error > 0) Serial.println(error);
  delay(4000);


  // =============================================================================
  // BACK TO DEFAULT I2C ADDRESS  

  Serial.println("CHANGE I2C ADDRESS BACK TO DEFAULT");
  error = sendCmdToPowMeter(MY_ADDRESS, 0x20, SET_I2C_ADDRESS, POWER_METER_ADDRESS);
  if (error > 0) Serial.println(error);

  // =============================================================================
  // CURRENT OFFSET
  // Might need to calibrate your device again if you test this
  /*
  Serial.println("USB CURRENT OFFSET - 10 mA");
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_CURRENT_OFFSET, -10000);
  if (error > 0) Serial.println(error);
  delay(4000);

  Serial.println("USB CURRENT OFFSET 0 mA");
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_CURRENT_OFFSET, 0);
  if (error > 0) Serial.println(error);
  delay(4000);

  Serial.println("VIN CURRENT OFFSET + 10 mA");
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_VIN_CURRENT_OFFSET, 10000);
  if (error > 0) Serial.println(error);
  delay(4000);

  Serial.println("VIN CURRENT OFFSET 0 mA");
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_VIN_CURRENT_OFFSET, 0);
  if (error > 0) Serial.println(error);
*/
  Serial.println("End setup");    
}

void loop() {
 
 // =============================================================================
 // QUERIES VALUES 
 
  error =  sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, GET_USB_VOLTAGE, 0, value.b, 4); 
  if (error > 0) Serial.println(error);
  Serial.print("Last USB voltage value is : ");
  Serial.println(value.fval, 2);
  
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, GET_USB_CURRENT, 0, value.b, 4); 
  if (error > 0) Serial.println(error);
  Serial.print("Last USB current value is : ");
  Serial.println(value.fval, 2);

  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, GET_VIN_VOLTAGE, 0, value.b, 4); 
  if (error > 0) Serial.println(error);
  Serial.print("Last VIN voltage value is : ");
  Serial.println(value.fval, 2);

  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, GET_VIN_CURRENT, 0, value.b, 4); 
  if (error > 0) Serial.println(error);
  Serial.print("Last VIN current value is : ");
  Serial.println(value.fval, 2);
  
  delay(1000);
  
}
