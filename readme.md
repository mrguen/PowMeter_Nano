Version: 1.0 by Pandauino.com / Thierry GUENNOU - April 2019

PowMeterNano is a tiny shield that is mounted on top of a Nano board or used as standalone device to monitor 2 power sources (USB and VIN)

USB channel resolution is 25 uA and measures up to 0.5A (or 0.8 A)

VIN channel resolution is 250 uA and mesaures up to 8A (short time, depending on the heat dissipation of your project)

For more info see the user's manual in /doc.

It uses these libraries:
- PowMeterNano_typedefs.h : defines a couple of enums used to pass I2C commands to the shield
- INA226.h library from https://github.com/jarzebski/Arduino-INA226
- "SSD1306Ascii.h", "SSD1306AsciiWire.h": these libraries are part of https://github.com/greiman/SSD1306Ascii and have been slightly modified in https://github.com/mrguen/SSD1306Ascii to provide a "big" but very lightweight numeric font
  (files: /src/fonts/CalBlk36nums.h, CalBlk36blank.h and allFonts.h)
- LowPower library from https://github.com/rocketscream/Low-Power
- EEPROMex.h library from http://thijs.elenbaas.net/2012/07/extended-eeprom-library-for-arduino


This software is licensed under https://creativecommons.org/licenses/by-nc-sa/4.0/

You are free to:
Share a copy and redistribute the material in any medium or format
Adapt a remix, transform, and build upon the material
The licensor cannot revoke these freedoms as long as you follow the license terms.
Under the following terms:
Attribution: You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
NonCommercial: You may not use the material for commercial purposes.
ShareAlike: If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
No additional restrictions: You may not apply legal terms or technological measures that legally restrict others from doing anything the license permits.

This code is based on Arduino code and is provided with the same warning that:
THIS SOFTWARE IS PROVIDED TO YOU "AS IS" AND WE MAKE NO EXPRESS OR IMPLIED WARRANTIES WHATSOEVER WITH RESPECT TO ITS FUNCTIONALITY, OPERABILITY, OR USE,
INCLUDING, WITHOUT LIMITATION, ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR INFRINGEMENT.
WE EXPRESSLY DISCLAIM ANY LIABILITY WHATSOEVER FOR ANY DIRECT, INDIRECT, CONSEQUENTIAL, INCIDENTAL OR SPECIAL DAMAGES, INCLUDING, WITHOUT LIMITATION, LOST REVENUES,
LOST PROFITS, LOSSES RESULTING FROM BUSINESS INTERRUPTION OR LOSS OF DATA, REGARDLESS OF THE FORM OF ACTION OR LEGAL THEORY UNDER WHICH THE LIABILITY MAY BE ASSERTED,
EVEN IF ADVISED OF THE POSSIBILITY OR LIKELIHOOD OF SUCH DAMAGES.

