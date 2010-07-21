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
	// TODO TimerClass move public vars to private and create functions for them
	// Maybe create ForceTrigger that sets value to 1
	// Maybe create ZeroTrigger that sets value to 0
	// Maybe create IsTriggerZeroed, returns true if 0, false otherwise
	// Just trying to prevent direct access to Trigger variable to make code easy to read
	//   and follow instead of having to know what values to set it to get desired response
	time_t Trigger;

	void Start();
	bool IsTriggered();
	void ForceTrigger() { Trigger = 1; }
	void SetInterval(time_t i) { Interval = i; }
	bool HasRunToday();  // TODO TimerClass remove fn
	void ResetHasRun();  // TODO TimerClass remove fn
	bool IsRunning();

private:
    bool HasRun; // TODO TimerClass remove var
    bool IsRun;
    time_t Interval;
};

#endif // __REEFANGEL_TIMER_H__
