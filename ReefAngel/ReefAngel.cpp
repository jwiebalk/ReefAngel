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
-----------------------------------------------------------------------------------------------------------------------------
Contribution from Curt Binder
06/24/2010
ReefAngel::DisplayVersion();
-----------------------------------------------------------------------------------------------------------------------------
*/

/*
06/24/2010 - RI - Changed RTC sync to every hour
*/

/*
6/28/2010 - Curt Binder - Added DosingPump function
*/

#include <ReefAngel_Globals.h>
#include <Wire.h>
#include <DS1307RTC.h>
#include "ReefAngel.h"

byte ButtonPress = 0;

SIGNAL(PCINT0_vect) {
	if (millis()-ButtonDebounce>600)
	{
		ButtonDebounce=millis();
		ButtonPress++;
	}

}

template <class T> int EEPROM_writeAnything(int ee, const T& value)
{
    const byte* p = (const byte*)(const void*)&value;
    int i;
    for (i = 0; i < sizeof(value); i++)
	  EEPROM.write(ee++, *p++);
    return i;
}

template <class T> int EEPROM_readAnything(int ee, T& value)
{
    byte* p = (byte*)(void*)&value;
    int i;
    for (i = 0; i < sizeof(value); i++)
	  *p++ = EEPROM.read(ee++);
    return i;
}

// NOTE for nested menus
// Associate a menu name to a numeric value
// the total number must match the max number of menus
enum Menus {
    MainMenu,
    OverrideMenu,
    ClearMenu,
    SetupMenu,
    LightsMenu,
    TempsMenu,
    TimeoutsMenu
};

// Main Menu
prog_char mainmenu_0_label[] PROGMEM = "Feeding Mode";
prog_char mainmenu_1_label[] PROGMEM = "Water Change Mode";
prog_char mainmenu_2_label[] PROGMEM = "Override ->";
prog_char mainmenu_3_label[] PROGMEM = "Clear ->";
prog_char mainmenu_4_label[] PROGMEM = "Setup ->";
prog_char mainmenu_5_label[] PROGMEM = "Version";
PROGMEM const char *mainmenu_items[] = {mainmenu_0_label, mainmenu_1_label, mainmenu_2_label, mainmenu_3_label, mainmenu_4_label, mainmenu_5_label/*, mainmenu_6_label*/};
enum MainMenuItem {
    MainMenu_FeedingMode,
    MainMenu_WaterChangeMode,
    MainMenu_Override,
    MainMenu_Clear,
    MainMenu_Setup,
    MainMenu_Version
};

// Override Menu
prog_char overridemenu_0_label[] PROGMEM = "Lights On";
prog_char overridemenu_1_label[] PROGMEM = "Lights Off";
PROGMEM const char *overridemenu_items[] = {overridemenu_0_label, overridemenu_1_label};
enum OverrideMenuItem {
    OverrideMenu_LightsOn,
    OverrideMenu_LightsOff
};

// Clear Menu
prog_char clearmenu_0_label[] PROGMEM = "ATO Timeout";
prog_char clearmenu_1_label[] PROGMEM = "Overheat";
PROGMEM const char *clearmenu_items[] = {clearmenu_0_label, clearmenu_1_label};
enum ClearMenuItem {
    ClearMenu_ATOTimeout,
    ClearMenu_Overheat
};

// Setup Menu
prog_char setupmenu_0_label[] PROGMEM = "Lights ->";
prog_char setupmenu_1_label[] PROGMEM = "Wavemaker";
prog_char setupmenu_2_label[] PROGMEM = "Temps ->";
prog_char setupmenu_3_label[] PROGMEM = "Timeouts ->";
prog_char setupmenu_4_label[] PROGMEM = "Feeding Timer";
prog_char setupmenu_5_label[] PROGMEM = "Calibrate pH";
//prog_char setupmenu_6_label[] PROGMEM = "Assign Ports";
prog_char setupmenu_6_label[] PROGMEM = "Date & Time";
//PROGMEM const char *setupmenu_items[] = {setupmenu_0_label, setupmenu_1_label, setupmenu_2_label, setupmenu_3_label,
//                                         setupmenu_4_label, setupmenu_5_label, setupmenu_6_label, setupmenu_7_label};
PROGMEM const char *setupmenu_items[] = {setupmenu_0_label, setupmenu_1_label, setupmenu_2_label, setupmenu_3_label,
                                         setupmenu_4_label, setupmenu_5_label, setupmenu_6_label};
enum SetupMenuItem {
    SetupMenu_Lights,
    SetupMenu_Wavemaker,
    SetupMenu_Temps,
    SetupMenu_Timeouts,
    SetupMenu_FeedingTimer,
    SetupMenu_CalibratePH,
    //SetupMenu_AssignPorts,
    SetupMenu_DateTime
};

// Lights Menu
prog_char lightsmenu_0_label[] PROGMEM = "Metal Halides";
prog_char lightsmenu_1_label[] PROGMEM = "Standard Lights";
prog_char lightsmenu_2_label[] PROGMEM = "LED PWM";
PROGMEM const char *lightsmenu_items[] = {lightsmenu_0_label, lightsmenu_1_label, lightsmenu_2_label};
enum LightsMenuItem {
    LightsMenu_MetalHalides,
    LightsMenu_StandardLights,
    LightsMenu_LEDPWM
};

// Temps Menu
prog_char tempsmenu_0_label[] PROGMEM = "Heater";
prog_char tempsmenu_1_label[] PROGMEM = "Fan / Chiller";
prog_char tempsmenu_2_label[] PROGMEM = "Overheat";
PROGMEM const char *tempsmenu_items[] = {tempsmenu_0_label, tempsmenu_1_label, tempsmenu_2_label};
enum TempsMenuItem {
    TempsMenu_Heater,
    TempsMenu_FanChiller,
    TempsMenu_Overheat
};

