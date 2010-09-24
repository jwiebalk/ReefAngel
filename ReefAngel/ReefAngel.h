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

#define ReefAngel_Version "0.8.3.1"

#define wifi
#include <WProgram.h>
#include <inttypes.h>
#include <Wire.h>
#include <EEPROM.h>
#include <OneWire.h>
#include <Time.h>
#include <DS1307RTC.h>
#include <NokiaLCD.h>
#include <avr/pgmspace.h>

// I2C Addresses
#define I2CEEPROM1          0x50
#define I2CEEPROM2          0x54
#define I2CClock            0x68
#define I2CExpander1        0x20
#define I2CExpander2        0x21

#define I2CEEPROM2_Main              0     //0-2999
#define I2CEEPROM2_Feedging          9919  //3000-4999
#define I2CEEPROM2_Water_Change      11919  //15300-18104

//Digital I/O
#define ledPin              7
#define tempPin             8
#define actinicPWMPin       9
#define daylightPWMPin      10
#define lowATOPin           11
#define highATOPin          12
#define okPin               13

//Analog I/O
#define VPin                0
#define HPin                1
#define PHPin               6
#define Piezo               16

//EEProm Pointers
#define PH_Min		        949
#define PH_Max		        951
#define T1Pointer			953

#define KeyPressRate		250

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

const prog_uchar SERVER_RA[] PROGMEM = "<html><head><script language='javascript' src='http://www.reefangel.com/wifi/ra.js'></script><link href='http://www.reefangel.com/wifi/ra.css' rel='stylesheet' type='text/css'></head><body onLoad='t();'>"
"<table><tr><td colspan=4 align=center><img src='http://www.reefangel.com/images/ra.jpg'></td></tr><tr>"
"<td background='http://www.reefangel.com/images/t1.jpg' align=center><div id=T1 class=style1></div></td>"
"<td background='http://www.reefangel.com/images/t2.jpg' align=center><div id=T2 class=style1></div></td>"
"<td background='http://www.reefangel.com/images/t3.jpg' align=center><div id=T3 class=style1></div></td>"
"<td background='http://www.reefangel.com/images/ph.jpg' align=center><div id=PH class=style1></div></td>"
"</tr><tr><td height='184' colspan=4 align=center><table width='500'><tr><td height='184' background='http://www.reefangel.com/images/panel.jpg'><table width='400' class='style3'><tr class='style2'>"
"<td><div id='l1'></div></td><td><div id='l2'></div></td><td><div id='l3'></div></td><td><div id='l4'></div></td><td><div id='l5'></div></td><td><div id='l6'></div></td><td><div id='l7'></div></td><td><div id='l8'></div></td>"
"</tr><tr class='style2'>"
"<td><div id='s1'><img src='http://www.reefangel.com/images/spacer.gif' width=41 height=85 border='0' usemap='#s1Map'></div></td>"
"<td><div id='s2'><img src='http://www.reefangel.com/images/spacer.gif' width=41 height=85 border='0' usemap='#s2Map'></div></td>"
"<td><div id='s3'><img src='http://www.reefangel.com/images/spacer.gif' width=41 height=85 border='0' usemap='#s3Map'></div></td>"
"<td><div id='s4'><img src='http://www.reefangel.com/images/spacer.gif' width=41 height=85 border='0' usemap='#s4Map'></div></td>"
"<td><div id='s5'><img src='http://www.reefangel.com/images/spacer.gif' width=41 height=85 border='0' usemap='#s5Map'></div></td>"
"<td><div id='s6'><img src='http://www.reefangel.com/images/spacer.gif' width=41 height=85 border='0' usemap='#s6Map'></div></td>"
"<td><div id='s7'><img src='http://www.reefangel.com/images/spacer.gif' width=41 height=85 border='0' usemap='#s7Map'></div></td>"
"<td><div id='s8'><img src='http://www.reefangel.com/images/spacer.gif' width=41 height=85 border='0' usemap='#s8Map'></div></td>"
"</tr></table></td></tr><tr><td><div id='temp'><H1>Please Wait... Loading...</H1></div></td></tr></table></td></tr></table>"
"<map name='s1Map'><area shape='rect' coords='0,0,41,15' href=\"javascript:r('11');\"><area shape='rect' coords='0,33,41,53' href=\"javascript:r('10');\"><area shape='rect' coords='0,70,41,85' href=\"javascript:r('12');\"></map>"
"<map name='s2Map'><area shape='rect' coords='0,0,41,15' href=\"javascript:r('21');\"><area shape='rect' coords='0,33,41,53' href=\"javascript:r('20');\"><area shape='rect' coords='0,70,41,85' href=\"javascript:r('22');\"></map>"
"<map name='s3Map'><area shape='rect' coords='0,0,41,15' href=\"javascript:r('31');\"><area shape='rect' coords='0,33,41,53' href=\"javascript:r('30');\"><area shape='rect' coords='0,70,41,85' href=\"javascript:r('32');\"></map>"
"<map name='s4Map'><area shape='rect' coords='0,0,41,15' href=\"javascript:r('41');\"><area shape='rect' coords='0,33,41,53' href=\"javascript:r('40');\"><area shape='rect' coords='0,70,41,85' href=\"javascript:r('42');\"></map>"
"<map name='s5Map'><area shape='rect' coords='0,0,41,15' href=\"javascript:r('51');\"><area shape='rect' coords='0,33,41,53' href=\"javascript:r('50');\"><area shape='rect' coords='0,70,41,85' href=\"javascript:r('52');\"></map>"
"<map name='s6Map'><area shape='rect' coords='0,0,41,15' href=\"javascript:r('61');\"><area shape='rect' coords='0,33,41,53' href=\"javascript:r('60');\"><area shape='rect' coords='0,70,41,85' href=\"javascript:r('62');\"></map>"
"<map name='s7Map'><area shape='rect' coords='0,0,41,15' href=\"javascript:r('71');\"><area shape='rect' coords='0,33,41,53' href=\"javascript:r('70');\"><area shape='rect' coords='0,70,41,85' href=\"javascript:r('72');\"></map>"
"<map name='s8Map'><area shape='rect' coords='0,0,41,15' href=\"javascript:r('81');\"><area shape='rect' coords='0,33,41,53' href=\"javascript:r('80');\"><area shape='rect' coords='0,70,41,85' href=\"javascript:r('82');\"></map>"
"</body></html>";

