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

// NOTE for nested menus
// NOTE Menu labels can be a max of 20 characters
// Associate a menu name to a numeric value
// the total number must match the max number of menus
enum Menus {
    MainMenu,
    SetupMenu,
    LightsMenu,
    TempsMenu
#if defined SetupExtras || defined ATOSetup
    ,
    TimeoutsMenu
#endif  // if defined SetupExtras || defined ATOSetup
};

// Main Menu
prog_char mainmenu_0_label[] PROGMEM = "Feeding";
prog_char mainmenu_1_label[] PROGMEM = "Water Change";
prog_char mainmenu_2_label[] PROGMEM = "Lights ->";
prog_char mainmenu_3_label[] PROGMEM = "Temps ->";
#if defined SetupExtras || defined ATOSetup
prog_char mainmenu_4_label[] PROGMEM = "Timeouts ->";
#endif  // if defined SetupExtras || defined ATOSetup
prog_char mainmenu_5_label[] PROGMEM = "Setup ->";
#ifdef VersionMenu
prog_char mainmenu_6_label[] PROGMEM = "Version";
#endif  // VersionMenu
PROGMEM const char *mainmenu_items[] = {
                    mainmenu_0_label,
                    mainmenu_1_label,
                    mainmenu_2_label,
                    mainmenu_3_label,
#if defined SetupExtras || defined ATOSetup
                    mainmenu_4_label,
#endif  // if defined SetupExtras || defined ATOSetup
                    mainmenu_5_label
#ifdef VersionMenu
                    ,
                    mainmenu_6_label
#endif  // VersionMenu
                    };
enum MainMenuItem {
    MainMenu_FeedingMode,
    MainMenu_WaterChangeMode,
    MainMenu_Lights,
    MainMenu_Temps,
#if defined SetupExtras || defined ATOSetup
    MainMenu_Timeouts,
#endif  // if defined SetupExtras || defined ATOSetup
    MainMenu_Setup
#ifdef VersionMenu
    ,
    MainMenu_Version
#endif  // VersionMenu
};


// Setup MenuWavemakerSetup
#ifdef WavemakerSetup
prog_char setupmenu_0_label[] PROGMEM = "Wavemaker";
#endif  // WavemakerSetup
#ifdef DosingPumpSetup
prog_char setupmenu_1_label[] PROGMEM = "Dosing Pump";
#endif  // DosingPumpSetup
prog_char setupmenu_2_label[] PROGMEM = "Calibrate pH";
#ifdef DateTimeSetup
prog_char setupmenu_3_label[] PROGMEM = "Date / Time";
#endif  // DateTimeSetup
PROGMEM const char *setupmenu_items[] = {
#ifdef WavemakerSetup
                    setupmenu_0_label,
#endif  // WavemakerSetup
#ifdef DosingPumpSetup
                    setupmenu_1_label,
#endif  // DosingPumpSetup
                    setupmenu_2_label
#ifdef DateTimeSetup
                    ,
                    setupmenu_3_label
#endif  // DateTimeSetup
                    };
enum SetupMenuItem {
#ifdef WavemakerSetup
    SetupMenu_Wavemaker,
#endif  // WavemakerSetup
#ifdef DosingPumpSetup
    SetupMenu_DosingPump,
#endif  // DosingPumpSetup
    SetupMenu_CalibratePH
#ifdef DateTimeSetup
    ,
    SetupMenu_DateTime
#endif  // DateTimeSetup
};


// Lights Menu
prog_char lightsmenu_0_label[] PROGMEM = "Lights On";
prog_char lightsmenu_1_label[] PROGMEM = "Lights Off";
#ifdef MetalHalideSetup
prog_char lightsmenu_2_label[] PROGMEM = "Metal Halides";
prog_char lightsmenu_3_label[] PROGMEM = "MH On Delay";
#endif  // MetalHalideSetup
prog_char lightsmenu_4_label[] PROGMEM = "Standard Lights";
#ifdef DisplayLEDPWM
prog_char lightsmenu_5_label[] PROGMEM = "LED PWM";
#endif  // DisplayLEDPWM
PROGMEM const char *lightsmenu_items[] = {
                            lightsmenu_0_label, lightsmenu_1_label,
#ifdef MetalHalideSetup
                            lightsmenu_2_label,
                            lightsmenu_3_label,
#endif  // MetalHalideSetup
                            lightsmenu_4_label
#ifdef DisplayLEDPWM
                            ,
                            lightsmenu_5_label
#endif  // DisplayLEDPWM
                            };
enum LightsMenuItem {
    LightsMenu_On,
    LightsMenu_Off,
#ifdef MetalHalideSetup
    LightsMenu_MetalHalides,
    LightsMenu_MetalHalideDelay,
#endif  // MetalHalideSetup
    LightsMenu_StandardLights
#ifdef DisplayLEDPWM
    ,
    LightsMenu_LEDPWM
#endif  // DisplayLEDPWM
};

// Temps Menu
prog_char tempsmenu_0_label[] PROGMEM = "Heater";
prog_char tempsmenu_1_label[] PROGMEM = "Chiller";
#ifdef OverheatSetup
prog_char tempsmenu_2_label[] PROGMEM = "Overheat Set";
#endif  // OverheatSetup
prog_char tempsmenu_3_label[] PROGMEM = "Overheat Clear";
PROGMEM const char *tempsmenu_items[] = {
                        tempsmenu_0_label,
                        tempsmenu_1_label,
#ifdef OverheatSetup
                        tempsmenu_2_label,
#endif  // OverheatSetup
                        tempsmenu_3_label};
enum TempsMenuItem {
    TempsMenu_Heater,
    TempsMenu_Chiller,
#ifdef OverheatSetup
    TempsMenu_Overheat,
#endif  // OverheatSetup
    TempsMenu_OverheatClr
};

// Timeouts Menu
#if defined SetupExtras || defined ATOSetup
#ifdef ATOSetup
prog_char timeoutsmenu_0_label[] PROGMEM = "ATO Set";
prog_char timeoutsmenu_1_label[] PROGMEM = "ATO Clear";
#endif  // ATOSetup
#ifdef SetupExtras
prog_char timeoutsmenu_2_label[] PROGMEM = "Feeding";
prog_char timeoutsmenu_3_label[] PROGMEM = "LCD";
#endif  // SetupExtras
PROGMEM const char *timeoutsmenu_items[] = {
#ifdef ATOSetup
                        timeoutsmenu_0_label,
                        timeoutsmenu_1_label
#ifdef SetupExtras
                        ,
#endif  // SetupExtras
#endif  // ATOSetup
#ifdef SetupExtras
                        timeoutsmenu_2_label,
                        timeoutsmenu_3_label
#endif  // SetupExtras
                        };
enum TimeoutsMenuItem {
#ifdef ATOSetup
    TimeoutsMenu_ATOSet,
    TimeoutsMenu_ATOClear
#ifdef SetupExtras
    ,
#endif  // SetupExtras
#endif  // ATOSetup
#ifdef SetupExtras
    TimeoutsMenu_Feeding,
    TimeoutsMenu_LCD
#endif  // SetupExtras
};
#endif // if defined SetupExtras || defined ATOSetup


ReefAngelClass::ReefAngelClass()
{
	PCMSK0 |= 32;
	PCICR |= 1;
}

void ReefAngelClass::Init()
{
	byte taddr = 0;

	Wire.begin();
	Serial.begin(57600);
	pinMode(Piezo, OUTPUT);
	digitalWrite(lowATOPin,HIGH); //pull up resistor on lowATOPin
	digitalWrite(highATOPin,HIGH); //pull up resistor on highATOPin
	LCD.Init();
	Joystick.Init();
	TempSensor.Init();
	setSyncProvider(RTC.get);   // the function to get the time from the RTC
	setSyncInterval(SECS_PER_HOUR);  // Changed to sync every hour.
	//now();
	RAStart=now();
	LCD.BacklightOn();
	Relay.AllOff();
	//EEPROM_writeAnything(PH_Max,840); // 840=PH10.0
	//EEPROM_writeAnything(PH_Min,550); // 550=PH7.0
    PHMin = InternalMemory.PHMin_read();
    PHMax = InternalMemory.PHMax_read();
	taddr = InternalMemory.T1Pointer_read();
	// since byte can never be negative, taddr<0 is a pointless check
	//if (taddr>120 || taddr<0) EEPROM_writeAnything(T1Pointer,t);
	if (taddr>120) InternalMemory.T1Pointer_write(0);

#ifdef SetupExtras
	Timer[0].SetInterval(InternalMemory.FeedingTimer_read());  // Default Feeding timer
	Timer[3].SetInterval(InternalMemory.LCDTimer_read());  // LCD Sleep Mode timer
#else
	Timer[0].SetInterval(900);  // Default Feeding timer
	Timer[3].SetInterval(600);  // LCD Sleep Mode timer
#endif  // SetupExtras
	Timer[3].Start();  // start timer
	Timer[5].SetInterval(720);  // Store Params
	Timer[5].ForceTrigger();

#ifdef wifi
	conn = false;
#endif  // wifi

    // Initialize the Nested Menus
    InitMenus();
}

