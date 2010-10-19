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

#ifndef __REEFANGEL_TEMPSENSOR_H__
#define __REEFANGEL_TEMPSENSOR_H__

#include <ReefAngel_Globals.h>

class ReefAngel_TempSensorClass
{
public:
	ReefAngel_TempSensorClass();
	void Init();
	void RequestConvertion();
	int ReadTemperature(byte addr[8]);

	byte addrT1[8];
	byte addrT2[8];
	byte addrT3[8];
	byte unit;

private:
};

#endif // __REEFANGEL_TEMPSENSOR_H__
