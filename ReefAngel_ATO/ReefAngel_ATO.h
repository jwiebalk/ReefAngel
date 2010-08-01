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

#ifndef __REEFANGEL_ATO_H__
#define __REEFANGEL_ATO_H__

#include <ReefAngel_Globals.h>

/*
    Auto Top Off Class
*/

class ReefAngel_ATOClass
{
public:
	ReefAngel_ATOClass();
    virtual bool IsActive() = 0;
	inline bool IsTopping() { return topping; }
	inline void StartTopping() { topping = true; }
	inline void StopTopping() { topping = false; }
	unsigned long Timer;

private:
    bool topping;
};

class ReefAngel_ATOHighClass : public ReefAngel_ATOClass
{
public:
    inline bool IsActive() { return !digitalRead(highATOPin); }
};

class ReefAngel_ATOLowClass : public ReefAngel_ATOClass
{
public:
    inline bool IsActive() { return !digitalRead(lowATOPin); }
};

#endif  // __REEFANGEL_ATO_H__