void ReefAngelClass::Refresh()
{
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

#endif  // wifi

	if (ds.read_bit()==0) return;  // ds for OneWire TempSensor
#ifdef DirectTempSensor
	LCD.Clear(DefaultBGColor,0,0,1,1);
	Params.Temp1=TempSensor.ReadTemperature(TempSensor.addrT1);
	LCD.Clear(DefaultBGColor,0,0,1,1);
	Params.Temp2=TempSensor.ReadTemperature(TempSensor.addrT2);
	LCD.Clear(DefaultBGColor,0,0,1,1);
	Params.Temp3=TempSensor.ReadTemperature(TempSensor.addrT3);
	LCD.Clear(DefaultBGColor,0,0,1,1);
	Params.PH=analogRead(PHPin);
	Params.PH=map(Params.PH, PHMin, PHMax, 700, 1000); // apply the calibration to the sensor reading
	LCD.Clear(DefaultBGColor,0,0,1,1);
	TempSensor.RequestConvertion();
	LCD.Clear(DefaultBGColor,0,0,1,1);
#else  // DirectTempSensor
    int x = TempSensor.ReadTemperature(TempSensor.addrT1);
    int y;
    y = x - Params.Temp1;
    // check to make sure the temp readings aren't beyond max allowed
#ifdef DEV_MODE
    Serial.print("T1: ");
    Serial.print(x);
    Serial.print("(");
    Serial.print(y);
    Serial.print(")");
#endif  // DEV_MODE
    if ( abs(y) < MAX_TEMP_SWING || Params.Temp1 == 0) Params.Temp1 = x;
    x = TempSensor.ReadTemperature(TempSensor.addrT2);
    y = x - Params.Temp2;
#ifdef DEV_MODE
    Serial.print(", T2: ");
    Serial.print(x);
    Serial.print("(");
    Serial.print(y);
    Serial.print(")");
#endif  // DEV_MODE
    if ( abs(y) < MAX_TEMP_SWING || Params.Temp2 == 0) Params.Temp2 = x;
    x = TempSensor.ReadTemperature(TempSensor.addrT3);
    y = x - Params.Temp3;
#ifdef DEV_MODE
    Serial.print(", T3: ");
    Serial.print(x);
    Serial.print("(");
    Serial.print(y);
    Serial.println(")");
#endif  // DEV_MODE
    if ( abs(y) < MAX_TEMP_SWING || Params.Temp3 == 0) Params.Temp3 = x;
	Params.PH=analogRead(PHPin);
	Params.PH=map(Params.PH, PHMin, PHMax, 700, 1000); // apply the calibration to the sensor reading
	TempSensor.RequestConvertion();
#endif  // DirectTempSensor
}

void ReefAngelClass::SetTemperatureUnit(byte unit)
{
    // 0 (or DEGREE_F) for farenheit
    // 1 (or DEGREE_C) for celcius
    TempSensor.unit = unit;
}

void ReefAngelClass::StandardLights(byte LightsRelay, byte OnHour, byte OnMinute, byte OffHour, byte OffMinute)
{
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
    if ( now()-RAStart > MHTimer )
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
    if ( LowATO.IsActive() && ( !LowATO.IsTopping()) )
    {
        LowATO.Timer = millis();
        LowATO.StartTopping();
        Relay.On(ATORelay);
    }

    // If the high switch is activated, this is a safeguard to prevent over running of the top off pump
    if ( HighATO.IsActive() )
    {
		LowATO.StopTopping();  // stop the low ato timer
		Relay.Off(ATORelay);
    }

    /*
    If the current time minus the start time of the ATO pump is greater than the specified timeout value
    AND the ATO pump is currently running:
    We turn on the status LED and shut off the ATO pump
    This prevents the ATO pump from contniously running.
    */
	if ( (millis()-LowATO.Timer > TempTimeout) && LowATO.IsTopping() )
	{
		LED.On();
		Relay.Off(ATORelay);
	}
}

void ReefAngelClass::SingleATO(bool bLow, byte ATORelay, byte byteTimeout, byte byteHrInterval)
{
    // if switch is active, stop the pump because the resevoir is full
    // when the switch is not active, we need to turn on the relay to fill up resevoir
    bool bCanRun = true;
    static int iLastTop = 0;
    if ( byteHrInterval )
    {
        int iSafeTop = NumMins(hour(), minute()) - iLastTop;
        if ( iSafeTop < 0 )
        {
            iSafeTop += 1440;
        }
        if ( iSafeTop < (byteHrInterval * 60) )
        {
            bCanRun = false;
        }
    }
    ReefAngel_ATOClass *ato;
    if ( bLow )
    {
        ato = &LowATO;
    }
    else
    {
        ato = &HighATO;
    }
    unsigned long t = byteTimeout;
    t *= 1000;
    if ( ato->IsActive() )
    {
        iLastTop = NumMins(hour(), minute());
        ato->StopTopping();
        Relay.Off(ATORelay);
    }
    else if ( !ato->IsTopping() )
    {
        if ( bCanRun )
        {
            ato->Timer = millis();
            ato->StartTopping();
            Relay.On(ATORelay);
        }
    }
    if ( (millis() - ato->Timer > t) && ato->IsTopping() )
    {
        LED.On();
        Relay.Off(ATORelay);
    }
}

void ReefAngelClass::DosingPump(byte DPRelay, byte DPTimer, byte OnHour, byte OnMinute, int RunTime)
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

#ifdef VersionMenu
void ReefAngelClass::DisplayVersion()
{
    // Display the Software Version
    LCD.DrawText(ModeScreenColor,DefaultBGColor,10,10,"Reef Angel");
    LCD.DrawText(ModeScreenColor,DefaultBGColor,10,20,"v"ReefAngel_Version);
#ifdef DEV_MODE
    LCD.DrawText(ModeScreenColor,DefaultBGColor,10,30,"Dev Mode");
#endif  // DEV_MODE

#ifdef wifi
    // Display wifi related information
    // Place holder information currently, need wifi module
    // to be able to write functions to retrieve actual information
    LCD.DrawText(ModeScreenColor,DefaultBGColor,10,40,"Wifi Enabled");
#endif  // wifi
}
#endif  // VersionMenu

void ReefAngelClass::ClearScreen(byte Color)
{
    // clears the entire screen
    LCD.Clear(Color, 0, 0, 131, 131);
}

void ReefAngelClass::InitMenus()
{
    // loads all the menus
    menusptr[MainMenu] = pgm_read_word(&(mainmenu_items[0]));
    menuqtysptr[MainMenu] = SIZE(mainmenu_items);
    menusptr[SetupMenu] = pgm_read_word(&(setupmenu_items[0]));
    menuqtysptr[SetupMenu] = SIZE(setupmenu_items);
    menusptr[LightsMenu] = pgm_read_word(&(lightsmenu_items[0]));
    menuqtysptr[LightsMenu] = SIZE(lightsmenu_items);
    menusptr[TempsMenu] = pgm_read_word(&(tempsmenu_items[0]));
    menuqtysptr[TempsMenu] = SIZE(tempsmenu_items);
#if defined SetupExtras || defined ATOSetup
    menusptr[TimeoutsMenu] = pgm_read_word(&(timeoutsmenu_items[0]));
    menuqtysptr[TimeoutsMenu] = SIZE(timeoutsmenu_items);
#endif  // if defined SetupExtras || defined ATOSetup

    // initialize menus
    PreviousMenu = DEFAULT_MENU;
    DisplayedMenu = DEFAULT_MENU;  // default menu to display
    SelectedMenuItem = DEFAULT_MENU_ITEM;  // default item to have selected
    redrawmenu = true;
    showmenu = false;  // initially we are showing the main graphic and no menu
}

