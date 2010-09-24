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

#include "ReefAngel.h"

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

ReefAngelClass::ReefAngelClass()
{
	PCMSK0 |= 32;
	PCICR |= 1;
	oldtick=0;
}

void ReefAngelClass::Init()
{
	byte taddr;
	byte t=0;

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
	now();
	LCD.BacklightOn();
	Relay.AllOff();
	PWM.SetDaylight(0);
	PWM.SetActinic(0);
	SetTemperatureUnit(0);
	ATO.topping=false;
	//EEPROM_writeAnything(PH_Max,900); // 750=PH10.0
	//EEPROM_writeAnything(PH_Min,540); // 480=PH7.0
	EEPROM_readAnything(PH_Min,PHMin);
	EEPROM_readAnything(PH_Max,PHMax);
	taddr=0;
	EEPROM_readAnything(T1Pointer,taddr);
	if (taddr>120 || taddr<0) EEPROM_writeAnything(T1Pointer,t);
	Timer[0].Interval=900; // Default Feeding timer
	Timer[3].Interval=600;  // set timer to x seconds - Timer 3 is used for sleep mode
	Timer[3].Start();  // start timer
	Timer[5].Interval=720; // Store Params
	Timer[5].Trigger=1;  //Initial trigger immediatelly
	Timer[1].Interval=200;  // set timer to x seconds - Timer 1 is used for wavemaker 1
	Timer[1].Start();  // start timer
	Timer[2].Interval=354;  // set timer to x seconds - Timer 2 is used for wavemaker 2
	Timer[2].Start();  // start timer
	//Timer[4].Interval=960; // Sleep mode
	//Timer[4].Start();
	conn=false;
	RAStart=millis();
	showmenu=false;
	SelectedMenu=255;
	selmenu=0;
}

void ReefAngelClass::LoadMenu(int pointer, byte qty)
{
	menupointer=pointer;
	menuqty=qty;
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

	if (ds.read_bit()==0) return;
	LCD.Clear(255,0,0,1,1);
	Params.Temp1=TempSensor.ReadTemperature(TempSensor.addrT1,TempUnit);
	LCD.Clear(255,0,0,1,1);
	Params.Temp2=TempSensor.ReadTemperature(TempSensor.addrT2,TempUnit);
	LCD.Clear(255,0,0,1,1);
	Params.Temp3=TempSensor.ReadTemperature(TempSensor.addrT3,TempUnit);
	LCD.Clear(255,0,0,1,1);
	Params.PH=analogRead(PHPin);
	Params.PH=map(Params.PH, PHMin, PHMax, 700, 1000); // apply the calibration to the sensor reading
	LCD.Clear(255,0,0,1,1);
	TempSensor.RequestConvertion();
	LCD.Clear(255,0,0,1,1);
	//if (Timer[4].IsTriggered()) LCD.BacklightOff();
}

void ReefAngelClass::SetTemperatureUnit(byte unit)
{
	TempUnit=unit;
}

