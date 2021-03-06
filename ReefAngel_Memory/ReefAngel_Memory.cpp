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

#include "ReefAngel_Memory.h"
#include <ReefAngel_Globals.h>
#include <Wire.h>

ReefAngel_MemoryClass::ReefAngel_MemoryClass()
{
}

byte ReefAngel_MemoryClass::Read(unsigned int address)
{
	byte rdata = 0xFF;
	Wire.beginTransmission(I2CEEPROM1);
	Wire.send((int)(address >> 8));   // MSB
	Wire.send((int)(address & 0xFF)); // LSB
	Wire.endTransmission();
	Wire.requestFrom(I2CEEPROM1,1);
	if (Wire.available())
        rdata = Wire.receive();
	return rdata;
}

void ReefAngel_MemoryClass::Write(unsigned int address, byte data)
{
	Wire.beginTransmission(I2CEEPROM1);
	Wire.send((int)(address >> 8));   // MSB
	Wire.send((int)(address & 0xFF)); // LSB
	Wire.send(data);
	Wire.endTransmission();
	delay(10);
}

ReefAngel_MemoryClass Memory;