void ReefAngelClass::ShowInterface()
{
    Refresh();

#ifdef wifi
    if ( conn )
    {
        // do something special when there is a connection
    }
    else
    {
#endif  // wifi
        // are we displaying the menu or not??
        if ( showmenu )
        {
            DisplayMenuHeading();
            DisplayMenu();
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
                    ClearScreen(DefaultBGColor);
                    SelectedMenuItem = DEFAULT_MENU_ITEM;
                    PreviousMenu = DEFAULT_MENU;
                    DisplayedMenu = MAIN_MENU;
                    showmenu = true;
                    redrawmenu = true;
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

                // display everything on the home screen except the graph
                // the graph is drawn/updated when we exit the main menu & when the parameters are saved
                LCD.DrawDate(6, 112);
#ifdef DisplayLEDPWM
                LCD.DrawMonitor(15, 60, Params, PWM.GetDaylightValue(), PWM.GetActinicValue());
#else
                LCD.DrawMonitor(15, 60, Params);
#endif  // DisplayLEDPWM
                byte TempRelay = Relay.RelayData;
                TempRelay &= Relay.RelayMaskOff;
                TempRelay |= Relay.RelayMaskOn;
                LCD.DrawOutletBox(12, 93, TempRelay);

                // Process any checks/tests/events that can happen while displaying the home screen
                // This can be the timers for wavemakers or any overheat temperatures

                // process timers
                if ( Timer[5].IsTriggered() )
                {
                    byte a = InternalMemory.T1Pointer_read();
                    int CurTemp;

                    // Values are stored in the I2CEEPROM1
                    a++;
                    if (a>=120) a=0;
                    Timer[5].Start();
                    CurTemp = map(Params.Temp1, T1LOW, T1HIGH, 0, 50); // apply the calibration to the sensor reading
                    CurTemp = constrain(CurTemp, 0, 50); // in case the sensor value is outside the range seen during calibration
                    //LCD.Clear(DefaultBGColor,0,0,1,1);
                    Memory.Write(a, CurTemp);
                    //LCD.Clear(DefaultBGColor,0,0,1,1);
                    CurTemp = map(Params.Temp2, T2LOW, T2HIGH, 0, 50); // apply the calibration to the sensor reading
                    CurTemp = constrain(CurTemp, 0, 50); // in case the sensor value is outside the range seen during calibration
                    //LCD.Clear(DefaultBGColor,0,0,1,1);
                    Memory.Write(a+120, CurTemp);
                    //LCD.Clear(DefaultBGColor,0,0,1,1);
                    CurTemp = map(Params.Temp3, T3LOW, T3HIGH, 0, 50); // apply the calibration to the sensor reading
                    CurTemp = constrain(CurTemp, 0, 50); // in case the sensor value is outside the range seen during calibration
                    //LCD.Clear(DefaultBGColor,0,0,1,1);
                    Memory.Write(a+240, CurTemp);
                    //LCD.Clear(DefaultBGColor,0,0,1,1);
                    CurTemp = map(Params.PH, PHLOW, PHHIGH, 0, 50); // apply the calibration to the sensor reading
                    CurTemp = constrain(CurTemp, 0, 50); // in case the sensor value is outside the range seen during calibration
                    //LCD.Clear(DefaultBGColor,0,0,1,1);
                    Memory.Write(a+360, CurTemp);
                    //LCD.Clear(DefaultBGColor,0,0,1,1);
                    InternalMemory.T1Pointer_write(a);
                    LCD.DrawGraph(5, 5);
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
#ifdef OverheatSetup
                if ( Params.Temp2 >= InternalMemory.OverheatTemp_read() )
#else  // OverheatSetup
                if ( Params.Temp2 >= 1500 )  // 150.0 F is the default
#endif // OverheatSetup
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
                    redrawmenu = true;
                    showmenu = true;
                    Timer[0].ForceTrigger();
                    Timer[3].Start();
                }
            }  // if DisplayedMenu == DEFAULT_MENU
        }  // if showmenu
#ifdef wifi
    }  // if conn
#endif  // wifi

}

void ReefAngelClass::DisplayMenu()
{
    // redrawmenu should only get set from within this function when we move the joystick or press the button
    byte qty = menuqtysptr[DisplayedMenu];
    int ptr = menusptr[DisplayedMenu];

    if ( Joystick.IsUp() )
    {
        // process UP press
        if ( (--SelectedMenuItem) > qty )
        {
            // we're moving up and we hit the top of the list
            // gotta wrap down to the bottom of the list
            // qty - 1 gives us the last item in our list
            //SelectedMenuItem = qty - 1;
            // This allows us to add in our last item
            SelectedMenuItem = qty;
        }
        redrawmenu = true;
        menutimeout = now();
    }

    if ( Joystick.IsDown() )
    {
        // process DOWN press
        // > allows for selection of last item, >= skips it
        if ( (++SelectedMenuItem) > qty )
        {
            // we've hit the bottom of the list
            // wrap around to the top of the list
            SelectedMenuItem = DEFAULT_MENU_ITEM;
        }
        redrawmenu = true;
        menutimeout = now();
    }

    // TODO Have ability to customize menu timeout delay
    if ( (now() - menutimeout) > MENU_TIMEOUT )
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
        ProcessButtonPress();
        redrawmenu = true;
        // Don't finish processing the rest of the menu
        return;
    }

    // don't redraw the menu if we don't have to
    if ( ! redrawmenu )
        return;

    byte i;
    byte bcolor, fcolor;
    char buffer[22];
    for ( i = 0; i <= qty; i++ )
    {
        bcolor = DefaultBGColor;
        fcolor = DefaultFGColor;
        if ( i < qty )
        {
            strcpy_P(buffer, (char *)ptr++);
        }
        else
        {
            // the last item in the list is either Exit or Prev Menu
            if ( DisplayedMenu == MainMenu )
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
            bcolor = SelectionBGColor;
            fcolor = SelectionFGColor;
        }
        LCD.Clear(bcolor, MENU_START_COL-3,
                         (i*MENU_START_ROW)+MENU_HEADING_SIZE-1,
                          MENU_END_COL,
                         (i*MENU_START_ROW)+(MENU_HEADING_SIZE+MENU_ITEM_HEIGHT-1));
        LCD.DrawText(fcolor, bcolor, MENU_START_COL, (i*MENU_START_ROW)+MENU_HEADING_SIZE, buffer);
    }  // for i
    // once drawn, no need to redraw yet
    redrawmenu = false;
}

void ReefAngelClass::DisplayMenuHeading()
{
    // NOTE do we redraw the menu heading or not?  use same logic as with the menu
    if ( ! redrawmenu )
        return;

    char buffer[10];

    switch ( DisplayedMenu )
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
#if defined SetupExtras || defined ATOSetup
        case TimeoutsMenu:
            {
                strcpy(buffer, "Timeouts:");
            }
            break;
#endif  // if defined SetupExtras || defined ATOSetup
    }  // switch MenuNum

    // clear the line that has the menu heading on it
    LCD.Clear(DefaultBGColor, MENU_START_COL, MENU_START_ROW, MAX_X, MAX_Y);
    // Display the menu heading
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, buffer);
}

void ReefAngelClass::DisplayMenuEntry(char *text)
{
    ClearScreen(DefaultBGColor);
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, text);
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*4, "Press to exit...");
}

void ReefAngelClass::FeedingMode()
{
	LCD.DrawText(ModeScreenColor, DefaultBGColor, 30, 10, "Feeding Mode");
	Timer[0].Start();  //Start Feeding Mode timer
#ifdef DisplayImages
    LCD.DrawEEPromImage(40,50, 40, 30, I2CEEPROM2, I2CEEPROM2_Feeding);
#endif  // DisplayImages

    int t;
    bool bDone = false;
    do
    {
        t = Timer[0].Trigger - now();
        if ( (t >= 0) && ! Timer[0].IsTriggered() )
        {
            LCD.Clear(DefaultBGColor,60+(intlength(t)*5),100,100,108);
            LCD.DrawText(DefaultFGColor,DefaultBGColor,60,100,t);
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
    ClearScreen(DefaultBGColor);
    Timer[3].Start();  // start LCD shutoff timer
}

void ReefAngelClass::WaterChangeMode()
{
	LCD.DrawText(ModeScreenColor, DefaultBGColor, 20, 10, "Water Change Mode");
#ifdef DisplayImages
	LCD.DrawEEPromImage(51,55, 40, 30, I2CEEPROM2, I2CEEPROM2_Water_Change);
#endif  // DisplayImages
	do
	{
	    // just wait for the button to be pressed
	    delay(200);
	} while ( ! Joystick.IsButtonPressed() );
	ClearScreen(DefaultBGColor);
	Timer[3].Start();  // start LCD shutoff timer
}

void ReefAngelClass::ProcessButtonPress()
{
    bool bResetMenuTimeout = true;
    switch ( DisplayedMenu )
    {
        default:  // DEFAULT_MENU == 255
//        {
//            // Default Screen
            break;
//        }
        case MainMenu:
        {
            ProcessButtonPressMain();
            break;
        }
        case SetupMenu:
        {
            ProcessButtonPressSetup();
            break;
        }
        case LightsMenu:
        {
            ProcessButtonPressLights();
            break;
        }
        case TempsMenu:
        {
            ProcessButtonPressTemps();
            break;
        }
#if defined SetupExtras || defined ATOSetup
        case TimeoutsMenu:
        {
            ProcessButtonPressTimeouts();
            break;
        }
#endif  // if defined SetupExtras || defined ATOSetup
        case EXCEED_TIMEOUT_MENU:
        {
            // we bypass all the other menus when the timeout has exceeded
            // we need to mimic the default action for the main menu in addition to
            // clearing out the stack
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            DisplayedMenu = DEFAULT_MENU;
            showmenu = false;
            ClearScreen(DefaultBGColor);
            bResetMenuTimeout = false;
            // we are exiting the menu, so draw the graph
            LCD.DrawGraph(5, 5);  // Redraw graphic of params
            break;
        }
    }
    // if a button was pressed, we have to reset the timeout after processing the press
    if ( bResetMenuTimeout )
    {
        menutimeout = now();
    }
}

void ReefAngelClass::ProcessButtonPressMain()
{
    showmenu = true;
    ClearScreen(DefaultBGColor);
    switch ( SelectedMenuItem )
    {
        case MainMenu_FeedingMode:
        {
            // turn off ports
            //byte CurrentRelayState = Relay.RelayData;
            Relay.Off(Port8);
            Relay.Off(Port4);
            Relay.Off(Port5);
            Relay.Write();
            // run feeding mode
            FeedingMode();
            // turn on ports
            Relay.On(Port8);
            Relay.On(Port4);
            Relay.On(Port5);
            // restore ports
            //Relay.RelayData = CurrentRelayState;
            Relay.Write();
            break;
        }
        case MainMenu_WaterChangeMode:
        {
            // turn off pumps for water change mode
            //byte CurrentRelayState = Relay.RelayData;
            Relay.Off(Port8);
            Relay.Off(Port4);
            Relay.Off(Port5);
            Relay.Write();
            // Display the water change mode
            WaterChangeMode();
            // turn on the pumps after water change mode
            Relay.On(Port8);
            Relay.On(Port4);
            Relay.On(Port5);
            //Relay.RelayData = CurrentRelayState;
            Relay.Write();
            break;
        }
        case MainMenu_Lights:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            PreviousMenu = DisplayedMenu;
            DisplayedMenu = LightsMenu;
            break;
        }
        case MainMenu_Temps:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            PreviousMenu = DisplayedMenu;
            DisplayedMenu = TempsMenu;
            break;
        }
#if defined SetupExtras || defined ATOSetup
        case MainMenu_Timeouts:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            PreviousMenu = DisplayedMenu;
            DisplayedMenu = TimeoutsMenu;
            break;
        }
#endif  // if defined SetupExtras || defined ATOSetup
        case MainMenu_Setup:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            PreviousMenu = DisplayedMenu;
            DisplayedMenu = SetupMenu;
            break;
        }
