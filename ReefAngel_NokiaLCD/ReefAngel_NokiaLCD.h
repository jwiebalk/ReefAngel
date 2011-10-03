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

#ifndef	ReefAngel_NokiaLCD_h
#define ReefAngel_NokiaLCD_h

#include <ReefAngel_Globals.h>

typedef struct  {
  int Temp1;
  int Temp2;
  int Temp3;
  int PH;
  int Salinity;
  int ORP;
} ParamsStruct;

enum FontHeight {
	Font8x8,
	Font8x16,
	Font12x16,
	Num8x8,
	Num8x16,
	Num12x16,
};

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
#if defined FONT_8x8 || defined FONT_8x16 || defined NUMBERS_8x8 || defined NUMBERS_8x16
	void DrawLargeText(byte fcolor, byte bcolor, byte x, byte y, char* text, byte height = Font8x8);
#endif  // FONT_8x8 || FONT_8x16 || NUMBERS_8x8 || NUMBERS_8x16
#if defined FONT_12x16 || defined NUMBERS_12x16
	void DrawHugeText(byte fcolor, byte bcolor, byte x, byte y, char* text, byte height = Font12x16);
#endif  // FONT_12x16 || NUMBERS_12x16
#ifdef NUMBERS_16x16
	void DrawHugeNumbers(byte fcolor, byte bcolor, byte x, byte y, char *text);
#endif  // NUMBERS_16x16
	void DrawText(byte fcolor, byte bcolor, byte x, byte y, char *text);
	void DrawText(byte fcolor, byte bcolor, byte x, byte y, int text);
	void DrawText(byte fcolor, byte bcolor, byte x, byte y, byte text);
	void DrawText(byte fcolor, byte bcolor, byte x, byte y, long text);
	void PutPixel(byte color, byte x, byte y);
	void SetContrast(byte Contrast);
	void DrawDate(byte x, byte y);
	void DrawOutletBox(byte x, byte y,byte RelayData);
#if defined DisplayLEDPWM && ! defined RemoveAllLights
	void DrawMonitor(byte x, byte y, ParamsStruct Params, byte DaylightPWMValue, byte ActinicPWMValue);
#else  // defined DisplayLEDPWM && ! defined RemoveAllLights
    void DrawMonitor(byte x, byte y, ParamsStruct Params);
#endif  // defined DisplayLEDPWM && ! defined RemoveAllLights
	void DrawGraph(byte x, byte y);
	void DrawEEPromImage(int swidth, int sheight, byte x, byte y, int I2CAddr, int EEaddr);

    // For Setup
    void DrawOption(int Option, byte Selected, byte x, byte y, char *unit, char *subunit, byte maxdigits = 2);
    void DrawCancel(bool Selected);
    void DrawOK(bool Selected);
    void DrawCalibrate(int i, byte x, byte y);

private:
	void SetBox(byte x1, byte y1, byte x2, byte y2);
	void SendCMD(byte data);
	void SendData(byte data);
	void ShiftBits(byte b);
	void DrawTextLine(byte fcolor, byte bcolor, byte x, byte y, char c);
#if defined FONT_8x8 || defined FONT_8x16 || defined FONT_12x16 || defined NUMBERS_8x8 || defined NUMBERS_8x16 || defined NUMBERS_12x16
	void DrawLargeTextLine(byte fcolor, byte bcolor, byte x, byte y, uint16_t c, byte height);
#endif  // FONT_8x8 || FONT_8x16 || FONT_12X16 || NUMBERS_8x8 || NUMBERS_8x16 || NUMBERS_12x16
#ifdef NUMBERS_16x16
	void DrawHugeNumbersLine(byte fcolor, byte bcolor, byte x, byte y, uint16_t c);
#endif  // NUMBERS_16x16

public:
	void DrawSingleMonitor(int Temp, byte fcolor, byte x, byte y, byte decimal);
	void DrawSingleGraph(byte color, byte x, byte y, int EEaddr);

};

#endif
