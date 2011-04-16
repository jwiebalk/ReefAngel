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

 /*
  * Updated by:  Curt Binder
  * Updates Released under Apache License, Version 2.0
  */

#ifndef __REEFANGEL_GLOBALS_H__
#define __REEFANGEL_GLOBALS_H__

#include <ReefAngel_Features.h>
#ifdef COLORS_PDE
#include <ReefAngel_CustomColors.h>
#endif  // COLORS_PDE
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

// Relay Box Modules
#define MAX_RELAY_EXPANSION_MODULES     8
// Expansion Box Module Port Defines
// Box0 is the main / original box
// Boxes 1 - 8 are the expansion boxes
#define Box0_Port1			Port1
#define Box0_Port2			Port2
#define Box0_Port3			Port3
#define Box0_Port4			Port4
#define Box0_Port5			Port5
#define Box0_Port6			Port6
#define Box0_Port7			Port7
#define Box0_Port8			Port8
#define Box1_Port1			11
#define Box1_Port2			12
#define Box1_Port3			13
#define Box1_Port4			14
#define Box1_Port5			15
#define Box1_Port6			16
#define Box1_Port7			17
#define Box1_Port8			18
#define Box2_Port1			21
#define Box2_Port2			22
#define Box2_Port3			23
#define Box2_Port4			24
#define Box2_Port5			25
#define Box2_Port6			26
#define Box2_Port7			27
#define Box2_Port8			28
#define Box3_Port1			31
#define Box3_Port2			32
#define Box3_Port3			33
#define Box3_Port4			34
#define Box3_Port5			35
#define Box3_Port6			36
#define Box3_Port7			37
#define Box3_Port8			38
#define Box4_Port1			41
#define Box4_Port2			42
#define Box4_Port3			43
#define Box4_Port4			44
#define Box4_Port5			45
#define Box4_Port6			46
#define Box4_Port7			47
#define Box4_Port8			48
#define Box5_Port1			51
#define Box5_Port2			52
#define Box5_Port3			53
#define Box5_Port4			54
#define Box5_Port5			55
#define Box5_Port6			56
#define Box5_Port7			57
#define Box5_Port8			58
#define Box6_Port1			61
#define Box6_Port2			62
#define Box6_Port3			63
#define Box6_Port4			64
#define Box6_Port5			65
#define Box6_Port6			66
#define Box6_Port7			67
#define Box6_Port8			68
#define Box7_Port1			71
#define Box7_Port2			72
#define Box7_Port3			73
#define Box7_Port4			74
#define Box7_Port5			75
#define Box7_Port6			76
#define Box7_Port7			77
#define Box7_Port8			78
#define Box8_Port1			81
#define Box8_Port2			82
#define Box8_Port3			83
#define Box8_Port4			84
#define Box8_Port5			85
#define Box8_Port6			86
#define Box8_Port7			87
#define Box8_Port8			88

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
#define I2CExpModule        0x38 // 0x38-3f

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
#define Mem_I_DP1RepeatInterval	  VarsStart+43
#define Mem_I_DP2RepeatInterval	  VarsStart+45
// Next value starts VarsStart+47

// EEProm Pointers
#define PH_Min		        949
#define PH_Max		        951
#define T1Pointer			953

#define KeyPressRate		250
#define DEGREE_F            0
#define DEGREE_C            1
#define MAX_TEMP_SWING      50  // Maximum value of temp change that can occur in 1 second, 100 = 10.0 F Degrees, 50 = 5.0 F Degrees
#define WEB_BANNER_QTY		12  // Quantity of parameters when sending the web banner

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
#define FEEDING_MODE		253
#define WATERCHANGE_MODE	252
#define DEFAULT_MENU_ITEM   0     // default menu item, first item on menu
#define MAIN_MENU           0

#ifndef COLORS_PDE
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

/*
Ability to completely customize all colors of the screen
To change an aspect of the screen, you simply need to change the color defined for the variable.
The variables are labeled intuitively.  Please note that when changing the entire screen color
to another color, you must make sure that all the corresponding BG colors (background colors) match
otherwise you will have a non-consistent background screen.  This is primarily the case when displaying
the main screen / homepage.

Also note that if you intend to change the colors, you will want to experiment to find a good color
combination.  If the background color is changed, you will most likely need to alter all of the other
colors in order for the screen to be easily readable.

You can use any of the colors specified above OR you can specify your own colors in hex code.
The values can range from 0x00 to 0xFF.  Most of the common colors are specified above, so the use
of your own hex code (number) is most likely not needed.  If you want to see what the colors will look like,
on the ReefAngel Google Groups page is a Color Chart image that will show you the colors.
*/

#define T1TempColor         COLOR_RED  // Text color for the T1 temp probe (homescreen)
#define T2TempColor         COLOR_CHOCOLATE  // Text color for the T2 temp probe (homescreen)
#define T3TempColor         COLOR_MEDIUMORCHID  // Text color for the T3 temp probe (homescreen)
#define PHColor             COLOR_SEAGREEN  //originally 0x34, Text color for the PH probe (homescreen)
#define DPColor             COLOR_SADDLEBROWN // Text color for Daylight PWM value (homescreen)
#define APColor             COLOR_DARKSLATEBLUE  //originally 0x49, Text color for the Actinic PWM value (homescreen)
#define CalibrateColor      COLOR_RED  // color of the PH calibration text values
#define ModeScreenColor     COLOR_BLUE  // color of the text on Feeding Mode, Water Change Mode and Version screens
#define DateTextColor       COLOR_RED  // color of the date on home screen
#define OutletBorderColor   0x54  // Border around the outlet box on home screen
#define OutletOnBGColor     0x0D  // Outlet box background color when outlet is on (homescreen)
#define OutletOnFGColor     COLOR_WHITE  // Outlet box text (number) color when outlet is on (homescreen)
#define OutletOffBGColor    COLOR_WHITE  // Outlet box background color when outlet is off (homescreen)
#define OutletOffFGColor    COLOR_BLACK  // Outlet box text (number) color when outlet is off (homescreen)
#define BtnBorderColor      COLOR_BLACK  // OK & Cancel button border color (setup screens)
#define BtnActiveColor      COLOR_GRAY  // OK & Cancel button ACTIVE (selected) background color (seup screens)
#define BtnInactiveColor    COLOR_LIGHTGRAY  // OK & Cancel button INACTIVE (not selected) background color (setup screens)
#define SelectionBGColor    COLOR_BLUE  // Background color for selected items (Menu & Setup screens)
#define SelectionFGColor    COLOR_WHITE  // Foreground (text) color for selected items (Menu & Setup screens)
#define DefaultBGColor      COLOR_WHITE  // Default background color
#define DefaultFGColor      COLOR_BLACK  // Default text color
#define GraphDotLineColor   0x49    // color of the dotted line in the middle of the graph

#endif  // COLORS_PDE

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
byte PWMSlope(byte startHour, byte startMinute, byte endHour, byte endMinute, byte startPWM, byte endPWM, byte Duration, byte oldValue);

// for virtual functions
extern "C" void __cxa_pure_virtual(void);
// other fixes
__extension__ typedef int __guard __attribute__((mode (__DI__)));
extern "C" int __cxa_guard_acquire(__guard *);
extern "C" void __cxa_guard_release (__guard *);
extern "C" void __cxa_guard_abort (__guard *);

#endif  // __REEFANGEL_GLOBALS_H__
