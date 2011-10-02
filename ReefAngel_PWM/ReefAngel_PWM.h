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

#ifndef __REEFANGEL_PWM_H__
#define __REEFANGEL_PWM_H__

#include <ReefAngel_Globals.h>

class ReefAngel_PWMClass
{
public:
	ReefAngel_PWMClass();
	void SetActinic(byte value);
	void SetDaylight(byte value);
#ifdef PWMExpansion
	void Expansion(byte cmd, byte data);
	void ExpansionSetPercent(byte p);
#endif  // PWMExpansion
	inline byte GetActinicValue() { return ActinicPWMValue; }
	inline byte GetDaylightValue() { return DaylightPWMValue; }

private:
	byte ActinicPWMValue;
	byte DaylightPWMValue;
};

#endif  // __REEFANGEL_PWM_H__
