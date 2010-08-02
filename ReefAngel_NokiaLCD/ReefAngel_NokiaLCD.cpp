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

#include <ReefAngel_Globals.h>
#include <Time.h>
#include "ReefAngel_NokiaLCD.h"
#include <Wire.h>
#include <EEPROM.h>

// Define Software SPI Pin Signal

#define BL 2          // Digital 2 --> BL
#define CS 3          // Digital 3 --> #CS
#define CLK 4         // Digital 4 --> SCLK
#define SDA 5         // Digital 5 --> SDATA
#define RESET 6       // Digital 6 --> #RESET

// already defined in Globals.h
//#define actinicPWMPin       9
//#define daylightPWMPin      10


// Phillips PCF8833 Command Set
#define NOP      0x00 	// nop
#define SWRESET  0x01 	// software reset
#define BSTROFF  0x02 	// booster voltage OFF
#define BSTRON   0x03 	// booster voltage ON
#define RDDIDIF  0x04 	// read display identification
#define RDDST    0x09 	// read display status
#define SLEEPIN  0x10 	// sleep in
#define SLEEPOUT 0x11 	// sleep out
#define PTLON    0x12 	// partial display mode
#define NORON    0x13 	// display normal mode
#define INVOFF   0x20 	// inversion OFF
#define INVON    0x21 	// inversion ON
#define DALO     0x22 	// all pixel OFF
#define DAL      0x23 	// all pixel ON
#define SETCON   0x25 	// write contrast
#define DISPOFF  0x28 	// display OFF
#define DISPON   0x29 	// display ON
#define CASET    0x2A 	// column address set
#define PASET    0x2B 	// page address set
#define RAMWR    0x2C 	// memory write
#define RGBSET   0x2D 	// colour set
#define PTLAR    0x30 	// partial area
#define VSCRDEF  0x33 	// vertical scrolling definition
#define TEOFF    0x34 	// test mode
#define TEON     0x35 	// test mode
#define MADCTL   0x36 	// memory access control
#define SEP      0x37 	// vertical scrolling start address
#define IDMOFF   0x38 	// idle mode OFF
#define IDMON    0x39 	// idle mode ON
#define COLMOD   0x3A 	// interface pixel format
#define SETVOP   0xB0 	// set Vop
#define BRS      0xB4 	// bottom row swap
#define TRS      0xB6 	// top row swap
#define DISCTR   0xB9 	// display control
//#define DAOR   0xBA 	// data order(DOR)
#define TCDFE    0xBD 	// enable/disable DF temperature compensation
#define TCVOPE   0xBF 	// enable/disable Vop temp comp
#define EC       0xC0 	// internal or external oscillator
#define SETMUL   0xC2 	// set multiplication factor
#define TCVOPAB  0xC3 	// set TCVOP slopes A and B
#define TCVOPCD  0xC4 	// set TCVOP slopes c and d
#define TCDF     0xC5 	// set divider frequency
#define DF8COLOR 0xC6 	// set divider frequency 8-color mode
#define SETBS    0xC7 	// set bias system
#define RDTEMP   0xC8 	// temperature read back
#define NLI      0xC9 	// n-line inversion
#define RDID1    0xDA 	// read ID1
#define RDID2    0xDB 	// read ID2
#define RDID3    0xDC 	// read ID3

// Moved to Globals.h for shared code
////a few basic colors
//#define RED			0xE0
//#define GREEN			0x1C
//#define BLUE			0x03
//#define YELLOW			0xFC
//#define MAGENTA			0xE3
//#define CYAN			0x1F
//#define BLACK			0x00
//#define WHITE			0xFF
//#define WaterTempColor      0xE0
//#define LightsTempColor     0xCC
//#define AmbientTempColor    0xAA
//#define PHColor             0x34
//#define DPColor             0x88
//#define APColor             0x49

#define cbi(reg, bit) (reg&=~(1<<bit))
#define sbi(reg, bit) (reg|= (1<<bit))

#define CS0 cbi(PORTD,CS);
#define CS1 sbi(PORTD,CS);
#define CLK0 cbi(PORTD,CLK);
#define CLK1 sbi(PORTD,CLK);
#define SDA0 cbi(PORTD,SDA);
#define SDA1 sbi(PORTD,SDA);
#define RESET0 cbi(PORTD,RESET);
#define RESET1 sbi(PORTD,RESET);
#define BL0 cbi(PORTD,BL);
#define BL1 sbi(PORTD,BL);