// Timeouts Menu
prog_char timeoutsmenu_0_label[] PROGMEM = "Auto Top Off";
prog_char timeoutsmenu_1_label[] PROGMEM = "Screensaver";
PROGMEM const char *timeoutsmenu_items[] = {timeoutsmenu_0_label, timeoutsmenu_1_label};
enum TimeoutsMenuItem {
    TimeoutsMenu_AutoTopOff,
    TimeoutsMenu_Screensaver
};

ReefAngelClass::ReefAngelClass()
{
	PCMSK0 |= 32;
	PCICR |= 1;
	oldtick=0;

	// Initialize some variables
	TempUnit = DEGREE_F;
}

void ReefAngelClass::Init()
{
	byte taddr;
	byte t = 0;

	Wire.begin();
	Serial.begin(57600);
	pinMode(Piezo, OUTPUT);
	digitalWrite(lowATOPin,HIGH); //pull up resistor on lowATOPin
	digitalWrite(highATOPin,HIGH); //pull up resistor on highATOPin
	LCD.Init();  // NOTE consider removing LCD Init because it gets handled in constructor
	Joystick.Init();  // NOTE consider removing Joystick Init because it gets handled in constructor
	TempSensor.Init(); // NOTE consider removing TempSensor Init because it gets handled in constructor
	setSyncProvider(RTC.get);   // the function to get the time from the RTC
	setSyncInterval(SECS_PER_HOUR);  // Changed to sync every hour.
	now();
	LCD.BacklightOn();
	Relay.AllOff();
	//EEPROM_writeAnything(PH_Max,900); // 750=PH10.0
	//EEPROM_writeAnything(PH_Min,540); // 480=PH7.0
	EEPROM_readAnything(PH_Min,PHMin);
	EEPROM_readAnything(PH_Max,PHMax);
	taddr = 0;
	EEPROM_readAnything(T1Pointer,taddr);
	if (taddr>120 || taddr<0) EEPROM_writeAnything(T1Pointer,t);

	Timer[0].SetInterval(900); // Default Feeding timer
	Timer[3].SetInterval(600); // set timer to x seconds - Timer 3 is used for sleep mode
	Timer[3].Start();  // start timer
	Timer[5].SetInterval(720);  // Store Params
	Timer[5].ForceTrigger();

	conn = false;
	showmenu = false;

    // Initialize the Nested Menus
    InitMenus();
}

void ReefAngelClass::Refresh()
{
	int tRead;
	now();

#ifdef wifi
  if (Serial.available()>0)
  {
	timeout=millis();
	conn=true;
	memcpy(&m_pushback[0], &m_pushback[1], 31);
	m_pushback[30]=Serial.read();
	m_pushback[31]=0;
	if (strncmp("GET /", m_pushback, 5)==0) reqtype=1;
	if (strncmp("GET /wifi", m_pushback, 9)==0) reqtype=2;
	if (strncmp("GET /r", m_pushback, 6)==0) reqtype=3;
	if (reqtype!=0)
	{
	Serial.flush();
	m_pushback[0]=0;
	if (reqtype==1)
	{
	  P(WebBodyMsg) = SERVER_DEFAULT;
	  WebResponse(WebBodyMsg, sizeof(WebBodyMsg) - 1);
	}
	if (reqtype==2)
	{
	  P(WebBodyMsg) = SERVER_HEADER_HTML;
	  printP(WebBodyMsg);
	  Serial.print(sizeof(SERVER_RA) - 1,DEC);
	  P(WebBodyMsg1) = SERVER_HEADER3;
	  printP(WebBodyMsg1);
	  PROGMEMprint(SERVER_RA);
	}
	if (reqtype==3)
	{
		byte o_relay=m_pushback[6]-'0';
		byte o_type=m_pushback[7]-'0';
		if (o_type==0)
		{
		  bitClear(Relay.RelayMaskOn,o_relay-1);
		  bitClear(Relay.RelayMaskOff,o_relay-1);
		}
		if (o_type==1)
		{
		  bitSet(Relay.RelayMaskOn,o_relay-1);
		  bitSet(Relay.RelayMaskOff,o_relay-1);
		}
		if (o_type==2)
		{
		  bitClear(Relay.RelayMaskOn,o_relay-1);
		  bitSet(Relay.RelayMaskOff,o_relay-1);
		}
		Relay.Write();
	  char temp[6];
	  int s=76;

	  s+=intlength(Params.Temp1);
	  s+=intlength(Params.Temp2);
	  s+=intlength(Params.Temp3);
	  s+=intlength(Params.PH);
	  s+=intlength(Relay.RelayData);
	  s+=intlength(Relay.RelayMaskOn);
	  s+=intlength(Relay.RelayMaskOff);
	  P(WebBodyMsg) = SERVER_HEADER_XML;
	  printP(WebBodyMsg);
	  Serial.print(s);
	  P(WebBodyMsg1) = SERVER_HEADER3;
	  printP(WebBodyMsg1);
	  Serial.print("<RA><T1>");
	  Serial.print(Params.Temp1);
	  Serial.print("</T1><T2>");
	  Serial.print(Params.Temp2);
	  Serial.print("</T2><T3>");
	  Serial.print(Params.Temp3);
	  Serial.print("</T3><PH>");
	  Serial.print(Params.PH);
	  Serial.print("</PH><R>");
	  Serial.print(Relay.RelayData,DEC);
	  Serial.print("</R><RON>");
	  Serial.print(Relay.RelayMaskOn,DEC);
	  Serial.print("</RON><ROFF>");
	  Serial.print(Relay.RelayMaskOff,DEC);
	  Serial.print("</ROFF></RA>");
	}
	reqtype=0;
	conn=false;
	Serial.flush();
	}

  }
  else
  {
    if (conn && millis()-timeout>1000) conn=false;
  }

#endif

	if (ds.read_bit()==0) return;  // ds for OneWire TempSensor
	LCD.Clear(COLOR_WHITE,0,0,1,1);
	Params.Temp1=TempSensor.ReadTemperature(TempSensor.addrT1,TempUnit);
	LCD.Clear(COLOR_WHITE,0,0,1,1);
	Params.Temp2=TempSensor.ReadTemperature(TempSensor.addrT2,TempUnit);
	LCD.Clear(COLOR_WHITE,0,0,1,1);
	Params.Temp3=TempSensor.ReadTemperature(TempSensor.addrT3,TempUnit);
	LCD.Clear(COLOR_WHITE,0,0,1,1);
	Params.PH=analogRead(PHPin);
	Params.PH=map(Params.PH, PHMin, PHMax, 700, 1000); // apply the calibration to the sensor reading
	LCD.Clear(COLOR_WHITE,0,0,1,1);
	TempSensor.RequestConvertion();
	LCD.Clear(COLOR_WHITE,0,0,1,1);
}

