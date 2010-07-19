/*
 * Copyright 2010 / Curt Binder
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

#ifndef __REEFANGEL_EEPROM_H__
#define __REEFANGEL_EEPROM_H__

#include <inttypes.h>
#include <EEPROM.h>


class ReefAngel_EEPROMClass : public EEPROMClass {
    public:
        // Functions that read / write a byte (uint8_t)
        uint8_t MHOnHour_read();
        void MHOnHour_write(uint8_t value);
        uint8_t MHOnMinute_read();
        void MHOnMinute_write(uint8_t value);
        uint8_t MHOffHour_read();
        void MHOffHour_write(uint8_t value);
        uint8_t MHOffMinute_read();
        void MHOffMinute_write(uint8_t value);
        uint8_t StdLightsOnHour_read();
        void StdLightsOnHour_write(uint8_t value);
        uint8_t StdLightsOnMinute_read();
        void StdLightsOnMinute_write(uint8_t value);
        uint8_t StdLightsOffHour_read();
        void StdLightsOffHour_write(uint8_t value);
        uint8_t StdLightsOffMinute_read();
        void StdLightsOffMinute_write(uint8_t value);
        uint8_t DP1Timer_read();
        void DP1Timer_write(uint8_t value);
        uint8_t DP2Timer_read();
        void DP2Timer_write(uint8_t value);
        uint8_t LEDPWMDaylight_read();
        void LEDPWMDaylight_write(uint8_t value);
        uint8_t LEDPWMActinic_read();
        void LEDPWMActinic_write(uint8_t value);
        uint8_t ATOTimeout_read();
        void ATOTimeout_write(uint8_t value);
        uint8_t MHDelay_read();
        void MHDelay_write(uint8_t value);
        uint8_t DP1OnHour_read();
        void DP1OnHour_write(uint8_t value);
        uint8_t DP1OnMinute_read();
        void DP1OnMinute_write(uint8_t value);
        uint8_t DP2OnHour_read();
        void DP2OnHour_write(uint8_t value);
        uint8_t DP2OnMinute_read();
        void DP2OnMinute_write(uint8_t value);

        // Functions that read/write an int
        int WM1Timer_read();
        void WM1Timer_write(int value);
        int WM2Timer_read();
        void WM2Timer_write(int value);
        int FeedingTimer_read();
        void FeedingTimer_write(int value);
        int ScreenSaverTimer_read();
        void ScreenSaverTimer_write(int value);
        int OverheatTemp_read();
        void OverheatTemp_write(int value);
        int HeaterTempOn_read();
        void HeaterTempOn_write(int value);
        int HeaterTempOff_read();
        void HeaterTempOff_write(int value);
        int ChillerTempOn_read();
        void ChillerTempOn_write(int value);
        int ChillerTempOff_read();
        void ChillerTempOff_write(int value);
        int PHMax_read();
        void PHMax_write(int value);
        int PHMin_read();
        void PHMin_write(int value);

    private:
        int read_int(int);
        void write_int(int, int);
};

#endif  // __REEFANGEL_EEPROM_H__