byte const font[475] = {
0x00 , 0x00 , 0x00 , 0x00 , 0x00 ,
0x00 , 0x06 , 0x5F , 0x06 , 0x00 ,                      // !
0x07 , 0x03 , 0x00 , 0x07 , 0x03 ,                      // ,
0x24 , 0x7E , 0x24 , 0x7E , 0x24 ,                      // #
0x24 , 0x2B , 0x6A , 0x12 , 0x00 ,                      // $
0x63 , 0x13 , 0x08 , 0x64 , 0x63 ,                      // %
0x36 , 0x49 , 0x56 , 0x20 , 0x50 ,                      // &
0x00 , 0x07 , 0x03 , 0x00 , 0x00 ,                      // //
0x00 , 0x3E , 0x41 , 0x00 , 0x00 ,                      // (
0x00 , 0x41 , 0x3E , 0x00 , 0x00 ,                      // )
0x08 , 0x3E , 0x1C , 0x3E , 0x08 ,                      // *
0x08 , 0x08 , 0x3E , 0x08 , 0x08 ,                      // +
0x00 , 0xE0 , 0x60 , 0x00 , 0x00 ,                      // ,
0x08 , 0x08 , 0x08 , 0x08 , 0x08 ,                      // -
0x00 , 0x60 , 0x60 , 0x00 , 0x00 ,                      // .
0x20 , 0x10 , 0x08 , 0x04 , 0x02 ,                      // /
0x3E , 0x51 , 0x49 , 0x45 , 0x3E ,                      // 0
0x00 , 0x42 , 0x7F , 0x40 , 0x00 ,                      // 1
0x62 , 0x51 , 0x49 , 0x49 , 0x46 ,                      // 2
0x22 , 0x49 , 0x49 , 0x49 , 0x36 ,                      // 3
0x18 , 0x14 , 0x12 , 0x7F , 0x10 ,                      // 4
0x2F , 0x49 , 0x49 , 0x49 , 0x31 ,                      // 5
0x3C , 0x4A , 0x49 , 0x49 , 0x30 ,                      // 6
0x01 , 0x71 , 0x09 , 0x05 , 0x03 ,                      // 7
0x36 , 0x49 , 0x49 , 0x49 , 0x36 ,                      // 8
0x06 , 0x49 , 0x49 , 0x29 , 0x1E ,                      // 9
0x00 , 0x6C , 0x6C , 0x00 , 0x00 ,                      // :
0x00 , 0xEC , 0x6C , 0x00 , 0x00 ,                      // ;
0x08 , 0x14 , 0x22 , 0x41 , 0x00 ,                      // <
0x24 , 0x24 , 0x24 , 0x24 , 0x24 ,                      // =
0x00 , 0x41 , 0x22 , 0x14 , 0x08 ,                      // >
0x02 , 0x01 , 0x59 , 0x09 , 0x06 ,                      // ?
0x3E , 0x41 , 0x5D , 0x55 , 0x1E ,                      // @
0x7E , 0x09 , 0x09 , 0x09 , 0x7E ,                      // A
0x7F , 0x49 , 0x49 , 0x49 , 0x36 ,                      // B
0x3E , 0x41 , 0x41 , 0x41 , 0x22 ,                      // C
0x7F , 0x41 , 0x41 , 0x41 , 0x3E ,                      // D
0x7F , 0x49 , 0x49 , 0x49 , 0x41 ,                      // E
0x7F , 0x09 , 0x09 , 0x09 , 0x01 ,                      // F
0x3E , 0x41 , 0x49 , 0x49 , 0x7A ,                      // G
0x7F , 0x08 , 0x08 , 0x08 , 0x7F ,                      // H
0x00 , 0x41 , 0x7F , 0x41 , 0x00 ,                      // I
0x30 , 0x40 , 0x40 , 0x40 , 0x3F ,                      // J
0x7F , 0x08 , 0x14 , 0x22 , 0x41 ,                      // K
0x7F , 0x40 , 0x40 , 0x40 , 0x40 ,                      // L
0x7F , 0x02 , 0x04 , 0x02 , 0x7F ,                      // M
0x7F , 0x02 , 0x04 , 0x08 , 0x7F ,                      // N
0x3E , 0x41 , 0x41 , 0x41 , 0x3E ,                      // O
0x7F , 0x09 , 0x09 , 0x09 , 0x06 ,                      // P
0x3E , 0x41 , 0x51 , 0x21 , 0x5E ,                      // Q
0x7F , 0x09 , 0x09 , 0x19 , 0x66 ,                      // R
0x26 , 0x49 , 0x49 , 0x49 , 0x32 ,                      // S
0x01 , 0x01 , 0x7F , 0x01 , 0x01 ,                      // T
0x3F , 0x40 , 0x40 , 0x40 , 0x3F ,                      // U
0x1F , 0x20 , 0x40 , 0x20 , 0x1F ,                      // V
0x3F , 0x40 , 0x3C , 0x40 , 0x3F ,                      // W
0x63 , 0x14 , 0x08 , 0x14 , 0x63 ,                      // X
0x07 , 0x08 , 0x70 , 0x08 , 0x07 ,                      // Y
0x71 , 0x49 , 0x45 , 0x43 , 0x00 ,                      // Z
0x00 , 0x7F , 0x41 , 0x41 , 0x00 ,                      // [
0x02 , 0x04 , 0x08 , 0x10 , 0x20 ,                      // Back slash
0x00 , 0x41 , 0x41 , 0x7F , 0x00 ,                      // ]
0x04 , 0x02 , 0x01 , 0x02 , 0x04 ,                      // ^
0x80 , 0x80 , 0x80 , 0x80 , 0x80 ,                      // _
//0x00 , 0x03 , 0x07 , 0x00 , 0x00 ,                      // `
0x20 , 0x40 , 0x80 , 0x40 , 0x20 ,                      // `
0x20 , 0x54 , 0x54 , 0x54 , 0x78 ,                      // a
0x7F , 0x44 , 0x44 , 0x44 , 0x38 ,                      // b
0x38 , 0x44 , 0x44 , 0x44 , 0x28 ,                      // c
0x38 , 0x44 , 0x44 , 0x44 , 0x7F ,                      // d
0x38 , 0x54 , 0x54 , 0x54 , 0x18 ,                      // e
0x08 , 0x7E , 0x09 , 0x09 , 0x00 ,                      // f
0x18 , 0xA4 , 0xA4 , 0xA4 , 0x7C ,                      // g
0x7F , 0x04 , 0x04 , 0x78 , 0x00 ,                      // h
0x00 , 0x00 , 0x7D , 0x00 , 0x00 ,                      // i
0x40 , 0x80 , 0x84 , 0x7D , 0x00 ,                      // j
0x7F , 0x10 , 0x28 , 0x44 , 0x00 ,                      // k
0x00 , 0x00 , 0x7F , 0x40 , 0x00 ,                      // l
0x7C , 0x04 , 0x18 , 0x04 , 0x78 ,                      // m
0x7C , 0x04 , 0x04 , 0x78 , 0x00 ,                      // n
0x38 , 0x44 , 0x44 , 0x44 , 0x38 ,                      // o
0xFC , 0x44 , 0x44 , 0x44 , 0x38 ,                      // p
0x38 , 0x44 , 0x44 , 0x44 , 0xFC ,                      // q
0x44 , 0x78 , 0x44 , 0x04 , 0x08 ,                      // r
0x08 , 0x54 , 0x54 , 0x54 , 0x20 ,                      // s
0x04 , 0x3E , 0x44 , 0x24 , 0x00 ,                      // t
0x3C , 0x40 , 0x20 , 0x7C , 0x00 ,                      // u
0x1C , 0x20 , 0x40 , 0x20 , 0x1C ,                      // v
0x3C , 0x60 , 0x30 , 0x60 , 0x3C ,                      // w
0x6C , 0x10 , 0x10 , 0x6C , 0x00 ,                      // x
0x9C , 0xA0 , 0x60 , 0x3C , 0x00 ,                      // y
0x64 , 0x54 , 0x54 , 0x4C , 0x00 ,                      // z
//0x08 , 0x3E , 0x41 , 0x41 , 0x00 ,                      // {
0x33 , 0x66 , 0xCC , 0x66 , 0x33 ,                      // {
0x00 , 0x00 , 0x7F , 0x00 , 0x00 ,                      // |
//0x00 , 0x41 , 0x41 , 0x3E , 0x08 ,                      // }
0xCC , 0x66 , 0x33 , 0x66 , 0xCC ,                      // }
0x02 , 0x01 , 0x02 , 0x01 , 0x00                        // ~
};


