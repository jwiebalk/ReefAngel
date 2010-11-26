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

#ifndef	ReefAngel_h
#define ReefAngel_h

#define ReefAngel_Version "0.8.5.9"
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


#ifdef wifi
#define SERVER_HEADER1 "HTTP/1.1 200 OK" CRLF "Server: ReefAngel" CRLF "Cache-Control: no-store, no-cache, must-revalidate" CRLF "Pragma: no-cache" CRLF "Connection: close" CRLF "Content-Type: text/"
#define SERVER_HEADER2 CRLF "Content-Length: "
#define SERVER_HEADER3 CRLF CRLF
#define SERVER_HEADER_HTML SERVER_HEADER1 "html" SERVER_HEADER2
#define SERVER_HEADER_XML SERVER_HEADER1 "xml" SERVER_HEADER2
#define SERVER_DEFAULT "<h1>Reef Angel Controller Web Server</h1>"
#define SERVER_RELAY ""
#define CRLF "\r\n"
#define CR "\r"
#define LF "\n"
#define P(name)   static const prog_uchar name[] PROGMEM
//#define SERVER_MONITOR "<html><head><script type='text/javascript'>var t;\nfunction timedCount(){loadXMLDoc();t=setTimeout('timedCount()',1000);}\nfunction loadXMLDoc(){if (window.XMLHttpRequest){xmlhttp=new XMLHttpRequest();}else{xmlhttp=new ActiveXObject('Microsoft.XMLHTTP');}\nxmlhttp.onreadystatechange=function(){if (xmlhttp.readyState==4 && xmlhttp.status==200){x=xmlhttp.responseXML.documentElement;\n"
//#define SERVER_MONITOR1 "document.getElementById('T1').innerHTML=x.getElementsByTagName('T1')[0].text;document.getElementById('T2').innerHTML=x.getElementsByTagName('T2')[0].text;\ndocument.getElementById('T3').innerHTML=x.getElementsByTagName('T3')[0].text;document.getElementById('PH').innerHTML=x.getElementsByTagName('PH')[0].text;}}\nxmlhttp.open('GET','params',true);xmlhttp.send();}timedCount();\n</script></head><body>\n<p>Temp1: <div id='T1'></div></p><p>Temp2: <div id='T2'></div></p><p>Temp3: <div id='T3'></div></p><p>PH: <div id='PH'></div></p>\n</body></html>"
//#define SERVER_AJAX "<html><head><script type='text/javascript'>function t(){l();var t1=setTimeout('t()',1000);}function l(){if (window.XMLHttpRequest) {a=new XMLHttpRequest();}else{a=new ActiveXObject('Microsoft.XMLHTTP');}a.onreadystatechange=function(){if (a.readyState==4 && a.status==200){x=a.responseXML.documentElement;document.getElementById('T1').innerHTML=x.getElementsByTagName('T1').item(0).firstChild.data/10;document.getElementById('T2').innerHTML=x.getElementsByTagName('T2').item(0).firstChild.data/10;"
//#define SERVER_AJAX1 "document.getElementById('T3').innerHTML=x.getElementsByTagName('T3').item(0).firstChild.data/10;document.getElementById('PH').innerHTML=x.getElementsByTagName('PH').item(0).firstChild.data/100;}};a.open('GET','params',true);a.send(null);}t();</script><style type='text/css'>.style1 {font-family: Arial, Helvetica, sans-serif;font-weight: bold;font-size: 24px;position: relative;top: 35px;left:-23px;width: 150px;height: 149px;}</style>"
//#define SERVER_AJAX2 "</head><body><table border=0 cellspacing=0 cellpadding=0><tr><td colspan=4 align=center><img src='http://www.reefangel.com/images/ra.jpg'></td></tr><tr><td background='http://www.reefangel.com/images/t1.jpg' align=center><div id=T1 class=style1></div></td><td background='http://www.reefangel.com/images/t2.jpg' align=center><div id=T2 class=style1></div></td><td background='http://www.reefangel.com/images/t3.jpg' align=center><div id=T3 class=style1></div></td><td background='http://www.reefangel.com/images/ph.jpg' align=center><div id=PH class=style1></div></td></tr></table></body></html>"
//http://www.xul.fr/ajax-get-xml.html

const prog_uchar SERVER_RA[] PROGMEM = "<script language='javascript' src='http://www.reefangel.com/wifi/ra1.js'></script>";

static char m_pushback[32];
static char m_pushbackDepth=0;
static byte reqtype=0;
static unsigned long timeout;
static bool conn=false;
#endif

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
#ifndef RemoveAllLights
	byte LightsOnPorts;
#endif  // RemoveAllLights

	void Init();
	void Refresh();
	void SetTemperatureUnit(byte unit);
	void StandardLights(byte LightsRelay, byte OnHour, byte OnMinute, byte OffHour, byte OffMinute);
	void MHLights(byte LightsRelay, byte OnHour, byte OnMinute, byte OffHour, byte OffMinute, byte MHDelay);
	void StandardHeater(byte HeaterRelay, int LowTemp, int HighTemp);
	void StandardFan(byte FanRelay, int LowTemp, int HighTemp);
	void StandardATO(byte ATORelay, int ATOTimeout);
	void SingleATO(bool bLow, byte ATORelay, byte byteTimeout, byte byteHrInterval);
	void DosingPump(byte DPRelay, byte DPTimer, byte OnHour, byte OnMinute, int RunTime);
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
	void Wavemaker1(byte WMRelay);
	//void Wavemaker1Setup(byte Relay);
	void Wavemaker2(byte WMRelay);
	//void Wavemaker2Setup(byte Relay);
#ifdef VersionMenu
	void DisplayVersion();
#endif  // VersionMenu
	void ClearScreen(byte Color);

    // Nested Menu Functions
    void InitMenus();
    void ShowInterface();
    void DisplayMenu();
    void DisplayMenuHeading();
    void DisplayMenuEntry(char *text);
    void FeedingMode();
    void WaterChangeMode();
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

	// Nested Menu variables
	int menusptr[Total_Menus];
	byte menuqtysptr[Total_Menus];
	byte DisplayedMenu;
	byte PreviousMenu;
	bool redrawmenu;

#ifdef WavemakerSetup
    // TODO find a better way to save the wavemaker ports for restarting once timers are updated from setup screen
    byte WM1Port;
    byte WM2Port;
#endif  // WavemakerSetup
};

#ifdef wifi
void WebResponse (const prog_uchar *response, long strsize);
void printP(const prog_uchar *str);
void PROGMEMprint(const prog_uchar str[]);
#endif


extern ReefAngelClass ReefAngel;  // make an instance for the user

#endif