#ifdef VersionMenu
        case MainMenu_Version:
        {
            DisplayVersion();
            // turn off the menu so we can wait for the button press to exit
            showmenu = false;
            break;
        }
#endif  // VersionMenu
        default:
        {
            // This will be the EXIT choice
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            // switch to the previous menu
            DisplayedMenu = DEFAULT_MENU;
            // disable the menu, display main screen
            showmenu = false;
            // When we exit the main menu, we will redraw the graph
            LCD.DrawGraph(5, 5);  // Redraw graphic of params
            break;
        }
    }
}

void ReefAngelClass::ProcessButtonPressSetup()
{
    showmenu = true;
    ClearScreen(DefaultBGColor);
    switch ( SelectedMenuItem )
    {
#ifdef WavemakerSetup
        case SetupMenu_Wavemaker:
        {
            int v = InternalMemory.WM1Timer_read();
            int y = InternalMemory.WM2Timer_read();
            if ( SetupDoubleOption(v, y, 0, 21600, 5, "s", "", "Setup Wavemakers", "WM1:", "WM2:") )
            {
                InternalMemory.WM1Timer_write(v);
                InternalMemory.WM2Timer_write(y);
                // after we set the values we need to update the timers
                Timer[1].SetInterval(v);
                Timer[1].Start();
                Timer[2].SetInterval(y);
                Timer[2].Start();
            }
            break;
        }
#endif  // WavemakerSetup
#ifdef DosingPumpSetup
        case SetupMenu_DosingPump:
        {
            SetupDosingPump();
            break;
        }
#endif  // DosingPumpSetup
        case SetupMenu_CalibratePH:
        {
            SetupCalibratePH();
            break;
        }
#ifdef DateTimeSetup
        case SetupMenu_DateTime:
        {
            SetupDateTime();
            break;
        }
#endif  // DateTimeSetup
        default:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            // switch to the previous menu
            DisplayedMenu = PreviousMenu;
            break;
        }
    }
}

void ReefAngelClass::ProcessButtonPressLights()
{
    showmenu = true;  // set to true when displaying setup screens
    ClearScreen(DefaultBGColor);
    switch ( SelectedMenuItem )
    {
        case LightsMenu_On:
        {
            // turns on ports 2 & 3
            //         Port Mask 87654321
            Relay.RelayMaskOn = B00000110;
#ifdef DisplayLEDPWM
            // sets PWM to 50%
            PWM.SetActinic(50);
            PWM.SetDaylight(50);
#endif  // DisplayLEDPWM
            Relay.Write();
            DisplayMenuEntry("Lights On");
            showmenu = false;
            break;
        }
        case LightsMenu_Off:
        {
            // Reset ports 2 & 3
            //         Port Mask 87654321
            Relay.RelayMaskOn = B00000000;
#ifdef DisplayLEDPWM
            // sets PWM to 0%
            PWM.SetActinic(0);
            PWM.SetDaylight(0);
#endif  // DisplayLEDPWM
            Relay.Write();
            DisplayMenuEntry("Restore Lights");
            showmenu = false;
            break;
        }
#ifdef MetalHalideSetup
        case LightsMenu_MetalHalides:
        {
            SetupLightsOptionDisplay(true);
            break;
        }
        case LightsMenu_MetalHalideDelay:
        {
            int v = InternalMemory.MHDelay_read();
            if ( SetupSingleOption(v, 0, 255, 3, "m", "", "Setup MH Delay") )
            {
                InternalMemory.MHDelay_write((uint8_t)v);
            }
            break;
        }
#endif  // MetalHalideSetup
        case LightsMenu_StandardLights:
        {
            SetupLightsOptionDisplay(false);
            break;
        }
#ifdef DisplayLEDPWM
        case LightsMenu_LEDPWM:
        {
            int v = InternalMemory.LEDPWMActinic_read();
            int y = InternalMemory.LEDPWMDaylight_read();
            if ( SetupDoubleOption(v, y, 0, 100, 3, "%", "", "Setup LED", "AP:", "DP:") )
            {
                InternalMemory.LEDPWMActinic_write((uint8_t)v);
                InternalMemory.LEDPWMDaylight_write((uint8_t)y);
            }
            break;
        }
#endif  // DisplayLEDPWM
        default:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            // switch to the previous menu
            DisplayedMenu = PreviousMenu;
            break;
        }
    }
}

void ReefAngelClass::ProcessButtonPressTemps()
{
    showmenu = true;
    ClearScreen(DefaultBGColor);
    switch ( SelectedMenuItem )
    {
        case TempsMenu_Heater:
        {
            int v = InternalMemory.HeaterTempOn_read();
            int y = InternalMemory.HeaterTempOff_read();
            if ( SetupDoubleOption(v, y, 700, 900, 3, "F", ".", "Setup Heater", "On @", "Off @") )
            {
                InternalMemory.HeaterTempOn_write(v);
                InternalMemory.HeaterTempOff_write(y);
            }
            break;
        }
        case TempsMenu_Chiller:
        {
            int v = InternalMemory.ChillerTempOn_read();
            int y = InternalMemory.ChillerTempOff_read();
            if ( SetupDoubleOption(v, y, 700, 900, 3, "F", ".", "Setup Chiller", "On @", "Off @") )
            {
                InternalMemory.ChillerTempOn_write(v);
                InternalMemory.ChillerTempOff_write(y);
            }
            break;
        }
#ifdef OverheatSetup
        case TempsMenu_Overheat:
        {
            int v = InternalMemory.OverheatTemp_read();
            if ( SetupSingleOption(v, 800, 2000, 4, "F", ".", "Setup Overheat") )
            {
                InternalMemory.OverheatTemp_write(v);
            }
            break;
        }
#endif  // OverheatSetup
        case TempsMenu_OverheatClr:
        {
            LED.Off();
            Relay.RelayMaskOff = B11111111;  // Override MHlight auto
            Relay.Write();
            DisplayMenuEntry("Clear Overheat");
            showmenu = false;
            break;
        }
        default:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            // switch to the previous menu
            DisplayedMenu = PreviousMenu;
            break;
        }
    }
}