void ReefAngelClass::StandardGUI()
{
	Refresh();
	if (conn)
	{
		//LED.On();
	}
	else
	{
		//LED.Off();
		if (showmenu)
		{
			byte bcolor;
			int pointer=menupointer;
			if (Joystick.IsUp())
			{
				if (--selmenu==255) selmenu=menuqty;
				menutimeout=now();
			}
			if (Joystick.IsDown())
			{
				if (++selmenu>menuqty) selmenu=0;
				menutimeout=now();
			}
			if (now()-menutimeout > 6)
			{
				selmenu=menuqty;
				ButtonPress++;
			}
			if (Joystick.IsButtonPressed())
			{
				Timer[3].Start();
				LCD.Clear(255,0,0,131,131);
				SelectedMenu=selmenu;
				showmenu=false;
				if (selmenu==menuqty)
				{
					ReturnMenuFunction();  // Return from menu function;
				}
				selmenu=0;
				return;
			}
			for (int a=0;a<menuqty;a++)
			{
				char buffer[22];
				bcolor=255;
				strcpy_P(buffer, (char * )pointer++);
				pointer+=strlen(buffer);
				if (selmenu==a) bcolor=243;
				LCD.Clear(bcolor,7,(a*10)+24,124,(a*10)+33 );
				LCD.DrawText(0,bcolor,10,(a*10)+25,buffer);
				//Serial.println(pointer);
				//Serial.println(buffer);
			}
			if (selmenu==menuqty) bcolor=243; else bcolor=255;
			LCD.Clear(bcolor,7,(menuqty*10)+24,124,(menuqty*10)+33 );
			LCD.DrawText(0,bcolor,10,(menuqty*10)+25,"Exit");
		}
		else
		{
			if (SelectedMenu==255)
			{
				if (Timer[3].IsTriggered()) LCD.BacklightOff();  // If timer 3 expires, turn backlight off
				if (Joystick.IsButtonPressed())
				{
					LCD.BacklightOn();
					if (Timer[3].Trigger==0)
					{
						Timer[3].Start();  // start timer
						return;
					}
					if (menupointer!=0)
					{
						LCD.Clear(255,0,0,131,131);
						LCD.DrawText(0,255,10,10,"Menu:");
						showmenu=true;
						menutimeout=now();
						return;
					}
				}
				if (Joystick.IsUp() || Joystick.IsDown() || Joystick.IsRight() || Joystick.IsLeft())
				{
					LCD.BacklightOn();
					Timer[3].Start();  // start timer
				}

				LCD.DrawDate(6, 112);  //Draw date and time at 6,40
				LCD.DrawMonitor(15, 60, Params, PWM.DaylightPWMValue, PWM.ActinicPWMValue);
				byte TempRelay=Relay.RelayData;
				TempRelay&=Relay.RelayMaskOff;
				TempRelay|=Relay.RelayMaskOn;
				LCD.DrawOutletBox(12,93,TempRelay);  //Draw outlet box at 12,93
				if (Timer[5].IsTriggered())
				{
					int a=EEPROM.read(T1Pointer);
					int CurTemp;

					a++;
					if (a>=120) a=0;
					Timer[5].Start();
					CurTemp = map(Params.Temp1, 700, 900, 0, 50); // apply the calibration to the sensor reading
					CurTemp = constrain(CurTemp, 0, 50); // in case the sensor value is outside the range seen during calibration
					LCD.Clear(255,0,0,1,1);
					Memory.Write(a, CurTemp);
					LCD.Clear(255,0,0,1,1);
					CurTemp = map(Params.Temp2, 650, 1500, 0, 50); // apply the calibration to the sensor reading
					CurTemp = constrain(CurTemp, 0, 50); // in case the sensor value is outside the range seen during calibration
					LCD.Clear(255,0,0,1,1);
					Memory.Write(a+120, CurTemp);
					LCD.Clear(255,0,0,1,1);
					CurTemp = map(Params.Temp3, 650, 920, 0, 50); // apply the calibration to the sensor reading
					CurTemp = constrain(CurTemp, 0, 50); // in case the sensor value is outside the range seen during calibration
					LCD.Clear(255,0,0,1,1);
					Memory.Write(a+240, CurTemp);
					LCD.Clear(255,0,0,1,1);
					CurTemp = map(Params.PH, 730, 890, 0, 50); // apply the calibration to the sensor reading
					CurTemp = constrain(CurTemp, 0, 50); // in case the sensor value is outside the range seen during calibration
					LCD.Clear(255,0,0,1,1);
					Memory.Write(a+360, CurTemp);
					LCD.Clear(255,0,0,1,1);
					EEPROM.write(T1Pointer,a);
					LCD.DrawGraph(5, 5, I2CEEPROM1, T1Pointer);
				}
			}
			else
			{
				if (Joystick.IsButtonPressed())
				{
					LCD.Clear(255,0,0,131,131);
					SelectedMenu=255;
					Timer[0].Trigger=1;
					Timer[3].Start();  // start timer
				}
			}
		}
	}
}