void ReefAngelClass::SetTemperatureUnit(byte unit)
{
    // 0 (or DEGREE_F) for farenheit
    // 1 (or DEGREE_C) for celcius
	TempUnit = unit;
}

void ReefAngelClass::StandardLights(byte LightsRelay, byte OnHour, byte OnMinute, byte OffHour, byte OffMinute)
{
	/*
	if (ScheduleTime(OffHour,OffMinute,0)>ScheduleTime(OnHour,OnMinute,0))
	{
		if (now() >= ScheduleTime(OnHour,OnMinute,0)) Relay.On(LightsRelay); else Relay.Off(LightsRelay);
		if (now() >= ScheduleTime(OffHour,OffMinute,0)) Relay.Off(LightsRelay);
	}
	else
	{
		if (now() >= ScheduleTime(OffHour,OffMinute,0)) Relay.Off(LightsRelay); else Relay.On(LightsRelay);
		if (now() >= ScheduleTime(OnHour,OnMinute,0)) Relay.On(LightsRelay);
	}
	*/
	if (NumMins(OffHour,OffMinute) > NumMins(OnHour,OnMinute))
	{
		if (NumMins(hour(),minute()) >= NumMins(OnHour,OnMinute)) Relay.On(LightsRelay); else Relay.Off(LightsRelay);
		if (NumMins(hour(),minute()) >= NumMins(OffHour,OffMinute)) Relay.Off(LightsRelay);
	}
	else
	{
		if (NumMins(hour(),minute()) >= NumMins(OffHour,OffMinute)) Relay.Off(LightsRelay); else Relay.On(LightsRelay);
		if (NumMins(hour(),minute()) >= NumMins(OnHour,OnMinute)) Relay.On(LightsRelay);
	}
}

void ReefAngelClass::MHLights(byte LightsRelay, byte OnHour, byte OnMinute, byte OffHour, byte OffMinute, byte MHDelay)
{
    unsigned long MHTimer = MHDelay;
    MHTimer *= SECS_PER_MIN;
    MHTimer *= 1000;
    if ( millis() > MHTimer )
        StandardLights(LightsRelay, OnHour, OnMinute, OffHour, OffMinute);
}

void ReefAngelClass::StandardHeater(byte HeaterRelay, int LowTemp, int HighTemp)
{
  if (Params.Temp1 <= LowTemp) Relay.On(HeaterRelay);  // If sensor 1 temperature <= LowTemp - turn on heater
  if (Params.Temp1 >= HighTemp) Relay.Off(HeaterRelay);  // If sensor 1 temperature >= HighTemp - turn off heater
}

void ReefAngelClass::StandardFan(byte FanRelay, int LowTemp, int HighTemp)
{
  if (Params.Temp1 >= HighTemp) Relay.On(FanRelay);  // If sensor 1 temperature >= HighTemp - turn on fan
  if (Params.Temp1 <= LowTemp) Relay.Off(FanRelay);  // If sensor 1 temperature <= LowTemp - turn off fan
}

void ReefAngelClass::StandardATO(byte ATORelay, int ATOTimeout)
{
    // Input:  Relay port and timeout value (max number of seconds that ATO pump is allowed to run)
	unsigned long TempTimeout = ATOTimeout;
	TempTimeout *= 1000;

	/*
	Is the low switch active (meaning we need to top off) and are we not currently topping off
	Then we set the timer to be now and start the topping pump
	*/
	// TODO remove next line
    //if (ATO.IsLowActive() && ATO.topping==false)
    if ( ATO.IsLowActive() && (!ATO.IsTopping()) )
    {
        ATO.ATOTimer = millis();
        ATO.StartTopping();
        Relay.On(ATORelay);
    }

    // If the high switch is activated, this is a safeguard to prevent over running of the top off pump
    if (ATO.IsHighActive())
    {
		ATO.StopTopping();
		Relay.Off(ATORelay);
    }

    /*
    If the current time minus the start time of the ATO pump is greater than the specified timeout value
    AND the ATO pump is currently running:
    We turn on the status LED and shut off the ATO pump
    This prevents the ATO pump from contniously running.
    */
	if ( (millis()-ATO.ATOTimer > TempTimeout) && ATO.IsTopping() )
	{
		LED.On();
		Relay.Off(ATORelay);
	}
}

