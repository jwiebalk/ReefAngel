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

#include "ReefAngel_Globals.h"
#include <EEPROM.h>


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

// for pure virtual functions
void __cxa_pure_virtual(void){};
// other fixes
int __cxa_guard_acquire(__guard *g) {return !*(char *)(g);};
void __cxa_guard_release (__guard *g) {*(char *)g = 1;};
void __cxa_guard_abort (__guard *) {};
