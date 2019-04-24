/* 
 *  This sketch shows how to calibrate the device 
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

void setup() {

  Serial.begin(57600);
  Serial.println("Begin setup");    
  Wire.begin (MY_ADDRESS);   

  Serial.println("USB CURRENT OFFSET xxx uA");
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_USB_CURRENT_OFFSET, 20);
  if (error > 0) Serial.println(error);

  Serial.println("VIN CURRENT OFFSET xxx uA");
  error = sendCmdToPowMeter(MY_ADDRESS, POWER_METER_ADDRESS, SET_VIN_CURRENT_OFFSET, 250);
  if (error > 0) Serial.println(error);

  Serial.println("End setup");    
}

void loop() {
  
}