void ReefAngelClass::DosingPump(byte DPRelay, byte DPTimer, byte OnHour, byte OnMinute, time_t RunTime)
{
    /*
    This function configures and sets up the dosing pump and turns it on at the appropriate time
    Once the timer has expired for the dosing pump, it shuts it off

    DPRelay - relay dosing pump is plugged into (0-8)
    Timer - timer to control dosing pump
    OnHour & OnMinute - time to turn on the dosing pump (in 24hr based time)
    RunTime - duration to run the pump
    */

    // Let's see if it's supposed to start running the timer now
    if ( (NumMins(hour(), minute()) == NumMins(OnHour, OnMinute)) && (second() == 0) )
    {
        Relay.On(DPRelay);
        //LED.On();
        Timer[DPTimer].SetInterval(RunTime);
        Timer[DPTimer].Start();
    }

    // is the timer expired?
    if ( Timer[DPTimer].IsTriggered() )
    {
        // timer expired, so let's shut off the pump
        Relay.Off(DPRelay);
        //LED.Off();
    }
}

char *ReefAngelClass::Version()
{
	return ReefAngel_Version;
}

void ReefAngelClass::DisplayVersion()
{
    // Display the Software Version
    LCD.DrawText(COLOR_BLUE,COLOR_WHITE,10,10,"Reef Angel");
    LCD.DrawText(COLOR_BLUE,COLOR_WHITE,10,20,"v"ReefAngel_Version);

#ifdef wifi
    // Display wifi related information
    // Place holder information currently, need wifi module
    // to be able to write functions to retrieve actual information
    LCD.DrawText(COLOR_BLUE,COLOR_WHITE,10,40,"Wifi Enabled");
    LCD.DrawText(COLOR_BLUE,COLOR_WHITE,10,50,"SSID: AP_SSID");
    LCD.DrawText(COLOR_BLUE,COLOR_WHITE,10,60,"IP: 255.255.255.255");
    LCD.DrawText(COLOR_BLUE,COLOR_WHITE,10,70,"Port: 80");
#endif  // wifi
}

void ReefAngelClass::SaveParamsToMemory()
{
    // Save parameters to memory and redraw the graph
    int a = EEPROM.read(T1Pointer);
    int CurTemp;

    a++;
    if (a>=120) a=0;
    Timer[5].Start();
    CurTemp = map(Params.Temp1, 700, 900, 0, 50); // apply the calibration to the sensor reading
    CurTemp = constrain(CurTemp, 0, 50); // in case the sensor value is outside the range seen during calibration
    LCD.Clear(COLOR_WHITE,0,0,1,1);
    Memory.Write(a, CurTemp);
    LCD.Clear(COLOR_WHITE,0,0,1,1);
    CurTemp = map(Params.Temp2, 650, 1500, 0, 50); // apply the calibration to the sensor reading
    CurTemp = constrain(CurTemp, 0, 50); // in case the sensor value is outside the range seen during calibration
    LCD.Clear(COLOR_WHITE,0,0,1,1);
    Memory.Write(a+120, CurTemp);
    LCD.Clear(COLOR_WHITE,0,0,1,1);
    CurTemp = map(Params.Temp3, 650, 920, 0, 50); // apply the calibration to the sensor reading
    CurTemp = constrain(CurTemp, 0, 50); // in case the sensor value is outside the range seen during calibration
    LCD.Clear(COLOR_WHITE,0,0,1,1);
    Memory.Write(a+240, CurTemp);
    LCD.Clear(COLOR_WHITE,0,0,1,1);
    CurTemp = map(Params.PH, 730, 890, 0, 50); // apply the calibration to the sensor reading
    CurTemp = constrain(CurTemp, 0, 50); // in case the sensor value is outside the range seen during calibration
    LCD.Clear(COLOR_WHITE,0,0,1,1);
    Memory.Write(a+360, CurTemp);
    LCD.Clear(COLOR_WHITE,0,0,1,1);
    EEPROM.write(T1Pointer,a);
    LCD.DrawGraph(5, 5, I2CEEPROM1, T1Pointer);
}

void ReefAngelClass::ClearScreen(byte Color)
{
    // clears the entire screen
    LCD.Clear(Color, 0, 0, 131, 131);
}

bool ReefAngelClass::IsTempOverheat(int temp)
{
    // TODO use overheat value from memory
    if ( temp >= 1500)  // if temp >= 150.0F
    {
        return true;
    }
    return false;
}

void ReefAngelClass::DisplayHomeScreen()
{
    // display everything on the home screen except the graph
    // the graph is drawn/updated when we exit the main menu & when the parameters are saved
    LCD.DrawDate(6, 112);
    LCD.DrawMonitor(15, 60, Params, PWM.GetDaylightValue(), PWM.GetActinicValue());
    byte TempRelay = Relay.RelayData;
    TempRelay &= Relay.RelayMaskOff;
    TempRelay |= Relay.RelayMaskOn;
    LCD.DrawOutletBox(12, 93, TempRelay);
}

void ReefAngelClass::InitMenus()
{
    // load the menus
    LoadAllMenus();

    // initialize menus
    DisplayedMenu = DEFAULT_MENU;  // default menu to display
    SelectedMenuItem = DEFAULT_MENU_ITEM;  // default item to have selected
    redraw = true;
    showmenu = false;  // initially we are showing the main graphic and no menu
}

