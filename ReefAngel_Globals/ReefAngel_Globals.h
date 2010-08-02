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

// EEProm locations
// Byte variables need 1 block
// Int variables need 2 blocks
// When adding more variables, use the previous value plus 1 or 2
//  depending on the previous values storage requirement
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
#define Mem_I_ScreenSaverTimer    VarsStart+16
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
// Next value starts VarsStart+40

// EEProm Pointers
#define PH_Min		        949
#define PH_Max		        951
#define T1Pointer			953

#define KeyPressRate		250
#define DEGREE_F            0
#define DEGREE_C            1

// Menu Defines
#define Total_Menus         7    // total number of menus
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

//  Global Colors
#define COLOR_BLACK                 0x00
#define COLOR_NAVY                  0x02
#define COLOR_BLUE                  0x03
#define COLOR_DARKGREEN             0x0C
#define COLOR_GREEN                 0x10
#define COLOR_TEAL                  0x12
#define COLOR_LIGHT_BLUE            0x13
#define COLOR_DEEPSKYBLUE           0x17
#define COLOR_DARKTURQUOISE         0x1A
#define COLOR_LIME                  0x1C
#define COLOR_SPRINGGREEN           0x1D
#define COLOR_MEDIUMSPRINGGREEN     0x1E
#define COLOR_CYAN                  0x1F
#define COLOR_MIDNIGHTBLUE          0x25
#define COLOR_DARKSLATEGREY         0x29
#define COLOR_FORESTGREEN           0x30
#define COLOR_SEAGREEN              0x31
#define COLOR_DODGERBLUE            0x33
#define COLOR_LIGHTSEAGREEN         0x36
#define COLOR_LIMEGREEN             0x39
#define COLOR_INDIGO                0x42
#define COLOR_DARKSLATEBLUE         0x4A
#define COLOR_DARKOLIVEGREEN        0x4D
#define COLOR_ROYALBLUE             0x4F
#define COLOR_STEELBLUE             0x52
#define COLOR_MEDIUMSEAGREEN        0x55
#define COLOR_TURQUOISE             0x5A
#define COLOR_SLATEBLUE             0x6A
#define COLOR_DIMGREY               0x6D
#define COLOR_MEDIUMSLATEBLUE       0x6F
#define COLOR_OLIVEDRAB             0x70
#define COLOR_SLATEGREY             0x72
#define COLOR_CORNFLOWERBLUE        0x73
#define COLOR_MEDIUMAQUAMARINE      0x7A
#define COLOR_LAWNGREEN             0x7C
#define COLOR_AQUAMARINE            0x7E
#define COLOR_MAROON                0x80
#define COLOR_PURPLE                0x82
#define COLOR_DARKORCHID            0x86
#define COLOR_BLUEVIOLET            0x87
#define COLOR_SADDLEBROWN           0x88
#define COLOR_SIENNA                0x89
#define COLOR_MEDIUMPURPLE          0x8F
#define COLOR_OLIVE                 0x90
#define COLOR_GRAY                  0x92
#define COLOR_DARKSEAGREEN          0x96
#define COLOR_YELLOWGREEN           0x99
#define COLOR_SKYBLUE               0x9B
#define COLOR_LIGHTGREEN            0x9E
#define COLOR_BROWN                 0xA4
#define COLOR_MEDIUMVIOLETRED       0xA6
#define COLOR_MEDIUMORCHID          0xAA
#define COLOR_DARKGOLDENROD         0xB0
#define COLOR_ROSYBROWN             0xB2
#define COLOR_DARKKHAKI             0xB5
#define COLOR_SILVER                0xB6
#define COLOR_LIGHTSTEELBLUE        0xB7
#define COLOR_POWDERBLUE            0xBB
#define COLOR_GREENYELLOW           0xBD
#define COLOR_PALETURQUOISE         0xBF
#define COLOR_CRIMSON               0xC5
#define COLOR_CHOCOLATE             0xCC
#define COLOR_INDIANRED             0xCD
#define COLOR_PALEVIOLETRED         0xCE
#define COLOR_ORCHID                0xCF
#define COLOR_DARKSALMON            0xD1
#define COLOR_PLUM                  0xD3
#define COLOR_GOLDENROD             0xD4
#define COLOR_TAN                   0xD6
#define COLOR_THISTLE               0xD7
#define COLOR_LIGHTGRAY             0xDA
#define COLOR_LAVENDER              0xDB
#define COLOR_LIGHTCYAN             0xDF
#define COLOR_RED                   0xE0
#define COLOR_MAGENTA               0xE3
#define COLOR_DEEPPINK              0xE6
#define COLOR_ORANGERED             0xE8
#define COLOR_TOMATO                0xED
#define COLOR_HOTPINK               0xEE
#define COLOR_DARKORANGE            0xF0
#define COLOR_SALMON                0xF1
#define COLOR_LIGHTCORAL            0xF2
#define COLOR_VIOLET                0xF3
#define COLOR_ORANGE                0xF4
#define COLOR_SANDYBROWN            0xF5
#define COLOR_PINK                  0xF6
#define COLOR_GOLD                  0xF8
#define COLOR_KHAKI                 0xFA
#define COLOR_ANTIQUEWHITE          0xFB
#define COLOR_YELLOW                0xFC
#define COLOR_LIGHTGOLDENRODYELLOW  0xFE
#define COLOR_WHITE                 0xFF


#define WaterTempColor      COLOR_RED
#define LightsTempColor     COLOR_CHOCOLATE
#define AmbientTempColor    COLOR_MEDIUMORCHID
#define PHColor             COLOR_SEAGREEN  //0x31, originally 0x34
#define DPColor             COLOR_SADDLEBROWN
#define APColor             COLOR_DARKSLATEBLUE  //0x4A, originally 0x49


// Global macros
#define SIZE(array) (sizeof(array) / sizeof(*array))

// external globally defined variables
extern byte ButtonPress;  // Defined in ReefCentral.cpp, used for joystick button presses
extern OneWire ds;  // Defined in TempSensor.cpp, used for TempSensorClass

// globally useable functions
byte intlength(int intin);
int NumMins(uint8_t ScheduleHour, uint8_t ScheduleMinute);
// for virtual functions
extern "C" void __cxa_pure_virtual(void);
// other fixes
__extension__ typedef int __guard __attribute__((mode (__DI__)));
extern "C" int __cxa_guard_acquire(__guard *);
extern "C" void __cxa_guard_release (__guard *);
extern "C" void __cxa_guard_abort (__guard *);

#endif  // __REEFANGEL_GLOBALS_H__