static char m_pushback[32];
static char m_pushbackDepth=0;
static byte reqtype=0;
static unsigned long timeout;
#endif

#define SIZE(array) (sizeof(array) / sizeof(*array))


template <class T> int EEPROM_writeAnything(int ee, const T& value);
template <class T> int EEPROM_readAnything(int ee, T& value);

static byte ButtonPress=0;
static unsigned long ButtonDebounce;
static OneWire ds(tempPin);
static byte oldtick;
static unsigned long RAStart;
static bool conn=false;

class JoystickClass
{

public:
	JoystickClass();
	void Init();
	bool IsButtonPressed();
	bool IsUp();
	bool IsDown();
	bool IsRight();
	bool IsLeft();

private:
	void JoystickCenter();
	int CalV, CalH;
	unsigned long KeyKeep;
	byte KeyTurbo;
	byte KeyCount;
};

class LEDClass
{

public:
	LEDClass();
	void On();
	void Off();

private:

};

class ATOClass
{
public:
	ATOClass();
	bool IsLowActive();
	bool IsHighActive();
	bool topping;
	unsigned long ATOTimer;
private:

};

class TempSensorClass
{
public:
	TempSensorClass();
	void Init();
	void RequestConvertion();
	int ReadTemperature(byte addr[8], byte unit);
	byte addrT1[8];
	byte addrT2[8];
	byte addrT3[8];

private:
};

class MemoryClass
{
public:
	MemoryClass();
	byte Read(unsigned int address);
	void Write(unsigned int address, byte data);

private:

};

class RelayClass
{
public:
	RelayClass();
	void On(byte ID);
	void Off(byte ID);
	void AllOn();
	void AllOff();
	void Toggle(byte ID);
	byte RelayData;
	byte RelayMaskOn;
	byte RelayMaskOff;
	void Write();

private:

};

class PWMClass
{
public:
	PWMClass();
	void SetActinic(byte value);
	void SetDaylight(byte value);
	byte ActinicPWMValue;
	byte DaylightPWMValue;
private:

};

class TimerClass
{
public:
	TimerClass();
	time_t Trigger;
	time_t Interval;
	void Start();
	bool IsTriggered();
private:
};

class ReefAngelClass
{

public:
	int PHMin,PHMax;
	ParamsStruct Params;
	ReefAngelClass();
	NokiaLCD LCD;
	JoystickClass Joystick;
	LEDClass LED;
	DS1307RTC RTC;
	ATOClass ATO;
	TempSensorClass TempSensor;
	MemoryClass Memory;
	RelayClass Relay;
	PWMClass PWM;
	TimerClass Timer[6];
	PROGMEM const char *menu_items;
	void Init();
	void Refresh();
	void LoadMenu(int pointer, byte qty);
	void SetTemperatureUnit(byte unit);
	void StandardGUI();
	void FeedingModeGUI();
	void WaterChangeModeGUI();
	void ReturnMenuFunction();
	void StandardLights(byte LightsRelay, byte OnHour, byte OnMinute, byte OffHour, byte OffMinute);
	void MHLights(byte LightsRelay, byte OnHour, byte OnMinute, byte OffHour, byte OffMinute, byte MHDelay);
	void StandardHeater(byte HeaterRelay, int LowTemp, int HighTemp);
	void StandardFan(byte FanRelay, int LowTemp, int HighTemp);
	void StandardATO(byte ATORelay, int ATOTimeout);
	char *Version();
	void DisplayVersion();
	void SetupCalibratePH();
	byte SelectedMenu;

private:
	byte TempUnit;
	int menupointer;
	byte menuqty;
	bool showmenu;
	byte selmenu;
	time_t menutimeout;
};

#ifdef wifi
void WebResponse (const prog_uchar *response, long strsize);
void printP(const prog_uchar *str);
void PROGMEMprint(const prog_uchar str[]);
#endif

byte intlength(int intin);
int NumMins(uint8_t ScheduleHour, uint8_t ScheduleMinute);


extern ReefAngelClass ReefAngel;  // make an instance for the user

#endif