void ReefAngelClass::LoadAllMenus()
{
    // loads all the menus
    menusptr[MainMenu] = pgm_read_word(&(mainmenu_items[0]));
    menuqtysptr[MainMenu] = SIZE(mainmenu_items);
    menusptr[OverrideMenu] = pgm_read_word(&(overridemenu_items[0]));
    menuqtysptr[OverrideMenu] = SIZE(overridemenu_items);
    menusptr[ClearMenu] = pgm_read_word(&(clearmenu_items[0]));
    menuqtysptr[ClearMenu] = SIZE(clearmenu_items);
    menusptr[SetupMenu] = pgm_read_word(&(setupmenu_items[0]));
    menuqtysptr[SetupMenu] = SIZE(setupmenu_items);
    menusptr[LightsMenu] = pgm_read_word(&(lightsmenu_items[0]));
    menuqtysptr[LightsMenu] = SIZE(lightsmenu_items);
    menusptr[TempsMenu] = pgm_read_word(&(tempsmenu_items[0]));
    menuqtysptr[TempsMenu] = SIZE(tempsmenu_items);
    menusptr[TimeoutsMenu] = pgm_read_word(&(timeoutsmenu_items[0]));
    menuqtysptr[TimeoutsMenu] = SIZE(timeoutsmenu_items);
}

void ReefAngelClass::ShowInterface()
{
    // Let's mimic StandardGUI()
    Refresh();

    if ( conn )
    {
        // do something special when there is a connection
    }
    else
    {
        // are we displaying the menu or not??
        if ( showmenu )
        {
            // showing the menu
            DisplayMenuHeading(DisplayedMenu);
            DisplayMenu(DisplayedMenu);
        }
        else
        {
            // not displaying the menu, so we're gonna show the default screen
            if ( DisplayedMenu == DEFAULT_MENU )
            {
                // process screensaver timeout
                if ( Timer[3].IsTriggered() )
                {
                    // Screensaver timeout expired
                    LCD.BacklightOff();
                }

                if ( Joystick.IsButtonPressed() )
                {
                    // turn the backlight on
                    LCD.BacklightOn();

                    // TODO check Timer[3] code
                    if ( Timer[3].Trigger == 0 )
                    {
                        Timer[3].Start();
                        return;
                    }

                    // Clears the screen to draw the menu
                    // Displays main menu, select first item, save existing menu
                    ClearScreen(COLOR_WHITE);
                    SelectedMenuItem = DEFAULT_MENU_ITEM;
                    SPreviousMenu.Push(DisplayedMenu);
                    DisplayedMenu = MAIN_MENU;
                    showmenu = true;
                    redraw = true;
                    menutimeout = now();
                    // get out of this function and display the menu
                    return;
                }

                if ( Joystick.IsUp() || Joystick.IsDown() || Joystick.IsRight() || Joystick.IsLeft() )
                {
                    // Turn backlight on
                    LCD.BacklightOn();
                    Timer[3].Start();
                }

                DisplayHomeScreen();

                // Process any checks/tests/events that can happen while displaying the home screen
                // This can be the timers for wavemakers or any overheat temperatures

                // process timers
                if ( Timer[5].IsTriggered() )
                {
                    SaveParamsToMemory();
                }

                // wavemaker 1 timer
                if ( Timer[1].IsTriggered() )  // If timer 1 expires
                {
                    Timer[1].Start();  // start timer
                    Relay.Toggle(Port4);  // toggle relay
                }
                // wavemaker 2 timer
                if ( Timer[2].IsTriggered() )  // If timer 2 expires
                {
                    Timer[2].Start();  // start timer
                    Relay.Toggle(Port5);  // toggle relay
                }

                // if temp2 exceeds overheat temp
                if ( IsTempOverheat(Params.Temp2) )
                {
                    // turn off MH lights (port 3)
                    LED.On();
                    Relay.RelayMaskOff = B11111011;
                }
                // commit relay changes
                Relay.Write();
            }
            else
            {
                // we are viewing another screen
                if ( Joystick.IsButtonPressed() )
                {
                    // button is pressed, so we gotta exit out, show the menu & redraw it too
                    redraw = true;
                    showmenu = true;
                    Timer[0].ForceTrigger();
                    Timer[3].Start();
                }
            }  // if DisplayedMenu == DEFAULT_MENU
        }  // if showmenu
    }  // if conn

}

void ReefAngelClass::DisplayMenu(byte MenuNum)
{
    int i;
    byte bcolor, fcolor;
    int qty = menuqtysptr[MenuNum];
    int ptr = menusptr[MenuNum];

    if ( Joystick.IsUp() )
    {
        // process UP press
        if ( --SelectedMenuItem == DEFAULT_MENU )
        {
            // we're moving up and we hit the top of the list
            // gotta wrap down to the bottom of the list
            // qty - 1 gives us the last item in our list
            //SelectedMenuItem = qty - 1;
            // This allows us to add in our last item
            SelectedMenuItem = qty;
        }
        redraw = true;
        menutimeout = now();
    }

    if ( Joystick.IsDown() )
    {
        // process DOWN press
        // > allows for selection of last item, >= skips it
        if ( ++SelectedMenuItem > qty )
        {
            // we've hit the bottom of the list
            // wrap around to the top of the list
            SelectedMenuItem = DEFAULT_MENU_ITEM;
        }
        redraw = true;
        menutimeout = now();
    }

    // TODO Have ability to customize menu timeout delay
    if ( (now() - menutimeout) > 10 )
    {
        // menu timeout returns to the main screen
        // skip all the other menu checks
        SelectedMenuItem = EXCEED_TIMEOUT_MENU;
        DisplayedMenu = EXCEED_TIMEOUT_MENU;
        ButtonPress++;
    }

    if ( Joystick.IsButtonPressed() )
    {
        // button gets pressed, so we need to handle the button press
        ProcessButtonPress(SelectedMenuItem);
        redraw = true;
        // Don't finish processing the rest of the menu
        return;
    }

    // don't redraw the menu if we don't have to
    if ( ! redraw )
        return;

    char buffer[22];
    int x2, y2;
    for ( i = 0; i <= qty; i++ )
    {
        bcolor = COLOR_WHITE;
        fcolor = COLOR_BLACK;
        if ( i < qty )
        {
            strcpy_P(buffer, (char *)ptr++);
        }
        else
        {
            // the last item in the list is either Exit or Prev Menu
            if ( MenuNum == MainMenu )
            {
                strcpy(buffer, "Exit");
            }
            else
            {
                strcpy(buffer, "<- Prev Menu");
            }
        }
        ptr += strlen(buffer);

        // change the background color on the selected menu entry
        if ( i == SelectedMenuItem )
        {
            bcolor = COLOR_BLUE;
            //fcolor = COLOR_WHITE;
        }
        LCD.Clear(bcolor, MENU_START_COL-3,
                         (i*MENU_START_ROW)+MENU_HEADING_SIZE-1,
                          MENU_END_COL,
                         (i*MENU_START_ROW)+(MENU_HEADING_SIZE+MENU_ITEM_HEIGHT-1));
        LCD.DrawText(fcolor, bcolor, MENU_START_COL, (i*MENU_START_ROW)+MENU_HEADING_SIZE, buffer);
    }  // for i
    // once drawn, no need to redraw yet
    redraw = false;
}