ReefAngel_NokiaLCD::ReefAngel_NokiaLCD()
{
    DDRD |= B01111100;   // Set SPI pins as output
    PORTD |= B01111000;  // Set SPI pins HIGH
}



void ReefAngel_NokiaLCD::ShiftBits(byte b)
{
    byte Bit;

    for (Bit = 0; Bit < 8; Bit++)     // 8 Bit Write
    {
        CLK0          // Standby SCLK
        if((b&0x80)>>7)
        {
            SDA1
        }
        else
        {
            SDA0
        }
        CLK1          // Strobe signal bit
        b <<= 1;   // Next bit data
    }
}

void ReefAngel_NokiaLCD::SendData(byte data) {
    CLK0
    SDA1
    CLK1
    ShiftBits(data);
}

void ReefAngel_NokiaLCD::SendCMD(byte data) {
    CLK0
    SDA0
    CLK1
    ShiftBits(data);

}

void ReefAngel_NokiaLCD::SetBox(byte x1, byte y1, byte x2, byte y2)
{
    SendCMD(CASET);   // page start/end ram
    SendData(x1);     // for some reason starts at 2
    SendData(x2);

    SendCMD(PASET);   // column start/end ram
    SendData(y1);
    SendData(y2);
}

void ReefAngel_NokiaLCD::Clear(byte color, byte x1, byte y1, byte x2, byte y2)
{
    uint16_t xmin, xmax, ymin, ymax;
    uint16_t i;
    unsigned int icolor;
    icolor = ~color;

    // best way to create a filled rectangle is to define a drawing box
    // and loop two pixels at a time
    // calculate the min and max for x and y directions
    xmin = (x1 <= x2) ? x1 : x2;
    xmax = (x1 > x2) ? x1 : x2;
    ymin = (y1 <= y2) ? y1 : y2;
    ymax = (y1 > y2) ? y1 : y2;

    // specify the controller drawing box according to those limits
    // Row address set (command 0x2B)
    SendCMD(CASET);
    SendData(xmin);
    SendData(xmax);

    // Column address set (command 0x2A)
    SendCMD(PASET);
    SendData(ymin);
    SendData(ymax);

    // WRITE MEMORY
    SendCMD(RAMWR);

    // loop on total number of pixels / 2
    for (i = 0; i < ((xmax - xmin + 1) * (ymax - ymin + 1)) ; i++)
    {
        // use the color value to output three data bytes covering two pixels
        // For some reason, it has to send blue first then green and red
        //SendData((color << 4) | ((color & 0xF0) >> 4));
        //SendData(((color >> 4) & 0xF0) | (color & 0x0F));
        //SendData((color & 0xF0) | (color >> 8));
        SendData(icolor);
    }
}