void ReefAngelClass::ReturnMenuFunction()
{
    SelectedMenu=255;  // Go back to main GUI
    LCD.DrawGraph(5, 5, I2CEEPROM1, T1Pointer);  // Redraw graphic of params
}
void ReefAngelClass::FeedingModeGUI()
{
	LCD.DrawText(19,255,30,10,"Feeding Mode");
    if (Timer[0].Trigger==0)
	{
		Timer[0].Start();  //Start Feeding Mode timer
		LCD.DrawEEPromImage(40,50, 40, 30, I2CEEPROM2, I2CEEPROM2_Feedging);
	}
    int t=Timer[0].Trigger-now();
    if (t>=0)
	{
		LCD.Clear(255,60+(intlength(t)*5),100,100,108);
		LCD.DrawText(0,255,60,100,t);
	}
	else
	{
		LCD.Clear(255,0,0,131,131);
		Timer[3].Start();  // start timer

	}
}

void ReefAngelClass::WaterChangeModeGUI()
{
	LCD.DrawText(19,255,20,10,"Water Change Mode");
    if (Timer[0].Trigger==0)
	{
		LCD.DrawEEPromImage(51,55, 40, 30, I2CEEPROM2, I2CEEPROM2_Water_Change);
	}
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
	unsigned long MHTimer=MHDelay*SECS_PER_MIN*1000;
	if (millis()-RAStart>MHTimer) StandardLights(LightsRelay, OnHour, OnMinute, OffHour, OffMinute);
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
	unsigned long TempTimeout=ATOTimeout*1000;
    if (ATO.IsLowActive() && ATO.topping==false)
    {
      ATO.ATOTimer=millis();
	  ATO.topping=true;
      Relay.On(ATORelay);
    }
    if (ATO.IsHighActive())
    {
		ATO.topping=false;
		Relay.Off(ATORelay);
    }
	if (millis()-ATO.ATOTimer>TempTimeout && ATO.topping)
	{
		LED.On();
		Relay.Off(ATORelay);
	}
}
char *ReefAngelClass::Version()
{
	return ReefAngel_Version;
}