void ReefAngelClass::DisplayMenuHeading(byte MenuNum)
{
    // NOTE do we redraw the menu heading or not?  use same logic as with the menu
    if ( ! redraw )
        return;

    char buffer[22];

    switch ( MenuNum )
    {
        default:
//            {
//                //strcpy(buffer, "Menu:");
//                sprintf(buffer, "Menu (%d):", MenuNum);
//            }
//            break;
        case MainMenu:
            {
                strcpy(buffer, "Main:");
            }
            break;
        case OverrideMenu:
            {
                strcpy(buffer, "Override:");
            }
            break;
        case ClearMenu:
            {
                strcpy(buffer, "Clear:");
            }
            break;
        case SetupMenu:
            {
                strcpy(buffer, "Setup:");
            }
            break;
        case LightsMenu:
            {
                strcpy(buffer, "Lights:");
            }
            break;
        case TempsMenu:
            {
                strcpy(buffer, "Temp:");
            }
            break;
        case TimeoutsMenu:
            {
                strcpy(buffer, "Timeouts:");
            }
            break;
    }  // switch MenuNum

    // clear the line that has the menu heading on it
    LCD.Clear(COLOR_WHITE, MENU_START_COL, MENU_START_ROW, MAX_X, MAX_Y);
    // Display the menu heading
    LCD.DrawText(COLOR_BLACK, COLOR_WHITE, MENU_START_COL, MENU_START_ROW, buffer);
}

void ReefAngelClass::DisplayMenuEntry(char *text)
{
    ClearScreen(COLOR_WHITE);
    LCD.DrawText(COLOR_BLACK, COLOR_WHITE, MENU_START_COL, MENU_START_ROW, text);
    LCD.DrawText(COLOR_BLACK, COLOR_WHITE, MENU_START_COL, MENU_START_ROW*4, "Press button");
    LCD.DrawText(COLOR_BLACK, COLOR_WHITE, MENU_START_COL+5, MENU_START_ROW*5, "to exit...");
    do
    {
        delay(500);
    } while ( ! Joystick.IsButtonPressed() );
}

void ReefAngelClass::FeedingMode()
{
	LCD.DrawText(COLOR_BLUE, COLOR_WHITE, 30, 10, "Feeding Mode");
	Timer[0].Start();  //Start Feeding Mode timer
    LCD.DrawEEPromImage(40,50, 40, 30, I2CEEPROM2, I2CEEPROM2_Feeding);

    int t;
    bool bDone = false;
    do
    {
        t = Timer[0].Trigger - now();
        if ( (t >= 0) && ! Timer[0].IsTriggered() )
        {
            LCD.Clear(COLOR_WHITE,60+(intlength(t)*5),100,100,108);
            LCD.DrawText(COLOR_BLACK,COLOR_WHITE,60,100,t);
        }
        else
        {
            bDone = true;
        }

        if ( Joystick.IsButtonPressed() )
        {
            // joystick button pressed, so we stop the feeding mode
            bDone = true;
        }
        delay(200);
    } while ( ! bDone );

    // we're finished, so let's clear the screen and return
    ClearScreen(COLOR_WHITE);
    Timer[3].Start();  // start timer
}

void ReefAngelClass::WaterChangeMode()
{
	LCD.DrawText(COLOR_BLUE, COLOR_WHITE, 20, 10, "Water Change Mode");
	LCD.DrawEEPromImage(51,55, 40, 30, I2CEEPROM2, I2CEEPROM2_Water_Change);
	do
	{
	    // just wait for the button to be pressed
	    delay(200);
	} while ( ! Joystick.IsButtonPressed() );
	ClearScreen(COLOR_WHITE);
}

void ReefAngelClass::ProcessButtonPress(byte smenu)
{
    bool bResetMenuTimeout = true;
    switch ( DisplayedMenu )
    {
        default:  // DEFAULT_MENU == 255
//        {
//            // Default Screen
//            break;
//        }
        case MainMenu:
        {
            ProcessButtonPressMain(smenu);
            break;
        }
        case OverrideMenu:
        {
            ProcessButtonPressOverride(smenu);
            break;
        }
        case ClearMenu:
        {
            ProcessButtonPressClear(smenu);
            break;
        }
        case SetupMenu:
        {
            ProcessButtonPressSetup(smenu);
            break;
        }
        case LightsMenu:
        {
            ProcessButtonPressLights(smenu);
            break;
        }
        case TempsMenu:
        {
            ProcessButtonPressTemps(smenu);
            break;
        }
        case TimeoutsMenu:
        {
            ProcessButtonPressTimeouts(smenu);
            break;
        }
        case EXCEED_TIMEOUT_MENU:
        {
            // we bypass all the other menus when the timeout has exceeded
            // we need to mimic the default action for the main menu in addition to
            // clearing out the stack
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            DisplayedMenu = DEFAULT_MENU;
            showmenu = false;
            SPreviousMenu.Clear();
            ClearScreen(COLOR_WHITE);
            bResetMenuTimeout = false;
            // we are exiting the menu, so draw the graph
            LCD.DrawGraph(5, 5, I2CEEPROM1, T1Pointer);  // Redraw graphic of params
            break;
        }
    }
    redraw = true;
    // if a button was pressed, we have to reset the timeout after processing the press
    if ( bResetMenuTimeout )
    {
        menutimeout = now();
    }
}

