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

#ifndef	ReefAngel_NokiaLCD_h
#define ReefAngel_NokiaLCD_h

#include <ReefAngel_Globals.h>

typedef struct  {
  int Temp1;
  int Temp2;
  int Temp3;
  int PH;
} ParamsStruct;


class ReefAngel_NokiaLCD
{

public:



ReefAngel_NokiaLCD();

	void Init();
	void Sleep();
	void Wake();
	void BacklightOn();
	void BacklightOff();
	void Clear(byte color, byte x1, byte y1, byte x2, byte y2);
	void DrawText(byte fcolor, byte bcolor, byte x, byte y, char *text);
	void DrawText(byte fcolor, byte bcolor, byte x, byte y, int text);
	void DrawText(byte fcolor, byte bcolor, byte x, byte y, byte text);
	void DrawText(byte fcolor, byte bcolor, byte x, byte y, long text);
	void PutPixel(byte color, byte x, byte y);
	void SetContrast(byte Contrast);
	void DrawDate(byte x, byte y);
	void DrawOutletBox(byte x, byte y,byte RelayData);
	void DrawMonitor(byte x, byte y, ParamsStruct Params, byte DaylightPWMValue, byte ActinicPWMValue);
	void DrawGraph(byte x, byte y, int I2CAddr, int pointer);
	void DrawEEPromImage(int swidth, int sheight, byte x, byte y, int I2CAddr, int EEaddr);

    // Setup Screens

private:
	void SetBox(byte x1, byte y1, byte x2, byte y2);
	void SendCMD(byte data);
	void SendData(byte data);
	void ShiftBits(byte b);
	void DrawTextLine(byte fcolor, byte bcolor,byte x, byte y,char c);
	void DrawSingleMonitor(int Temp, byte fcolor, byte x, byte y, byte decimal);
	void DrawSingleGraph(byte color, byte x, byte y, int I2CAddr, int EEaddr);

};

#endif