void ReefAngel_NokiaLCD::Init()
{
    // Initial state
    CS1
    CS0

    // Hardware Reset LCD
    RESET0
    delay(100);
    RESET1
    delay(100);

    //Software Reset
    SendCMD(SWRESET);

    //Sleep Out
    SendCMD(SLEEPOUT);

    //Booster ON
    SendCMD(BSTRON);


    //Display On
    SendCMD(DISPON);

    //Normal display mode
    SendCMD(NORON);

    //Display inversion on
    SendCMD(INVON);

    //Data order
    //SendCMD(0xBA);

    SendCMD(SETCON);
    SendData(0x38);

    //Memory data access control
    SendCMD(MADCTL);

    //SendData(8|64);   //rgb + MirrorX
    //SendData(8|128);   //rgb + MirrorY
    SendData(0xc0);

    SendCMD(COLMOD);
    SendData(2);   //16-Bit per Pixel

    Clear(0xff,0,0,131,131);

}

void ReefAngel_NokiaLCD::Sleep()
{
    SendCMD(DISPOFF);
    SendCMD(SLEEPIN);
    BacklightOff();
}

void ReefAngel_NokiaLCD::Wake()
{
    BacklightOn();
    SendCMD(SLEEPOUT);
    SendCMD(DISPON);
}

void ReefAngel_NokiaLCD::BacklightOn()
{
    BL1
}

void ReefAngel_NokiaLCD::BacklightOff()
{
    BL0
}


