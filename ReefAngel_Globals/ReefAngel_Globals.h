/*
 * Copyright 2010 Reef Angel / Roberto Imai
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef __REEFANGEL_GLOBALS_H__
#define __REEFANGEL_GLOBALS_H__

#include <ReefAngel_Features.h>
#include <ReefAngel_CustomColors.h>
#include <WProgram.h>
#include <Time.h>
#include <OneWire.h>

// Outlets on Relay box
#define Port8   8
#define Port7   7
#define Port6   6
#define Port5   5
#define Port4   4
#define Port3   3
#define Port2   2
#define Port1   1

//Analog I/O
#define VPin                0
#define HPin                1
#define PHPin               6
#define Piezo               16

//Digital I/O
#define ledPin              7
#define tempPin             8
#define actinicPWMPin       9
#define daylightPWMPin      10
#define lowATOPin           11
#define highATOPin          12
#define okPin               13

// I2C Addresses
#define I2CEEPROM1          0x50
#define I2CEEPROM2          0x54
#define I2CClock            0x68
#define I2CExpander1        0x20
#define I2CExpander2        0x21

// I2C Images Addresses
#define I2CEEPROM2_Main              0     //0-2999
#define I2CEEPROM2_Feeding           9919  //3000-4999
#define I2CEEPROM2_Water_Change      11919  //15300-18104

/*
EEPROM locations

To Read/Write to these locations, use the InternalMemory variable.
Most (if not all) of these memory locations have a read/write function already created.

If a memory location does not have a specific function to read/write to it, you may use the
read/write or read_int/write_int functions to read/write to the memory location.

Byte variables need 1 block
Int variables need 2 blocks

When adding more variables, use the previous value plus 1 or 2
  depending on the previous values storage requirement
*/
#define VarsStart                 800
#define Mem_B_MHOnHour            VarsStart
#define Mem_B_MHOnMinute          VarsStart+1
#define Mem_B_MHOffHour           VarsStart+2
#define Mem_B_MHOffMinute         VarsStart+3
#define Mem_B_StdLightsOnHour     VarsStart+4
#define Mem_B_StdLightsOnMinute   VarsStart+5
#define Mem_B_StdLightsOffHour    VarsStart+6
#define Mem_B_StdLightsOffMinute  VarsStart+7
#define Mem_I_WM1Timer            VarsStart+8
#define Mem_I_WM2Timer            VarsStart+10
#define Mem_B_DP1Timer            VarsStart+12
#define Mem_B_DP2Timer            VarsStart+13
#define Mem_I_FeedingTimer        VarsStart+14
#define Mem_I_LCDTimer            VarsStart+16
#define Mem_I_OverheatTemp        VarsStart+18
#define Mem_B_LEDPWMDaylight      VarsStart+20
#define Mem_B_LEDPWMActinic       VarsStart+21
#define Mem_I_HeaterTempOn        VarsStart+22
#define Mem_I_HeaterTempOff       VarsStart+24
#define Mem_I_ChillerTempOn       VarsStart+26
#define Mem_I_ChillerTempOff      VarsStart+28
#define Mem_B_ATOTimeout          VarsStart+30
#define Mem_I_PHMax               VarsStart+31
#define Mem_I_PHMin               VarsStart+33
#define Mem_B_MHDelay             VarsStart+35
#define Mem_B_DP1OnHour           VarsStart+36
#define Mem_B_DP1OnMinute         VarsStart+37
#define Mem_B_DP2OnHour           VarsStart+38
#define Mem_B_DP2OnMinute         VarsStart+39
#define Mem_B_ATOHourInterval     VarsStart+40
#define Mem_B_ATOHighHourInterval VarsStart+41
#define Mem_B_ATOHighTimeout      VarsStart+42
// Next value starts VarsStart+43

// EEProm Pointers
#define PH_Min		        949
#define PH_Max		        951
#define T1Pointer			953

#define KeyPressRate		250
#define DEGREE_F            0
#define DEGREE_C            1
#define MAX_TEMP_SWING      50  // Maximum value of temp change that can occur in 1 second, 100 = 10.0 F Degrees, 50 = 5.0 F Degrees

// Menu Defines
#if defined SetupExtras || defined ATOSetup
#ifndef RemoveAllLights
#define Total_Menus         5    // total number of menus
#else  // RemoveAllLights
#define Total_Menus         4
#endif  // RemoveAllLights
#else  // if defined SetupExtras || defined ATOSetup
#ifndef RemoveAllLights
#define Total_Menus         4    // total number of menus
#else // RemoveAllLights
#define Total_Menus         3
#endif  // RemoveAllLights
#endif  // if defined SetupExtras || defined ATOSetup
#define MENU_TIMEOUT        10   // timeout for menu to return to main screen
#define MENU_HEADING_SIZE   25   // pixels used for Menu Heading
#define MENU_ITEM_HEIGHT    9    // pixel height of a line of text
#define MENU_START_COL      7
#define MENU_START_ROW      10
#define MENU_END_COL        124
#define MAX_Y               128
#define MAX_X               128
#define DEFAULT_MENU        255
#define EXCEED_TIMEOUT_MENU 254
#define DEFAULT_MENU_ITEM   0     // default menu item, first item on menu
#define MAIN_MENU           0

/*
Graph Range values

Below are listed the ranges for each one of the graph lines.  The range is the specified value for being
"low" and "high".  So the "low" value is the bottom of the graph and the "high" value is the top of the graph.
If the actual value observed is above or below the "high" and "low" value respectively, it will be capped
at the appropriate value.  Meaning, if the T1 high value is set to 90.0 F and the controller actually reads
95.0 F, the highest value reported in the graph will be 90.0 F.

The defaults should work just fine more everybody, but some people may want to alter the values.  This would be
necessary if you didn't want T2 to be used for the lights temperature.

The defaults are as follows:
formate is:  Probe: LOW - HIGH

T1: 70.0F - 90.0F
T2: 65.0F - 150.0F
T3: 65.0F - 92.0F
PH: 7 - 10
*/
#define T1LOW   700
#define T1HIGH  900
#define T2LOW   650
#define T2HIGH  1500
#define T3LOW   650
#define T3HIGH  920
#define PHLOW   550
#define PHHIGH  840

// Global macros
#define SIZE(array) (sizeof(array) / sizeof(*array))

// external globally defined variables
extern byte ButtonPress;  // Defined in ReefCentral.cpp, used for joystick button presses
extern OneWire ds;  // Defined in TempSensor.cpp, used for TempSensorClass

// globally useable functions
byte intlength(int intin);
int NumMins(uint8_t ScheduleHour, uint8_t ScheduleMinute);
bool IsLeapYear(int year);

// for virtual functions
extern "C" void __cxa_pure_virtual(void);
// other fixes
__extension__ typedef int __guard __attribute__((mode (__DI__)));
extern "C" int __cxa_guard_acquire(__guard *);
extern "C" void __cxa_guard_release (__guard *);
extern "C" void __cxa_guard_abort (__guard *);

#endif  // __REEFANGEL_GLOBALS_H__