#if defined SetupExtras || defined ATOSetup
void ReefAngelClass::ProcessButtonPressTimeouts()
{
    showmenu = true;
    ClearScreen(DefaultBGColor);
    switch ( SelectedMenuItem )
    {
#ifdef ATOSetup
        case TimeoutsMenu_ATOSet:
        {
            int v = InternalMemory.ATOTimeout_read();
            if ( SetupSingleOption(v, 0, 255, 3, "s", "", "ATO Timeout") )
            {
                InternalMemory.ATOTimeout_write((uint8_t)v);
            }
            break;
        }
        case TimeoutsMenu_ATOClear:
        {
            // Need delay for clearing & returning screen
            LED.Off();
            LowATO.StopTopping();
            HighATO.StopTopping();
            DisplayMenuEntry("Clear ATO Timeout");
            showmenu = false;
            break;
        }
#endif  // ATOSetup
#ifdef SetupExtras
        case TimeoutsMenu_Feeding:
        {
            int v = InternalMemory.FeedingTimer_read();
            if ( SetupSingleOption(v, 0, 3600, 4, "s", "", "Feeding Timer") )
            {
                InternalMemory.FeedingTimer_write(v);
                // update the feeding timer value
                Timer[0].SetInterval(v);
            }
            break;
        }
        case TimeoutsMenu_LCD:
        {
            int v = InternalMemory.LCDTimer_read();
            if ( SetupSingleOption(v, 0, 3600, 4, "s", "", "Screen Timeout") )
            {
                InternalMemory.LCDTimer_write(v);
                // update the timer value
                Timer[3].SetInterval(v);
                Timer[3].Start();
            }
            break;
        }
#endif  // SetupExtras
        default:
        {
            SelectedMenuItem = DEFAULT_MENU_ITEM;
            // switch to the previous menu
            DisplayedMenu = PreviousMenu;
            break;
        }
    }
}
#endif  // if defined SetupExtras || defined ATOSetup

// Setup Menu Screens
// Setup Screens
bool ReefAngelClass::SetupSingleOption(int &v, int rangemin, int rangemax, byte maxdigits,
                       char* unit, char* subunit, char* title)
{
    // return true to save value stored in out in memory
    enum choices {
        OPT,
        OK,
        CANCEL
    };
    byte sel = OPT;
    bool bSave = false;
    bool bDone = false;
    bool bRedraw = true;
    bool bDrawButtons = true;
    ClearScreen(DefaultBGColor);
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, title);
    do
    {
        if ( bRedraw )
        {
            switch ( sel )
            {
                case OPT:
                {
                    LCD.DrawOption(v, 1, MENU_START_COL+35, MENU_START_ROW*5, unit, subunit, maxdigits);
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case OK:
                {
                    if ( bDrawButtons )
                    {
                        LCD.DrawOption(v, 0, MENU_START_COL+35, MENU_START_ROW*5, unit, subunit, maxdigits);
                        LCD.DrawOK(true);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case CANCEL:
                {
                    if ( bDrawButtons )
                    {
                        LCD.DrawOption(v, 0, MENU_START_COL+35, MENU_START_ROW*5, unit, subunit, maxdigits);
                        LCD.DrawOK(false);
                        LCD.DrawCancel(true);
                    }
                    break;
                }
            }
            bRedraw = false;
            bDrawButtons = false;
        }
        if ( Joystick.IsUp() )
        {
            if ( sel == OPT )
            {
                bRedraw = true;
                v++;
                if ( v > rangemax )
                {
                    v = rangemin;
                }
            }
        }
        if ( Joystick.IsDown() )
        {
            if ( sel == OPT )
            {
                bRedraw = true;
                v--;
                if ( v < rangemin )
                {
                    v = rangemax;
                }
            }
        }
        if ( Joystick.IsRight() )
        {
            bRedraw = true;
            bDrawButtons = true;
            sel++;
            if ( sel > CANCEL )
            {
                sel = OPT;
            }
        }
        if ( Joystick.IsLeft() )
        {
            bRedraw = true;
            bDrawButtons = true;
            sel--;
            if ( sel > CANCEL )
            {
                sel = CANCEL;
            }
        }
        if ( Joystick.IsButtonPressed() )
        {
            if ( sel == OK )
            {
                bSave = true;
                bDone = true;
            }
            else if ( sel == CANCEL )
            {
                bDone = true;
            }
        }
    } while ( ! bDone );

    // return true saves the value, false ignores the value
    return bSave;
}

bool ReefAngelClass::SetupDoubleOption(int &v, int &y, int rangemin, int rangemax, byte maxdigits,
                       char* unit, char* subunit, char* title,
                       char* prefix1, char* prefix2)
{
    // return true to save value stored in out in memory
    enum choices {
        OPT1,
        OPT2,
        OK,
        CANCEL
    };
    byte sel = OPT1;
    bool bSave = false;
    bool bDone = false;
    bool bRedraw = true;
    bool bDrawButtons = true;
    byte offset = 50;
    ClearScreen(DefaultBGColor);
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, title);
    // prefix for each option
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*4, prefix1);
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*6, prefix2);
    do
    {
        if ( bRedraw )
        {
            switch ( sel )
            {
                case OPT1:
                    {
                        LCD.DrawOption(y, 0, MENU_START_COL+offset, MENU_START_ROW*6, unit, subunit, maxdigits);
                        LCD.DrawOption(v, 1, MENU_START_COL+offset, MENU_START_ROW*4, unit, subunit, maxdigits);
                        if ( bDrawButtons )
                        {
                            LCD.DrawOK(false);
                            LCD.DrawCancel(false);
                        }
                        break;
                    }
                case OPT2:
                    {
                        LCD.DrawOption(v, 0, MENU_START_COL+offset, MENU_START_ROW*4, unit, subunit, maxdigits);
                        LCD.DrawOption(y, 1, MENU_START_COL+offset, MENU_START_ROW*6, unit, subunit, maxdigits);
                        if ( bDrawButtons )
                        {
                            LCD.DrawOK(false);
                            LCD.DrawCancel(false);
                        }
                        break;
                    }
                case OK:
                    {
                        if ( bDrawButtons )
                        {
                            LCD.DrawOption(v, 0, MENU_START_COL+offset, MENU_START_ROW*4, unit, subunit, maxdigits);
                            LCD.DrawOption(y, 0, MENU_START_COL+offset, MENU_START_ROW*6, unit, subunit, maxdigits);
                            LCD.DrawOK(true);
                            LCD.DrawCancel(false);
                        }
                        break;
                    }
                case CANCEL:
                    {
                        if ( bDrawButtons )
                        {
                            LCD.DrawOption(v, 0, MENU_START_COL+offset, MENU_START_ROW*4, unit, subunit, maxdigits);
                            LCD.DrawOption(y, 0, MENU_START_COL+offset, MENU_START_ROW*6, unit, subunit, maxdigits);
                            LCD.DrawOK(false);
                            LCD.DrawCancel(true);
                        }
                        break;
                    }
            }
            bRedraw = false;
            bDrawButtons = false;
        } // if bRedraw
        if ( Joystick.IsUp() )
        {
            bRedraw = true;
            if ( sel == OPT1 )
            {
                v++;
                if ( v > rangemax )
                {
                    v = rangemin;
                }
            }
            else if ( sel == OPT2 )
            {
                y++;
                if ( y > rangemax )
                {
                    y = rangemin;
                }
            }
        }
        if ( Joystick.IsDown() )
        {
            bRedraw = true;
            if ( sel == OPT1 )
            {
                v--;
                if ( v < rangemin )
                {
                    v = rangemax;
                }
            }
            else if ( sel == OPT2 )
            {
                y--;
                if ( y < rangemin )
                {
                    y = rangemax;
                }
            }
        }
        if ( Joystick.IsRight() )
        {
            bRedraw = true;
            bDrawButtons = true;  // only redraw the buttons if we are moving right or left
            // move right, if we are on cancel, wrap around to opt1
            sel++;
            if ( sel > CANCEL )
            {
                sel = OPT1;
            }
        }
        if ( Joystick.IsLeft() )
        {
            bRedraw = true;
            bDrawButtons = true;
            // move left, if we are on opt1, wrap around to cancel
            sel--;
            if ( sel > CANCEL )
            {
                sel = CANCEL;
            }
        }
        if ( Joystick.IsButtonPressed() )
        {
            // only break when button pressed on ok or cancel
            if ( sel == OK )
            {
                bDone = true;
                bSave = true;
            }
            else if ( sel == CANCEL )
            {
                bDone = true;
            }
        }
    } while ( ! bDone );

    // return true saves the value, false ignores the value
    return bSave;
}