void ReefAngel_NokiaLCD::DrawTextLine(byte fcolor, byte bcolor,byte x, byte y,char c)
{
    unsigned int i;
    unsigned int ifcolor, ibcolor;
    ifcolor = ~fcolor;
    ibcolor = ~bcolor;
    SetBox(x,y,x,y+7);
    SendCMD(RAMWR);
    for(i=0;i<8;i++)
    {
        if (1<<i & c)
            SendData(ifcolor);
        else
            SendData(ibcolor);
    }
}

void ReefAngel_NokiaLCD::DrawText(byte fcolor, byte bcolor, byte x, byte y,char *text)
{
    byte c;
    byte t;
    unsigned int i;
    unsigned int j;
    while(*text != 0)
    {
        t = *text;
        i = t - 32;
        i = i * 5;
        for(j = i; j < i+5; j++)
        {
            c = font[j];
            DrawTextLine(fcolor, bcolor, x++, y, c);
        }
        DrawTextLine(fcolor, bcolor, x++, y, 0);
        text++;
    }
}

void ReefAngel_NokiaLCD::DrawText(byte fcolor, byte bcolor, byte x, byte y,int text)
{
	char temp[6];
	itoa(text,temp,10);
	DrawText(fcolor, bcolor, x, y,temp);
}

void ReefAngel_NokiaLCD::DrawText(byte fcolor, byte bcolor, byte x, byte y,byte text)
{
	char temp[6];
	itoa(text,temp,10);
	DrawText(fcolor, bcolor, x, y,temp);
}

void ReefAngel_NokiaLCD::DrawText(byte fcolor, byte bcolor, byte x, byte y,long text)
{
	char temp[20];
	ltoa(text,temp,10);
	DrawText(fcolor, bcolor, x, y,temp);
}

void ReefAngel_NokiaLCD::PutPixel(byte color, byte x, byte y)
{
    unsigned int icolor;
    icolor = ~color;

    SendCMD(CASET);   // page start/end ram
    SendData(x);      // for some reason starts at 2
    SendData(x+1);

    SendCMD(PASET);   // column start/end ram
    SendData(y);
    SendData(y+1);
    SendCMD(RAMWR);
    SendData(icolor);
}

void ReefAngel_NokiaLCD::SetContrast(byte Contrast)
{
    SendCMD(SETCON);
    SendData(Contrast);
}


void ReefAngel_NokiaLCD::DrawDate(byte x, byte y)
{
    //byte iTimeHourOffset=0;
    char text[21];
    char temp[]="  ";
    strcpy(text,"");
    itoa(month(),temp,10);
    if (temp[1]==0) strcat(text,"0");
    strcat(text,temp);
    strcat(text,"/");
    itoa(day(),temp,10);
    if (temp[1]==0) strcat(text,"0");
    strcat(text,temp);
    strcat(text,"/");
    itoa(year()-2000,temp,10);
    if (temp[1]==0) strcat(text,"0");
    strcat(text,temp);
    strcat(text," ");
    //if (iTimeHour>12) iTimeHourOffset=12;
    itoa(hourFormat12(),temp,10);
    if (temp[1]==0) strcat(text,"0");
    strcat(text,temp);
    strcat(text,":");
    itoa(minute(),temp,10);
    if (temp[1]==0) strcat(text,"0");
    strcat(text,temp);
    strcat(text,":");
    itoa(second(),temp,10);
    if (temp[1]==0) strcat(text,"0");
    strcat(text,temp);
    if (isAM())
    {
        strcat(text," AM");
    }
    else
    {
        strcat(text," PM");
    }
    DrawText(COLOR_RED, COLOR_WHITE, x, y, text);
}

void ReefAngel_NokiaLCD::DrawOutletBox(byte x, byte y,byte RelayData)
{
    Clear(94,x,y,x+104,y);
    Clear(94,x,y+12,x+104,y+12);
    for (int a=0;a<8;a++)
    {
        byte bcolor = COLOR_WHITE;
        byte fcolor = COLOR_BLACK;
        char temp[]="  ";
        if ((RelayData&(1<<a))==1<<a)
        {
            bcolor = 13;
            fcolor = COLOR_WHITE;
        }
        Clear(bcolor,x+1+(a*13),y+1,x+14+(a*13),y+11);
        itoa(a+1,temp,10);
        DrawText(fcolor,bcolor,x+5+(a*13),y+3,temp);
    }
}

