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

#include "ReefAngel_AI.h"
#include <ReefAngel_Globals.h>


ReefAngel_AIClass::ReefAngel_AIClass()
{
	AIChannels[0]=0;
	AIChannels[1]=0;
	AIChannels[2]=0;
	StreamDelay=300;
	AImillis=millis();
}

void ReefAngel_AIClass::SetPort(byte portnum)
{
	port=portnum;
	pinMode(port,OUTPUT);
	digitalWrite(port,LOW);
}

void ReefAngel_AIClass::SetChannel(byte channel, byte intensity)
{
	AIChannels[channel]=intensity;
}

byte ReefAngel_AIClass::GetChannel(byte channel)
{
	return AIChannels[channel];
}

void ReefAngel_AIClass::SetStreamDelay(int delay)
{
	StreamDelay=delay;
}

void ReefAngel_AIClass::Send()
{
	for (int a=0;a<3;a++)
	{
		SWprint(164);
		SWprint(161);
		SWprint(114);
		SWprint(RawChannel[a]);
		SWprint(AIChannels[a]*2.55);
	}
}

void ReefAngel_AIClass::SWprint(byte data)
{
  byte mask;
  //startbit
  digitalWrite(port,HIGH);
  delayMicroseconds(bit9600Delay);
  for (mask = 0x01; mask>0; mask <<= 1) {
    if (data & mask){ // choose bit
     digitalWrite(port,LOW); // send 1
    }
    else{
     digitalWrite(port,HIGH); // send 0
    }
    delayMicroseconds(bit9600Delay);
  }
  //stop bit
  digitalWrite(port, LOW);
  delayMicroseconds(bit9600Delay);
}