void ReefAngelClass::ProcessButtonPressMain(byte smenu)
{
    ClearScreen(COLOR_WHITE);
    switch ( smenu )
    {
        case MainMenu_FeedingMode:
        {
            //DisplayMenuEntry("Feeding Mode");
            // turn off ports
            //byte CurrentRelayState = Relay.RelayData;
            Relay.Off(Sump);
            Relay.Off(Port4);
            Relay.Off(Port5);
            Relay.Write();
            // run feeding mode
            FeedingMode();
            // turn on ports
            Relay.On(Sump);
            Relay.On(Port4);
            Relay.On(Port5);
            // restore ports
            //Relay.RelayData = CurrentRelayState;
            Relay.Write();
            break;
        }
        case MainMenu_WaterChangeMode:
        {
            //DisplayMenuEntry("Water Change Mode");
            // turn off pumps for water change mode
            //byte CurrentRelayState = Relay.RelayData;
            Relay.Off(Sump);
            Relay.Off(Port4);
            Relay.Off(Port5);
            Relay.Write();
            // Display the water change mode
            WaterChangeMode();
            // turn on the pumps after water change mode
            Relay.On(Sump);
            Relay.On(Port4);
            Relay.On(Port5);
            //Relay.RelayData = CurrentRelayState;
            Relay.Write();
            break;
        }
        case MainMenu_Override:
        {
            // switch menus
            // save the old displayed menu and reset the selected item to be the first in the list
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            SPreviousMenu.Push(DisplayedMenu);
            DisplayedMenu = OverrideMenu;
            break;
        }
        case MainMenu_Clear:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            SPreviousMenu.Push(DisplayedMenu);
            DisplayedMenu = ClearMenu;
            break;
        }
        case MainMenu_Setup:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            SPreviousMenu.Push(DisplayedMenu);
            DisplayedMenu = SetupMenu;
            break;
        }
        case MainMenu_Version:
        {
            //DisplayMenuEntry("Version Item");
            DisplayVersion();
            // turn off the menu so we can wait for the button press to exit
            showmenu = false;
            break;
        }
        default:
        {
            // This will be the EXIT choice
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            // switch to the previous menu
            SPreviousMenu.Pop(DisplayedMenu);
            // disable the menu, display main screen
            showmenu = false;
            // When we exit the main menu, we will redraw the graph
            LCD.DrawGraph(5, 5, I2CEEPROM1, T1Pointer);  // Redraw graphic of params
            break;
        }
    }
}

void ReefAngelClass::ProcessButtonPressOverride(byte smenu)
{
    ClearScreen(COLOR_WHITE);
    switch ( smenu )
    {
        case OverrideMenu_LightsOff:
        {
            // Reset ports 2 & 3
            Relay.RelayMaskOn = B00000000;
            // sets PWM to 0%
            PWM.SetActinic(0);
            PWM.SetDaylight(0);
            Relay.Write();
            DisplayMenuEntry("Restore Lights");
            break;
        }
        case OverrideMenu_LightsOn:
        {
            // turns on ports 2 & 3
            Relay.RelayMaskOn = B00000110;
            // sets PWM to 50%
            PWM.SetActinic(50);
            PWM.SetDaylight(50);
            Relay.Write();
            DisplayMenuEntry("Lights On");
            break;
        }
        default:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            // switch to the previous menu
            SPreviousMenu.Pop(DisplayedMenu);
            break;
        }
    }
}

void ReefAngelClass::ProcessButtonPressClear(byte smenu)
{
    ClearScreen(COLOR_WHITE);
    switch ( smenu )
    {
        case ClearMenu_ATOTimeout:
        {
            // Need delay for clearing & returning screen
            LED.Off();
            ATO.StopTopping();
            // If displaymenuentry is called, the functionality stays inside it.
            DisplayMenuEntry("Clear ATO Timeout");
            // if we set showmenu=false, the main program loop executes until a button press is signaled
            break;
        }
        case ClearMenu_Overheat:
        {
            LED.Off();
            Relay.RelayMaskOff = B11111111;  // Override MHlight auto
            Relay.Write();
            DisplayMenuEntry("Clear Overheat");
            break;
        }
        default:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            // switch to the previous menu
            SPreviousMenu.Pop(DisplayedMenu);
            break;
        }
    }
}

void ReefAngelClass::ProcessButtonPressSetup(byte smenu)
{
    ClearScreen(COLOR_WHITE);
    switch ( smenu )
    {
        case SetupMenu_Lights:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            SPreviousMenu.Push(DisplayedMenu);
            DisplayedMenu = LightsMenu;
            break;
        }
        case SetupMenu_Wavemaker:
        {
            DisplayMenuEntry("Setup Wavemaker");
            //SetupWavemakersDisplay();
            break;
        }
        case SetupMenu_Temps:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            SPreviousMenu.Push(DisplayedMenu);
            DisplayedMenu = TempsMenu;
            break;
        }
        case SetupMenu_Timeouts:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            SPreviousMenu.Push(DisplayedMenu);
            DisplayedMenu = TimeoutsMenu;
            break;
        }
        case SetupMenu_FeedingTimer:
        {
            DisplayMenuEntry("Setup Feeding Timer");
            break;
        }
        case SetupMenu_CalibratePH:
        {
            DisplayMenuEntry("Calibrate pH");
            break;
        }
