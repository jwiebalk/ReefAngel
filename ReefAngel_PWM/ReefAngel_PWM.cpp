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

#include "ReefAngel_PWM.h"
#include <ReefAngel_Globals.h>
#include <Wire.h>

ReefAngel_PWMClass::ReefAngel_PWMClass()
{
    // initialize variables
    ActinicPWMValue = 0;
    DaylightPWMValue = 0;
}

void ReefAngel_PWMClass::SetActinic(byte value)
{
    analogWrite(actinicPWMPin, value*2.55);
    ActinicPWMValue = value;

}

void ReefAngel_PWMClass::SetDaylight(byte value)
{
    analogWrite(daylightPWMPin, value*2.55);
    DaylightPWMValue = value;
}

#ifdef PWMEXPANSION
void ReefAngel_PWMClass::Expansion(byte cmd, byte data)
{
	Wire.beginTransmission(8);  // transmit to device #2, consider having this user defined possibly
	Wire.send('$');				// send the $$$
	Wire.send('$');
	Wire.send('$');
	Wire.send(cmd);				// send the command
	Wire.send(data);			// send the data
	Wire.endTransmission();		// stop transmitting
}

void ReefAngel_PWMClass::ExpansionSetPercent(byte p)
{
	// loop through all 6 channels and send the value
	for ( byte a = 0; a < 6; a++ )
	{
		Expansion(a, int(2.55*p));
	}
}
#endif  // PWMEXPANSION