void ReefAngelClass::SetupLightsOptionDisplay(bool bMetalHalide)
{
    enum choices {
        OPT1, // On Hour
        OPT2, // On Minute
        OPT3, // Off Hour
        OPT4, // Off Minute
        OK,
        CANCEL
    };
    byte sel = OPT1;
    bool bSave = false;
    bool bDone = false;
    bool bRedraw = true;
    bool bDrawButtons = true;
    uint8_t h1, h2, m1, m2;
    char msg[20];
    byte offset_hr = 45;
    byte offset_min = offset_hr+20;
    if ( bMetalHalide )
    {
        strcpy(msg, "Metal Halide Setup");
        h1 = InternalMemory.MHOnHour_read();
        m1 = InternalMemory.MHOnMinute_read();
        h2 = InternalMemory.MHOffHour_read();
        m2 = InternalMemory.MHOffMinute_read();
    }
    else
    {
        strcpy(msg, "Std Lights Setup");
        h1 = InternalMemory.StdLightsOnHour_read();
        m1 = InternalMemory.StdLightsOnMinute_read();
        h2 = InternalMemory.StdLightsOffHour_read();
        m2 = InternalMemory.StdLightsOffMinute_read();
    }
    ClearScreen(DefaultBGColor);
    // header / title
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, msg);
    // prefixes, draw in ':' between options
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*5, "On:");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL+offset_hr+13, MENU_START_ROW*5, ":");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*7, "Off:");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL+offset_hr+13, MENU_START_ROW*7, ":");
    do
    {
        if ( bRedraw )
        {
            switch ( sel )
            {
                case OPT1:
                {
                    // draw the second line items before the first line items
                    // so the UP & DOWN arrows show properly
                    LCD.DrawOption(m2, 0, MENU_START_COL+offset_min, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(h2, 0, MENU_START_COL+offset_hr, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(m1, 0, MENU_START_COL+offset_min, MENU_START_ROW*5, "", "");
                    LCD.DrawOption(h1, 1, MENU_START_COL+offset_hr, MENU_START_ROW*5, "", "");
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case OPT2:
                {
                    // draw the second line items before the first line items
                    // so the UP & DOWN arrows show properly
                    LCD.DrawOption(m2, 0, MENU_START_COL+offset_min, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(h2, 0, MENU_START_COL+offset_hr, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(m1, 1, MENU_START_COL+offset_min, MENU_START_ROW*5, "", "");
                    LCD.DrawOption(h1, 0, MENU_START_COL+offset_hr, MENU_START_ROW*5, "", "");
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case OPT3:
                {
                    // draw the first line items before the second line items
                    // so the UP & DOWN arrows show properly
                    LCD.DrawOption(m1, 0, MENU_START_COL+offset_min, MENU_START_ROW*5, "", "");
                    LCD.DrawOption(h1, 0, MENU_START_COL+offset_hr, MENU_START_ROW*5, "", "");
                    LCD.DrawOption(m2, 0, MENU_START_COL+offset_min, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(h2, 1, MENU_START_COL+offset_hr, MENU_START_ROW*7, "", "");
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case OPT4:
                {
                    // draw the first line items before the second line items
                    // so the UP & DOWN arrows show properly
                    LCD.DrawOption(m1, 0, MENU_START_COL+offset_min, MENU_START_ROW*5, "", "");
                    LCD.DrawOption(h1, 0, MENU_START_COL+offset_hr, MENU_START_ROW*5, "", "");
                    LCD.DrawOption(m2, 1, MENU_START_COL+offset_min, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(h2, 0, MENU_START_COL+offset_hr, MENU_START_ROW*7, "", "");
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case OK:
                {
                    if ( bDrawButtons )
                    {
                        LCD.DrawOption(m1, 0, MENU_START_COL+offset_min, MENU_START_ROW*5, "", "");
                        LCD.DrawOption(h1, 0, MENU_START_COL+offset_hr, MENU_START_ROW*5, "", "");
                        LCD.DrawOption(m2, 0, MENU_START_COL+offset_min, MENU_START_ROW*7, "", "");
                        LCD.DrawOption(h2, 0, MENU_START_COL+offset_hr, MENU_START_ROW*7, "", "");
                        LCD.DrawOK(true);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case CANCEL:
                {
                    if ( bDrawButtons )
                    {
                        LCD.DrawOption(m1, 0, MENU_START_COL+offset_min, MENU_START_ROW*5, "", "");
                        LCD.DrawOption(h1, 0, MENU_START_COL+offset_hr, MENU_START_ROW*5, "", "");
                        LCD.DrawOption(m2, 0, MENU_START_COL+offset_min, MENU_START_ROW*7, "", "");
                        LCD.DrawOption(h2, 0, MENU_START_COL+offset_hr, MENU_START_ROW*7, "", "");
                        LCD.DrawOK(false);
                        LCD.DrawCancel(true);
                    }
                    break;
                }
            }
            bRedraw = false;
            bDrawButtons = false;
        }
        if ( Joystick.IsUp() )
        {
            bRedraw = true;
            if ( sel == OPT1 )
            {
                h1++;
                if ( h1 > 23 )
                {
                    h1 = 0;
                }
            }
            else if ( sel == OPT2 )
            {
                m1++;
                if ( m1 > 59 )
                {
                    m1 = 0;
                }
            }
            else if ( sel == OPT3 )
            {
                h2++;
                if ( h2 > 23 )
                {
                    h2 = 0;
                }
            }
            else if ( sel == OPT4 )
            {
                m2++;
                if ( m2 > 59 )
                {
                    m2 = 0;
                }
            }
        }
        if ( Joystick.IsDown() )
        {
            bRedraw = true;
            if ( sel == OPT1 )
            {
                h1--;
                if ( h1 > 23 )
                {
                    h1 = 23;
                }
            }
            else if ( sel == OPT2 )
            {
                m1--;
                if ( m1 > 59 )
                {
                    m1 = 59;
                }
            }
            else if ( sel == OPT3 )
            {
                h2--;
                if ( h2 > 23 )
                {
                    h2 = 23;
                }
            }
            else if ( sel == OPT4 )
            {
                m2--;
                if ( m2 > 59 )
                {
                    m2 = 59;
                }
            }
        }
        if ( Joystick.IsRight() )
        {
            bRedraw = true;
            bDrawButtons = true;  // only redraw the buttons if we are moving right or left
            // move right, if we are on cancel, wrap around to opt1
            sel++;
            if ( sel > CANCEL )
            {
                sel = OPT1;
            }
        }
        if ( Joystick.IsLeft() )
        {
            bRedraw = true;
            bDrawButtons = true;
            // move left, if we are on opt1, wrap around to cancel
            sel--;
            if ( sel > CANCEL )
            {
                sel = CANCEL;
            }
        }
        if ( Joystick.IsButtonPressed() )
        {
            // only break when button pressed on ok or cancel
            if ( sel == OK )
            {
                bDone = true;
                bSave = true;
            }
            else if ( sel == CANCEL )
            {
                bDone = true;
            }
        }
    } while ( ! bDone );
    if ( bSave )
    {
        if ( bMetalHalide )
        {
            InternalMemory.MHOnHour_write(h1);
            InternalMemory.MHOnMinute_write(m1);
            InternalMemory.MHOffHour_write(h2);
            InternalMemory.MHOffMinute_write(m2);
        }
        else
        {
            InternalMemory.StdLightsOnHour_write(h1);
            InternalMemory.StdLightsOnMinute_write(m1);
            InternalMemory.StdLightsOffHour_write(h2);
            InternalMemory.StdLightsOffMinute_write(m2);
        }
    }
}

void ReefAngelClass::SetupCalibratePH()
{
    bool bOKSel = false;
    bool bSave = false;
    bool bDone = false;
    bool bDrawButtons = true;
    int iTPHMin = 1024, iTPHMax = 0;
    int iP = 0;
    byte offset = 65;
    // draw labels
    ClearScreen(DefaultBGColor);
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, "Calibrate PH");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*3, "PH 7.0");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*7, "PH 10.0");
    do
    {
        iP = analogRead(PHPin);
        LCD.DrawCalibrate(iP, MENU_START_COL + offset, MENU_START_ROW*5);
        if ( iP < iTPHMin )
        {
            iTPHMin = iP;
            LCD.DrawCalibrate(iP, MENU_START_COL + offset, MENU_START_ROW*3);
        }
        if ( iP > iTPHMax )
        {
            iTPHMax = iP;
            LCD.DrawCalibrate(iP, MENU_START_COL + offset, MENU_START_ROW*7);
        }
        if (  bDrawButtons )
        {
            if ( bOKSel )
            {
                LCD.DrawOK(true);
                LCD.DrawCancel(false);
            }
            else
            {
                LCD.DrawOK(false);
                LCD.DrawCancel(true);
            }
            bDrawButtons = false;
        }
        if ( Joystick.IsUp() || Joystick.IsDown() || Joystick.IsRight() || Joystick.IsLeft() )
        {
            // toggle the selection
            bOKSel = !bOKSel;
            bDrawButtons = true;
        }
        if ( Joystick.IsButtonPressed() )
        {
            bDone = true;
            if ( bOKSel )
            {
                bSave = true;
            }
        }
    } while ( ! bDone );

    if ( bSave )
    {
        // save PHMin & PHMax to memory
        InternalMemory.PHMax_write(iTPHMax);
        InternalMemory.PHMin_write(iTPHMin);
    }
}

#ifdef DateTimeSetup
void ReefAngelClass::SetupDateTime()
{
    enum choices {
        MONTH,
        DAY,
        YEAR,
        HOUR,
        MINUTE,
        OK,
        CANCEL
    };
    byte sel = MONTH;
    bool bSave = false;
    bool bDone = false;
    bool bRedraw = true;
    bool bDrawButtons = true;
    byte Year, Month, Day, Hour, Minute;
    byte MonthDays[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    byte DateRow = 45, TimeRow = 75;

    Year = year() - 2000;
    Month = month();
    Day = day();
    Hour = hour();
    Minute = minute();

    ClearScreen(DefaultBGColor);
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, "Set Date & Time");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, 10, DateRow,"Date:");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, 10, TimeRow,"Time:");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, 62, DateRow, "/");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, 82, DateRow, "/");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, 62, TimeRow, ":");

    do
    {
        if ( bRedraw )
        {
            switch ( sel )
            {
                case MONTH:
                {
                    LCD.DrawOption(Month, 1, 49, DateRow, "", "", 2);
                    LCD.DrawOption(Day, 0, 69, DateRow, "", "", 2);
                    LCD.DrawOption(Year, 0, 89, DateRow, "", "", 2);
                    LCD.DrawOption(Hour, 0, 49, TimeRow, "", "", 2);
                    LCD.DrawOption(Minute, 0, 69, TimeRow, "", "", 2);
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case DAY:
                {
                    LCD.DrawOption(Month, 0, 49, DateRow, "", "", 2);
                    LCD.DrawOption(Day, 1, 69, DateRow, "", "", 2);
                    LCD.DrawOption(Year, 0, 89, DateRow, "", "", 2);
                    LCD.DrawOption(Hour, 0, 49, TimeRow, "", "", 2);
                    LCD.DrawOption(Minute, 0, 69, TimeRow, "", "", 2);
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case YEAR:
                {
                    LCD.DrawOption(Month, 0, 49, DateRow, "", "", 2);
                    LCD.DrawOption(Day, 0, 69, DateRow, "", "", 2);
                    LCD.DrawOption(Year, 1, 89, DateRow, "", "", 2);
                    LCD.DrawOption(Hour, 0, 49, TimeRow, "", "", 2);
                    LCD.DrawOption(Minute, 0, 69, TimeRow, "", "", 2);
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case HOUR:
                {
                    LCD.DrawOption(Month, 0, 49, DateRow, "", "", 2);
                    LCD.DrawOption(Day, 0, 69, DateRow, "", "", 2);
                    LCD.DrawOption(Year, 0, 89, DateRow, "", "", 2);
                    LCD.DrawOption(Hour, 1, 49, TimeRow, "", "", 2);
                    LCD.DrawOption(Minute, 0, 69, TimeRow, "", "", 2);
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case MINUTE:
                {
                    LCD.DrawOption(Month, 0, 49, DateRow, "", "", 2);
                    LCD.DrawOption(Day, 0, 69, DateRow, "", "", 2);
                    LCD.DrawOption(Year, 0, 89, DateRow, "", "", 2);
                    LCD.DrawOption(Hour, 0, 49, TimeRow, "", "", 2);
                    LCD.DrawOption(Minute, 1, 69, TimeRow, "", "", 2);
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case OK:
                {
                    if ( bDrawButtons )
                    {
                        LCD.DrawOption(Month, 0, 49, DateRow, "", "", 2);
                        LCD.DrawOption(Day, 0, 69, DateRow, "", "", 2);
                        LCD.DrawOption(Year, 0, 89, DateRow, "", "", 2);
                        LCD.DrawOption(Hour, 0, 49, TimeRow, "", "", 2);
                        LCD.DrawOption(Minute, 0, 69, TimeRow, "", "", 2);
                        LCD.DrawOK(true);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case CANCEL:
                {
                    if ( bDrawButtons )
                    {
                        LCD.DrawOption(Month, 0, 49, DateRow, "", "", 2);
                        LCD.DrawOption(Day, 0, 69, DateRow, "", "", 2);
                        LCD.DrawOption(Year, 0, 89, DateRow, "", "", 2);
                        LCD.DrawOption(Hour, 0, 49, TimeRow, "", "", 2);
                        LCD.DrawOption(Minute, 0, 69, TimeRow, "", "", 2);
                        LCD.DrawOK(false);
                        LCD.DrawCancel(true);
                    }
                    break;
                }
            }

            bRedraw = false;
            bDrawButtons = false;
        }
        if ( Joystick.IsUp() )
        {
            switch ( sel )
            {
                case MONTH:
                {
                    Month++;
                    if ( Month > 12 )
                    {
                        Month = 1;
                    }
                    break;
                }
                case DAY:
                {
                    Day++;
                    // lookup days in a month table
                    if ( ! IsLeapYear(2000+Year) )
                    {
                        // not leap year
                        if ( Day > MonthDays[Month] )
                        {
                            Day = 1;
                        }
                    }
                    else
                    {
                        // leap year, only special case is February
                        if ( Month == 2 )
                        {
                            if ( Day > 29 )
                            {
                                Day = 1;
                            }
                        }
                        else
                        {
                            if ( Day > MonthDays[Month] )
                            {
                                Day = 1;
                            }
                        }
                    }
                    break;
                }
                case YEAR:
                {
                    Year++;
                    if ( Year > 99 )
                    {
                        Year = 0;
                    }
                    break;
                }
                case HOUR:
                {
                    Hour++;
                    if ( Hour > 23 )
                    {
                        Hour = 0;
                    }
                    break;
                }
                case MINUTE:
                {
                    Minute++;
                    if ( Minute > 59 )
                    {
                        Minute = 0;
                    }
                    break;
                }
            }
            bRedraw = true;
        }
        if ( Joystick.IsDown() )
        {
            switch ( sel )
            {
                case MONTH:
                {
                    Month--;
                    if ( Month < 1 || Month > 12 )
                    {
                        Month = 12;
                    }
                    break;
                }
                case DAY:
                {
                    Day--;
                    // lookup days in a month table
                    if ( ! IsLeapYear(2000+Year) )
                    {
                        // not leap year
                        if ( Day < 1 || Day > MonthDays[Month] )
                        {
                            Day = MonthDays[Month];
                        }
                    }
                    else
                    {
                        // leap year, only special case is February
                        if ( Month == 2 )
                        {
                            if ( Day < 1 || Day > MonthDays[Month] )
                            {
                                Day = 29;
                            }
                        }
                        else
                        {
                            if ( Day < 1 || Day > MonthDays[Month] )
                            {
                                Day = MonthDays[Month];
                            }
                        }
                    }
                    break;
                }
                case YEAR:
                {
                    Year--;
                    if ( Year > 99 )
                    {
                        Year = 99;
                    }
                    break;
                }
                case HOUR:
                {
                    Hour--;
                    if ( Hour > 23 )
                    {
                        Hour = 23;
                    }
                    break;
                }
                case MINUTE:
                {
                    Minute--;
                    if ( Minute > 59 )
                    {
                        Minute = 59;
                    }
                    break;
                }
            }
            bRedraw = true;
        }
        if ( Joystick.IsLeft() )
        {
            bRedraw = true;
            bDrawButtons = true;
            sel--;
            if ( sel > CANCEL )
            {
                sel = CANCEL;
            }
        }
        if ( Joystick.IsRight() )
        {
            bRedraw = true;
            bDrawButtons = true;
            sel++;
            if ( sel > CANCEL )
            {
                sel = MONTH;
            }
        }
        if ( Joystick.IsButtonPressed() )
        {
            if ( sel == OK )
            {
                bDone = true;
                bSave = true;
            }
            else if ( sel == CANCEL )
            {
                bDone = true;
            }
        }
    } while ( ! bDone );

    if ( bSave )
    {
        // Set Date & Time
        setTime(Hour, Minute, 0, Day, Month, Year);
        now();
        RTC.set(now());
    }
}
#endif  // DateTimeSetup

#ifdef DosingPumpSetup
void ReefAngelClass::SetupDosingPump()
{
    enum choices {
        OPT1, // DP1 On Hour
        OPT2, // DP1 On Minute
        OPT3, // DP1 Run Time
        OPT4, // DP2 On Hour
        OPT5, // DP2 On Minute
        OPT6, // DP2 Run Time
        OK,
        CANCEL
    };
    byte sel = OPT1;
    bool bSave = false;
    bool bDone = false;
    bool bRedraw = true;
    bool bDrawButtons = true;
    uint8_t h1, h2, m1, m2, r1, r2;
    byte offset_hr = 45;
    byte offset_min = offset_hr+20;
    h1 = InternalMemory.DP1OnHour_read();
    m1 = InternalMemory.DP1OnMinute_read();
    r1 = InternalMemory.DP1Timer_read();
    h2 = InternalMemory.DP2OnHour_read();
    m2 = InternalMemory.DP2OnMinute_read();
    r2 = InternalMemory.DP2Timer_read();
    ClearScreen(DefaultBGColor);
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW, "DP Setup");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*3, "DP1 On:");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL+offset_hr+13, MENU_START_ROW*3, ":");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*5, "Run Time:");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*7, "DP2 On:");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL+offset_hr+13, MENU_START_ROW*7, ":");
    LCD.DrawText(DefaultFGColor, DefaultBGColor, MENU_START_COL, MENU_START_ROW*9, "Run Time:");
    do
    {
        if ( bRedraw )
        {
            switch ( sel )
            {
                case OPT1:
                {
                    LCD.DrawOption(m2, 0, MENU_START_COL+offset_min, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(h2, 0, MENU_START_COL+offset_hr, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(r2, 0, MENU_START_COL+offset_min, MENU_START_ROW*9, "", "", 3);
                    LCD.DrawOption(m1, 0, MENU_START_COL+offset_min, MENU_START_ROW*3, "", "");
                    LCD.DrawOption(h1, 1, MENU_START_COL+offset_hr, MENU_START_ROW*3, "", "");
                    LCD.DrawOption(r1, 0, MENU_START_COL+offset_min, MENU_START_ROW*5, "", "", 3);
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case OPT2:
                {
                    LCD.DrawOption(m2, 0, MENU_START_COL+offset_min, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(h2, 0, MENU_START_COL+offset_hr, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(r2, 0, MENU_START_COL+offset_min, MENU_START_ROW*9, "", "", 3);
                    LCD.DrawOption(r1, 0, MENU_START_COL+offset_min, MENU_START_ROW*5, "", "", 3);
                    LCD.DrawOption(m1, 1, MENU_START_COL+offset_min, MENU_START_ROW*3, "", "");
                    LCD.DrawOption(h1, 0, MENU_START_COL+offset_hr, MENU_START_ROW*3, "", "");
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case OPT3:
                {
                    LCD.DrawOption(m2, 0, MENU_START_COL+offset_min, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(h2, 0, MENU_START_COL+offset_hr, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(r2, 0, MENU_START_COL+offset_min, MENU_START_ROW*9, "", "", 3);
                    LCD.DrawOption(m1, 0, MENU_START_COL+offset_min, MENU_START_ROW*3, "", "");
                    LCD.DrawOption(h1, 0, MENU_START_COL+offset_hr, MENU_START_ROW*3, "", "");
                    LCD.DrawOption(r1, 1, MENU_START_COL+offset_min, MENU_START_ROW*5, "", "", 3);
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case OPT4:
                {
                    LCD.DrawOption(m1, 0, MENU_START_COL+offset_min, MENU_START_ROW*3, "", "");
                    LCD.DrawOption(h1, 0, MENU_START_COL+offset_hr, MENU_START_ROW*3, "", "");
                    LCD.DrawOption(r1, 0, MENU_START_COL+offset_min, MENU_START_ROW*5, "", "", 3);
                    LCD.DrawOption(m2, 0, MENU_START_COL+offset_min, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(h2, 1, MENU_START_COL+offset_hr, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(r2, 0, MENU_START_COL+offset_min, MENU_START_ROW*9, "", "", 3);
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case OPT5:
                {
                    LCD.DrawOption(m1, 0, MENU_START_COL+offset_min, MENU_START_ROW*3, "", "");
                    LCD.DrawOption(h1, 0, MENU_START_COL+offset_hr, MENU_START_ROW*3, "", "");
                    LCD.DrawOption(r1, 0, MENU_START_COL+offset_min, MENU_START_ROW*5, "", "", 3);
                    LCD.DrawOption(r2, 0, MENU_START_COL+offset_min, MENU_START_ROW*9, "", "", 3);
                    LCD.DrawOption(m2, 1, MENU_START_COL+offset_min, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(h2, 0, MENU_START_COL+offset_hr, MENU_START_ROW*7, "", "");
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case OPT6:
                {
                    LCD.DrawOption(m1, 0, MENU_START_COL+offset_min, MENU_START_ROW*3, "", "");
                    LCD.DrawOption(h1, 0, MENU_START_COL+offset_hr, MENU_START_ROW*3, "", "");
                    LCD.DrawOption(r1, 0, MENU_START_COL+offset_min, MENU_START_ROW*5, "", "", 3);
                    LCD.DrawOption(m2, 0, MENU_START_COL+offset_min, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(h2, 0, MENU_START_COL+offset_hr, MENU_START_ROW*7, "", "");
                    LCD.DrawOption(r2, 1, MENU_START_COL+offset_min, MENU_START_ROW*9, "", "", 3);
                    if ( bDrawButtons )
                    {
                        LCD.DrawOK(false);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case OK:
                {
                    if ( bDrawButtons )
                    {
                        LCD.DrawOption(m1, 0, MENU_START_COL+offset_min, MENU_START_ROW*3, "", "");
                        LCD.DrawOption(h1, 0, MENU_START_COL+offset_hr, MENU_START_ROW*3, "", "");
                        LCD.DrawOption(r1, 0, MENU_START_COL+offset_min, MENU_START_ROW*5, "", "", 3);
                        LCD.DrawOption(m2, 0, MENU_START_COL+offset_min, MENU_START_ROW*7, "", "");
                        LCD.DrawOption(h2, 0, MENU_START_COL+offset_hr, MENU_START_ROW*7, "", "");
                        LCD.DrawOption(r2, 0, MENU_START_COL+offset_min, MENU_START_ROW*9, "", "", 3);
                        LCD.DrawOK(true);
                        LCD.DrawCancel(false);
                    }
                    break;
                }
                case CANCEL:
                {
                    if ( bDrawButtons )
                    {
                        LCD.DrawOption(m1, 0, MENU_START_COL+offset_min, MENU_START_ROW*3, "", "");
                        LCD.DrawOption(h1, 0, MENU_START_COL+offset_hr, MENU_START_ROW*3, "", "");
                        LCD.DrawOption(r1, 0, MENU_START_COL+offset_min, MENU_START_ROW*5, "", "", 3);
                        LCD.DrawOption(m2, 0, MENU_START_COL+offset_min, MENU_START_ROW*7, "", "");
                        LCD.DrawOption(h2, 0, MENU_START_COL+offset_hr, MENU_START_ROW*7, "", "");
                        LCD.DrawOption(r2, 0, MENU_START_COL+offset_min, MENU_START_ROW*9, "", "", 3);
                        LCD.DrawOK(false);
                        LCD.DrawCancel(true);
                    }
                    break;
                }
            }
            bRedraw = false;
            bDrawButtons = false;
        }
        if ( Joystick.IsUp() )
        {
            bRedraw = true;
            if ( sel == OPT1 )
            {
                h1++;
                if ( h1 > 23 )
                {
                    h1 = 0;
                }
            }
            else if ( sel == OPT2 )
            {
                m1++;
                if ( m1 > 59 )
                {
                    m1 = 0;
                }
            }
            else if ( sel == OPT3 )
            {
                r1++;
            }
            else if ( sel == OPT4 )
            {
                h2++;
                if ( h2 > 23 )
                {
                    h2 = 0;
                }
            }
            else if ( sel == OPT5 )
            {
                m2++;
                if ( m2 > 59 )
                {
                    m2 = 0;
                }
            }
            else if ( sel == OPT6 )
            {
                r2++;
            }
        }
        if ( Joystick.IsDown() )
        {
            bRedraw = true;
            if ( sel == OPT1 )
            {
                h1--;
                if ( h1 > 23 )
                {
                    h1 = 23;
                }
            }
            else if ( sel == OPT2 )
            {
                m1--;
                if ( m1 > 59 )
                {
                    m1 = 59;
                }
            }
            else if ( sel == OPT3 )
            {
                r1--;
            }
            else if ( sel == OPT4 )
            {
                h2--;
                if ( h2 > 23 )
                {
                    h2 = 23;
                }
            }
            else if ( sel == OPT5 )
            {
                m2--;
                if ( m2 > 59 )
                {
                    m2 = 59;
                }
            }
            else if ( sel == OPT6 )
            {
                r2--;
            }
        }
        if ( Joystick.IsRight() )
        {
            bRedraw = true;
            bDrawButtons = true;  // only redraw the buttons if we are moving right or left
            // move right, if we are on cancel, wrap around to opt1
            sel++;
            if ( sel > CANCEL )
            {
                sel = OPT1;
            }
        }
        if ( Joystick.IsLeft() )
        {
            bRedraw = true;
            bDrawButtons = true;
            // move left, if we are on opt1, wrap around to cancel
            sel--;
            if ( sel > CANCEL )
            {
                sel = CANCEL;
            }
        }
        if ( Joystick.IsButtonPressed() )
        {
            // only break when button pressed on ok or cancel
            if ( sel == OK )
            {
                bDone = true;
                bSave = true;
            }
            else if ( sel == CANCEL )
            {
                bDone = true;
            }
        }
    } while ( ! bDone );
    if ( bSave )
    {
        InternalMemory.DP1OnHour_write(h1);
        InternalMemory.DP1OnMinute_write(m1);
        InternalMemory.DP1Timer_write(r1);
        InternalMemory.DP2OnHour_write(h2);
        InternalMemory.DP2OnMinute_write(m2);
        InternalMemory.DP2Timer_write(r2);
    }
}
#endif  // DosingPumpSetup


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