void ReefAngelClass::DisplayVersion()
{
    // Display the Software Version
    LCD.DrawText(19,255,10,10,"Reef Angel");
        LCD.DrawText(19,255,10,20,"v"ReefAngel_Version);

#ifdef wifi
    // Display wifi related information
    // Place holder information currently, need wifi module
    // to be able to write functions to retrieve actual information
        LCD.DrawText(19,255,10,40,"Wifi Enabled");
        LCD.DrawText(19,255,10,50,"SSID: AP_SSID");
        LCD.DrawText(19,255,10,60,"IP: 255.255.255.255");
        LCD.DrawText(19,255,10,70,"Port: 80");
#endif  // wifi
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
    LCD.Clear(0xFF, 0, 0, 131, 131);
    LCD.DrawText(0x00, 0xFF, 7, 10, "Calibrate PH");
    LCD.DrawText(0x00, 0xFF, 7, 10*3, "PH 7.0");
    LCD.DrawText(0x00, 0xFF, 7, 10*7, "PH 10.0");
    do
    {
        iP = analogRead(PHPin);
        LCD.DrawCalibrate(iP, 7 + offset, 10*5);
        if ( iP < iTPHMin )
        {
            iTPHMin = iP;
            LCD.DrawCalibrate(iP, 7 + offset, 10*3);
        }
        if ( iP > iTPHMax )
        {
            iTPHMax = iP;
            LCD.DrawCalibrate(iP, 7 + offset, 10*7);
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
        EEPROM_writeAnything(PH_Max, iTPHMax);
        EEPROM_writeAnything(PH_Min, iTPHMin);
    }
}


LEDClass::LEDClass()
{
	pinMode(ledPin, OUTPUT);
	digitalWrite(ledPin,LOW);
}

void LEDClass::On()
{
	digitalWrite(ledPin,HIGH);
}

void LEDClass::Off()
{
	digitalWrite(ledPin,LOW);
}

JoystickClass::JoystickClass()
{
	pinMode(okPin, INPUT);
	digitalWrite(okPin,HIGH); //pull up resistor on okPin

}

void JoystickClass::Init()
{
	CalV=analogRead(VPin);
	CalH=analogRead(HPin);
	ButtonPress=0;
	KeyTurbo=1;
	KeyCount=0;
}

bool JoystickClass::IsButtonPressed()
{
	if (ButtonPress>0)
	{
		ButtonPress=0;
		return true;
	}
	else
	{
		return false;
	}
}

bool JoystickClass::IsUp()
{
	JoystickCenter();
	if ((analogRead(VPin)>CalV+70 && (millis()-KeyKeep)>KeyPressRate/KeyTurbo))
	{
		KeyCount++;
		if (KeyCount>5) KeyTurbo=5;
	    KeyKeep=millis();
		return true;
	}
	else
	{
		return false;
	}
}

bool JoystickClass::IsDown()
{
	JoystickCenter();
	if ((analogRead(VPin)<CalV-70 && (millis()-KeyKeep)>KeyPressRate/KeyTurbo))
	{
		KeyCount++;
		if (KeyCount>5) KeyTurbo=5;
	    KeyKeep=millis();
		return true;
	}
	else
	{
		return false;
	}
}

bool JoystickClass::IsRight()
{
	JoystickCenter();
	if ((analogRead(HPin)<CalH-70 && (millis()-KeyKeep)>KeyPressRate/KeyTurbo))
	{
		KeyCount++;
		if (KeyCount>5) KeyTurbo=5;
	    KeyKeep=millis();
		return true;
	}
	else
	{
		return false;
	}
}

bool JoystickClass::IsLeft()
{
	JoystickCenter();
	if ((analogRead(HPin)>CalH+70 && (millis()-KeyKeep)>KeyPressRate/KeyTurbo))
	{
		KeyCount++;
		if (KeyCount>5) KeyTurbo=5;
	    KeyKeep=millis();
		return true;
	}
	else
	{
		return false;
	}
}

void JoystickClass::JoystickCenter()
{
	if (analogRead(VPin)<CalV+70 && analogRead(VPin)>CalV-70 && analogRead(HPin)<CalH+70 && analogRead(HPin)>CalH-70)
	{
		KeyCount=0;
		KeyTurbo=1;
		KeyKeep=0;
	}
}

ATOClass::ATOClass()
{
}
bool ATOClass::IsLowActive()
{
	return !digitalRead(lowATOPin);
}

bool ATOClass::IsHighActive()
{
	return !digitalRead(highATOPin);
}

TempSensorClass::TempSensorClass()
{
}

void TempSensorClass::Init()
{
	byte addr[8];
	byte count=0;
	while (ds.search(addr))
	{
		count++;
		if(addr[0]==0x28)
		{
			if (count==1) memcpy(addrT1,addr,8);
			if (count==2) memcpy(addrT2,addr,8);
			if (count==3) memcpy(addrT3,addr,8);
		}
	}
	ds.reset_search();
}

void TempSensorClass::RequestConvertion()
{
	if(addrT1[0]==0x28)
	{
		ds.reset();
		ds.select(addrT1);
		ds.write(0x44,0);
	}
	if(addrT2[0]==0x28)
	{
		ds.reset();
		ds.select(addrT2);
		ds.write(0x44,0);
	}
	if(addrT3[0]==0x28)
	{
		ds.reset();
		ds.select(addrT3);
		ds.write(0x44,0);
	}
}

int TempSensorClass::ReadTemperature(byte addr[8], byte unit)
{
	//byte count=0;
	int Temp=0;
	byte data[12];


	//while (ds.search(addr))
	//{
		if(addr[0]==0x28)
		{
	//		count+=1;
			ds.reset();
			ds.select(addr);
			ds.write(0xBE);
			for (int i = 0; i < 2; i++)
			{	     // we need 9 bytes
				data[i] = ds.read();
			}
	//		if (SensorID==count)
	//		{
				Temp=(data[1]<<8)+data[0];//take the two bytes from the response relating to temperature
				Temp=Temp/1.6;
				if (Temp == 0) return 0;
				if (unit==0) Temp=Temp*1.8+320; // comment this line out to get celcius
	//		}
		}
	//	if (Temp != 0xffff) return Temp;
	//}
	//ds.reset_search();
	return Temp;
}

MemoryClass::MemoryClass()
{
}

byte MemoryClass::Read(unsigned int address)
{
	byte rdata = 0xFF;
	Wire.beginTransmission(I2CEEPROM1);
	Wire.send((int)(address >> 8));   // MSB
	Wire.send((int)(address & 0xFF)); // LSB
	Wire.endTransmission();
	Wire.requestFrom(I2CEEPROM1,1);
	if (Wire.available()) rdata = Wire.receive();
	return rdata;
}

void MemoryClass::Write(unsigned int address, byte data)
{
	Wire.beginTransmission(I2CEEPROM1);
	Wire.send((int)(address >> 8));   // MSB
	Wire.send((int)(address & 0xFF)); // LSB
	Wire.send(data);
	Wire.endTransmission();
	delay(10);
}

RelayClass::RelayClass()
{
	RelayData=0;
	RelayMaskOn=0;
	RelayMaskOff=0xff;
}

void RelayClass::On(byte ID)
{
    bitSet(RelayData,ID-1);
	//Write();
}

void RelayClass::Off(byte ID)
{
    bitClear(RelayData,ID-1);
	//Write();
}

void RelayClass::AllOn()
{
	RelayData=0xff;
	Write();
}

void RelayClass::AllOff()
{
	RelayData=0;
	Write();
}

void RelayClass::Toggle(byte ID)
{
	RelayData^=1<<(ID-1);
}

void RelayClass::Write()
{
  byte TempRelay=RelayData;
  TempRelay&=RelayMaskOff;
  TempRelay|=RelayMaskOn;

  Wire.beginTransmission(I2CExpander1);
  Wire.send(~TempRelay);   // MSB
  Wire.endTransmission();
}

// Test with Mask
//if (ReefAngel.Joystick.IsUp())
//{
//  ReefAngel.Relay.RelayMaskOn=0xff;
//  ReefAngel.Relay.RelayMaskOff=0xff;
//}
//if (ReefAngel.Joystick.IsDown())
//{
//  ReefAngel.Relay.RelayMaskOn=0;
//  ReefAngel.Relay.RelayMaskOff=0;
//}
//if (ReefAngel.Joystick.IsRight())
//{
//  ReefAngel.Relay.RelayMaskOn=0;
//  ReefAngel.Relay.RelayMaskOff=0xff;
//}


PWMClass::PWMClass()
{
}

void PWMClass::SetActinic(byte value)
{
  analogWrite(actinicPWMPin,value*255/100);
  ActinicPWMValue=value;

}

void PWMClass::SetDaylight(byte value)
{
  analogWrite(daylightPWMPin,value*255/100);
  DaylightPWMValue=value;
}

TimerClass::TimerClass()
{
	Trigger=0;
	Interval=0;
}

void TimerClass::Start()
{
	if (Interval>0) Trigger=now()+Interval;
}

bool TimerClass::IsTriggered()
{
	if (now()>Trigger && Trigger!=0)
	{
		Trigger=0;
		return true;
	}
	else
	{
		return false;
	}
}

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

byte intlength(int intin)
{
  if (intin>9999) return 5;
  if (intin>999) return 4;
  if (intin>99) return 3;
  if (intin>9) return 2;
  if (intin>=0 && intin<=9) return 1;
  if (intin<0) return 2;
}

int NumMins(uint8_t ScheduleHour, uint8_t ScheduleMinute)
{
	return (ScheduleHour*60) + ScheduleMinute;
}

ReefAngelClass ReefAngel = ReefAngelClass() ;
