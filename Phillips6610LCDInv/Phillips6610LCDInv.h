#ifndef	Phillips6610LCDInv_h

#define Phillips6610LCDInv_h

#include "WProgram.h"


class Phillips6610LCDInv
{

public:



Phillips6610LCDInv();

void lcd_init();
void lcd_Sleep();
void lcd_Wake();
void lcd_BacklightOn();
void lcd_BacklightOff();
void lcd_clear(byte color, byte x1, byte y1, byte x2, byte y2);
void lcd_draw_text(byte fcolor, byte bcolor, byte x, byte y,char *text);
void lcd_put_pixel(byte color, byte x, byte y);
void lcd_set_box(byte x1, byte y1, byte x2, byte y2);
void sendCMD(byte data);
void sendData(byte data);


void DrawOption(int Option, byte Selected, byte x, byte y, char *unit, char *subunit, byte maxdigits);
void DrawCancel(byte Selected);
void DrawOK(byte Selected);
void DrawCalibrate(int i, byte x, byte y);

private:
void shiftBits(byte b);
void draw_text_line(byte fcolor, byte bcolor,byte x, byte y,char c);

};
#endif