//        case SetupMenu_AssignPorts:
//        {
//            DisplayMenuEntry("Assign Ports");
//            break;
//        }
        case SetupMenu_DateTime:
        {
            DisplayMenuEntry("Set Date/Time");
            break;
        }
        default:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            // switch to the previous menu
            SPreviousMenu.Pop(DisplayedMenu);
            break;
        }
    }
}

void ReefAngelClass::ProcessButtonPressLights(byte smenu)
{
    ClearScreen(COLOR_WHITE);
    switch ( smenu )
    {
        case LightsMenu_MetalHalides:
        {
            DisplayMenuEntry("Metal Halides");
            break;
        }
        case LightsMenu_StandardLights:
        {
            DisplayMenuEntry("Standard Lights");
            break;
        }
        case LightsMenu_LEDPWM:
        {
            DisplayMenuEntry("LED PWM");
            break;
        }
        default:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            // switch to the previous menu
            SPreviousMenu.Pop(DisplayedMenu);
            break;
        }
    }
}

void ReefAngelClass::ProcessButtonPressTemps(byte smenu)
{
    ClearScreen(COLOR_WHITE);
    switch ( smenu )
    {
        case TempsMenu_Heater:
        {
            DisplayMenuEntry("Setup Heater");
            break;
        }
        case TempsMenu_FanChiller:
        {
            DisplayMenuEntry("Setup Fan/Chiller");
            break;
        }
        case TempsMenu_Overheat:
        {
            DisplayMenuEntry("Setup Overheat");
            //SetupOverheatDisplay();
            break;
        }
        default:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            // switch to the previous menu
            SPreviousMenu.Pop(DisplayedMenu);
            break;
        }
    }
}

void ReefAngelClass::ProcessButtonPressTimeouts(byte smenu)
{
    ClearScreen(COLOR_WHITE);
    switch ( smenu )
    {
        case TimeoutsMenu_AutoTopOff:
        {
            DisplayMenuEntry("Setup ATO");
            break;
        }
        case TimeoutsMenu_Screensaver:
        {
            DisplayMenuEntry("Setup Screensaver");
            break;
        }
        default:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            // switch to the previous menu
            SPreviousMenu.Pop(DisplayedMenu);
            break;
        }
    }
}


// Setup Screens
//void ReefAngelClass::SetupWavemakersDisplay()
//{
//    ClearScreen(COLOR_WHITE);
//    LCD.DrawText(COLOR_BLACK, COLOR_WHITE, MENU_START_COL, MENU_START_ROW, "Setup Wavemakers:");
//    // Read wavemaker timer values from memory
//    int wm1 = 600; //InternalMemory.WM1Timer_read();
//    int wm2 = 200; //InternalMemory.WM2Timer_read();
//    bool bSave = false;
//    DisplayWavemaker1(wm1);
//    DisplayWavemaker2(wm2);
//    // Wait for button press to exit
//    do
//    {
//        delay(500);
//    } while ( ! Joystick.IsButtonPressed() );
//    // if OK button pressed, save values to memory
////    if ( bSave )
////    {
////        InternalMemory.WM1Timer_write(wm1);
////        InternalMemory.WM2Timer_write(wm2);
////    }
//}
//
//void ReefAngelClass::DisplayWavemaker1(int t)
//{
//    LCD.DrawText(COLOR_BLACK, COLOR_WHITE, MENU_START_COL, MENU_START_ROW*3, "Wavemaker 1");
//}
//
//void ReefAngelClass::DisplayWavemaker2(int t)
//{
//    LCD.DrawText(COLOR_BLACK, COLOR_WHITE, MENU_START_COL, MENU_START_ROW*4, "Wavemaker 2");
//}
//
//void ReefAngelClass::SetupOverheatDisplay()
//{
//    ClearScreen(COLOR_WHITE);
//    LCD.DrawText(COLOR_BLACK, COLOR_WHITE, MENU_START_COL, MENU_START_ROW, "Setup Overheat:");
//    int o = 1500; //InternalMemory.OverheatTemp_read();
//    bool bSave = false;
//    DisplayOverheat(o);
//    do
//    {
//        delay(500);
//    } while ( ! Joystick.IsButtonPressed() );
////    if ( bSave )
////    {
////        InternalMemory.OverheatTemp_write(o);
////    }
//}
//
//void ReefAngelClass::DisplayOverheat(int t)
//{
//    char buf[32];
//    sprintf(buf, "Overheat: %d", t);
//    LCD.DrawText(COLOR_BLACK, COLOR_WHITE, MENU_START_COL, MENU_START_ROW*3, buf);
//}


#ifdef wifi

void WebResponse (const prog_uchar *response, long strsize)
{
//  P(WebHeaderMsg) = SERVER_HEADER_HTML;
//  printP(WebHeaderMsg);
  P(WebBodyMsg) = SERVER_HEADER_HTML;
  printP(WebBodyMsg);
  Serial.print(strsize,DEC);
  P(WebBodyMsg1) = SERVER_HEADER3;
  printP(WebBodyMsg1);
  printP(response);
}

void printP(const prog_uchar *str)
{
  char a;
  do
  {
    a=pgm_read_byte(str++);
    if (a!=0) Serial.print(a);
  }
  while (a!=0);
}

void PROGMEMprint(const prog_uchar str[])
{
  char c;
  if(!str) return;
  while((c = pgm_read_byte(str++)))
    Serial.print(c,BYTE);
}

#endif



ReefAngelClass ReefAngel = ReefAngelClass() ;
