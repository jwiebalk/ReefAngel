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

#include "ReefAngel_Timer.h"
#include <ReefAngel_Globals.h>

ReefAngel_TimerClass::ReefAngel_TimerClass()
{
	Trigger = 0;
	Interval = 0;
}

void ReefAngel_TimerClass::Start()
{
	if (Interval)
	{
	    Trigger = now() + Interval;
	}
}

bool ReefAngel_TimerClass::IsTriggered()
{
	if (now()>Trigger && Trigger!=0)
	{
		Trigger = 0;
		return true;
	}
	else
	{
		return false;
	}
}
