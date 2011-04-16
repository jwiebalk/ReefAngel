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

#ifndef	ReefAngel_h
#define ReefAngel_h

#define ReefAngel_Version "0.8.5.14"
//#define DEV_MODE  // Developer Mode

#include <ReefAngel_Globals.h>
#include <ReefAngel_EEPROM.h>  // NOTE read/write internal memory
#include <Time.h>
#include <ReefAngel_NokiaLCD.h>
#include <ReefAngel_ATO.h>
#include <ReefAngel_Joystick.h>
#include <ReefAngel_LED.h>
#include <ReefAngel_TempSensor.h>
#include <ReefAngel_Relay.h>
#include <ReefAngel_PWM.h>
#include <ReefAngel_Timer.h>
#include <ReefAngel_Memory.h>
#include <avr/pgmspace.h>


static unsigned long ButtonDebounce;
static unsigned long RAStart;


class ReefAngelClass
{

public:
	int PHMin,PHMax;
	ParamsStruct Params;
	ReefAngelClass();
	ReefAngel_NokiaLCD LCD;
	ReefAngel_JoystickClass Joystick;
	ReefAngel_LEDClass LED;
	DS1307RTC RTC;
	ReefAngel_ATOHighClass HighATO;
	ReefAngel_ATOLowClass LowATO;
	ReefAngel_TempSensorClass TempSensor;
	ReefAngel_RelayClass Relay;
#if defined DisplayLEDPWM && ! defined RemoveAllLights
	ReefAngel_PWMClass PWM;
#endif  // defined DisplayLEDPWM && ! defined RemoveAllLights
	/*
	Timers:
	0 - Feeding Mode timer
	1 - Wavemaker 1 or Dosing Pump 1
	2 - Wavemaker 2 or Dosing Pump 2
	3 - Backlight timer / Sleep timer
	4 - Not used
	5 - Store params to eeprom
	*/
	ReefAngel_TimerClass Timer[6];
	byte SelectedMenuItem;

	// Ports to toggle during different modes
	byte FeedingModePorts;
	byte WaterChangePorts;
	byte OverheatShutoffPorts;
#ifdef RelayExp
	// Expansion Module ports
	byte FeedingModePortsE[MAX_RELAY_EXPANSION_MODULES];
	byte WaterChangePortsE[MAX_RELAY_EXPANSION_MODULES];
	byte OverheatShutoffPortsE[MAX_RELAY_EXPANSION_MODULES];
#endif  // RelayExp
#ifndef RemoveAllLights
	byte LightsOnPorts;
#ifdef RelayExp
	byte LightsOnPortsE[MAX_RELAY_EXPANSION_MODULES];
#endif  // RelayExp
#endif  // RemoveAllLights

#ifdef WavemakerSetup
    // TODO find a better way to save the wavemaker ports for restarting once timers are updated from setup screen
    byte WM1Port;
    byte WM2Port;
#endif  // WavemakerSetup

	void Init();
	void Refresh();
	void SetTemperatureUnit(byte unit);
	void StandardLights(byte LightsRelay, byte OnHour, byte OnMinute, byte OffHour, byte OffMinute);
	void MHLights(byte LightsRelay, byte OnHour, byte OnMinute, byte OffHour, byte OffMinute, byte MHDelay);
	void StandardHeater(byte HeaterRelay, int LowTemp, int HighTemp);
	void StandardFan(byte FanRelay, int LowTemp, int HighTemp);
	void StandardATO(byte ATORelay, int ATOTimeout);
	void SingleATO(bool bLow, byte ATORelay, byte byteTimeout, byte byteHrInterval);
	void DosingPump(byte DPRelay, byte DPTimer, byte OnHour, byte OnMinute, byte RunTime);
	void DosingPumpRepeat(byte DPRelay, byte DPTimer, int RepeatMinute, byte RunTime);
	void Wavemaker(byte WMRelay, byte WMTimer);

	// Simplified PDE versions of the calls
	void StandardLights(byte Relay);
	void MHLights(byte Relay);
	void StandardHeater(byte Relay);
	void StandardFan(byte Relay);
	void StandardATO(byte Relay);
	void SingleATOLow(byte Relay);
	void SingleATOHigh(byte Relay);
	void DosingPump1(byte Relay);
	void DosingPump2(byte Relay);
	void DosingPumpRepeat1(byte Relay);
	void DosingPumpRepeat2(byte Relay);
	void Wavemaker1(byte WMRelay);
	void Wavemaker2(byte WMRelay);
#ifdef VersionMenu
	void DisplayVersion();
#endif  // VersionMenu
	void ClearScreen(byte Color);
#ifdef DisplayLEDPWM
	void MoonlightPWM(byte RelayID, bool ShowPWM);
#endif  // DisplayLEDPWM

	// Logging Functions - WebBanner and PCLogging
	void PCLogging();
#ifdef wifi
	void LoadWebBanner(int pointer, byte qty);
	void WebBanner();
#endif  // wifi

    // Nested Menu Functions
    void InitMenus();
    void ShowInterface();
    void DisplayMenu();
    void DisplayMenuHeading();
    void DisplayMenuEntry(char *text);
    void ProcessButtonPress();
    void ProcessButtonPressMain();
    void ProcessButtonPressSetup();
#ifndef RemoveAllLights
    void ProcessButtonPressLights();
#endif  // RemoveAllLights
    void ProcessButtonPressTemps();
#if defined SetupExtras || defined ATOSetup
    void ProcessButtonPressTimeouts();
#endif  // if defined SetupExtras || defined ATOSetup

    // Setup Screens
    bool SetupOption(int &v, int &y, int rangemin, int rangemax, byte maxdigits,
                     char* unit, char* subunit, char* title,
                     char* prefix1, char* prefix2);
    void SetupLightsOptionDisplay(bool bMetalHalide);
    void SetupCalibratePH();
#ifdef DateTimeSetup
    void SetupDateTime();
#endif  // DateTimeSetup
#ifdef DosingPumpSetup
    void SetupDosingPump();
#endif  // DosingPumpSetup

private:
	bool showmenu;
	time_t menutimeout;
	byte taddr;

	// Nested Menu variables
	int menusptr[Total_Menus];
	byte menuqtysptr[Total_Menus];
	byte DisplayedMenu;
	byte PreviousMenu;
	bool redrawmenu;

#ifdef wifi
	// WebBanner variables
	int webbannerpointer;
	byte webbannerqty;
#endif  // wifi

};


extern ReefAngelClass ReefAngel;  // make an instance for the user

#endif

