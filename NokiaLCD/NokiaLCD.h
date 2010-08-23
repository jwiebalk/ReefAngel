#ifndef	NokiaLCD_h

#define NokiaLCD_h

#include "WProgram.h"

typedef struct  { 
  int Temp1; 
  int Temp2;
  int Temp3;
  int PH;
} ParamsStruct;


class NokiaLCD
{

public:



NokiaLCD();

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
