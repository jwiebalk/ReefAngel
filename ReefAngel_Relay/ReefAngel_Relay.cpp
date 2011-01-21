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

#include "ReefAngel_Relay.h"
#include <ReefAngel_Globals.h>
#include <Wire.h>

ReefAngel_RelayClass::ReefAngel_RelayClass()
{
	RelayData = 0;
	RelayMaskOn = 0;
	RelayMaskOff = 0xff;
#ifdef RelayExp
	for ( byte EID = 0; EID < MAX_RELAY_EXPANSION_MODULES; EID++ )
	{
		RelayDataE[EID] = 0;
		RelayMaskOnE[EID] = 0;
		RelayMaskOffE[EID] = 0xff;
	}
#endif  // RelayExp
}

void ReefAngel_RelayClass::On(byte ID)
{
    if ( ID < 9 ) bitSet(RelayData, ID-1);
#ifdef RelayExp
	if ( (ID > 10) && (ID < 89) )
	{
		byte EID = byte(ID/10);
		bitSet(RelayDataE[EID-1],(ID%10)-1);
	}
#endif  // RelayExp
}

void ReefAngel_RelayClass::Off(byte ID)
{
    if ( ID < 9 ) bitClear(RelayData, ID-1);
#ifdef RelayExp
	if ( (ID > 10) && (ID < 89) )
	{
		byte EID = byte(ID/10);
		bitSet(RelayDataE[EID-1],(ID%10)-1);
	}
#endif  // RelayExp
}

void ReefAngel_RelayClass::AllOn()
{
	RelayData = 0xff;
#ifdef RelayExp
	for ( byte EID = 0; EID < MAX_RELAY_EXPANSION_MODULES; EID++ )
	{
		RelayDataE[EID] = 0xff;
	}
#endif  // RelayExp
}

void ReefAngel_RelayClass::AllOff()
{
	RelayData = 0;
#ifdef RelayExp
	for ( byte EID = 0; EID < MAX_RELAY_EXPANSION_MODULES; EID++ )
	{
		RelayDataE[EID] = 0;
	}
#endif  // RelayExp
}

void ReefAngel_RelayClass::Toggle(byte ID)
{
	if ( ID < 9 ) RelayData ^= 1 << (ID-1);
#ifdef RelayExp
	if ( (ID > 10) && (ID < 89) )
	{
		byte EID = byte(ID/10);
		RelayDataE[EID-1] ^= 1 << ((ID%10)-1);
	}
#endif  // RelayExp
}

void ReefAngel_RelayClass::Write()
{
    byte TempRelay = RelayData;
    TempRelay &= RelayMaskOff;
    TempRelay |= RelayMaskOn;

    Wire.beginTransmission(I2CExpander1);
    Wire.send(~TempRelay);   // MSB
    Wire.endTransmission();
#ifdef RelayExp
	for ( byte EID = 0; EID < MAX_RELAY_EXPANSION_MODULES; EID++ )
	{
		TempRelay = RelayDataE[EID];
		TempRelay &= RelayMaskOffE[EID];
		TempRelay |= RelayMaskOnE[EID];
		Wire.beginTransmission(I2CExpModule+EID);
		Wire.send(~TempRelay);  // MSB
		Wire.endTransmission();
	}
#endif  // RelayExp
}