void ReefAngel_NokiaLCD::DrawSingleMonitor(int Temp, byte fcolor, byte x, byte y, byte decimal)
{
    char text[7];
    char temptxt[3];
    if (Temp==0xFFFF) Temp=0;
    itoa(Temp/decimal,text,10);
    if (decimal>1)
    {
        itoa(Temp%decimal,temptxt,10);
        strcat(text , ".");
        if (Temp%decimal<10 && decimal==100) strcat(text , "0");
        strcat(text , temptxt);
    }
    Clear(COLOR_WHITE,x,y,x+30,y+8);
    DrawText(fcolor,COLOR_WHITE,x,y,text);
}

void ReefAngel_NokiaLCD::DrawMonitor(byte x, byte y, ParamsStruct Params, byte DaylightPWMValue, byte ActnicPWMValue)
{
    DrawText(WaterTempColor,COLOR_WHITE,x,y,"T1:");
    DrawSingleMonitor(Params.Temp1, WaterTempColor, x+18, y,10);
    DrawText(LightsTempColor,COLOR_WHITE,x,y+10,"T2:");
    DrawSingleMonitor(Params.Temp2, LightsTempColor, x+18, y+10,10);
    DrawText(AmbientTempColor,COLOR_WHITE,x,y+20,"T3:");
    DrawSingleMonitor(Params.Temp3, AmbientTempColor, x+18, y+20,10);
    DrawText(PHColor,COLOR_WHITE,x+60,y,"PH:");
    DrawSingleMonitor(Params.PH, PHColor, x+78, y,100);
    DrawText(DPColor,COLOR_WHITE,x+60,y+10,"DP:");
    DrawSingleMonitor(DaylightPWMValue, DPColor, x+78, y+10,1);
    DrawText(APColor,COLOR_WHITE,x+60,y+20,"AP:");
    DrawSingleMonitor(ActnicPWMValue, APColor, x+78, y+20,1);
}

void ReefAngel_NokiaLCD::DrawSingleGraph(byte color, byte x, byte y, int I2CAddr, int EEaddr)
{
	int start;
	for (int a=0;a<120;a++)
	{
		start=EEaddr+a;
		if (start > (int(EEaddr/120)+1)*120) start=start-120;
		Wire.beginTransmission(I2CAddr);
		Wire.send((int)(start >> 8));   // MSB
		Wire.send((int)(start & 0xFF)); // LSB
		Wire.endTransmission();
		Wire.requestFrom(I2CAddr,1);
		if (Wire.available()) PutPixel(color,x+a,y+50-Wire.receive());
	}

}

void ReefAngel_NokiaLCD::DrawEEPromImage(int swidth, int sheight, byte x, byte y, int I2CAddr, int EEaddr)
{
    int count = 0;
    SetBox(x,y,swidth-1+x,sheight-1+y);
    SendCMD(0x2c);
    //for (int j = 0; j < sheight; j++)
    //{
    //  for (int i = 0; i < swidth; i++)
    //  {
    //e.lcd_put_pixel(readEEPROM(I2CEEPROM,count+start),i+x,j+y);
    //    count++;
    //  }
    //  WatchDogReset();
    //}
    do
    {
        Wire.beginTransmission(I2CAddr);
        Wire.send((int)(EEaddr+count >> 8));   // MSB
        Wire.send((int)(EEaddr+count & 0xFF)); // LSB
        Wire.endTransmission();
        Wire.requestFrom(I2CAddr,30);
        for (int j = 0; j < 30; j++)
        {
            count+=1;
            if ((count<=swidth*sheight) && Wire.available()) SendData(~Wire.receive());
        }
    }
    while (count < swidth*sheight);
}

void ReefAngel_NokiaLCD::DrawGraph(byte x, byte y, int I2CAddr, int pointer)
{
    Clear(COLOR_WHITE,0,y,131,y+50);
    Clear(COLOR_BLACK,x,y,x,y+50);
    for (int i=6; i<=131; i+=3)
    {
        PutPixel(0x49, i, y+25);
    }
    DrawSingleGraph(WaterTempColor,x,y,I2CAddr,EEPROM.read(pointer));
    DrawSingleGraph(LightsTempColor,x,y,I2CAddr,EEPROM.read(pointer)+120);
    DrawSingleGraph(AmbientTempColor,x,y,I2CAddr,EEPROM.read(pointer)+240);
    DrawSingleGraph(PHColor,x,y,I2CAddr,EEPROM.read(pointer)+360);
}
