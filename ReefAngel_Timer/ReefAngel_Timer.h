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

#ifndef __REEFANGEL_TIMER_H__
#define __REEFANGEL_TIMER_H__

#include <Time.h>

class ReefAngel_TimerClass
{
public:
	ReefAngel_TimerClass();
	time_t Trigger;

	void Start();
	bool IsTriggered();
	inline void ForceTrigger() { Trigger = 1; }
	inline void SetInterval(int i) { Interval = i; }

private:
    time_t Interval;
};

#endif // __REEFANGEL_TIMER_H__
