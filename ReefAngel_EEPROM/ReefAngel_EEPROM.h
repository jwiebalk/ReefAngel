/*
 * Copyright 2010 Curt Binder
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
#include <avr/eeprom.h>

/*
This class reads/writes to the internal EEPROM memory
*/

class ReefAngel_EEPROMClass {
    public:
        // Functions that read / write a byte (uint8_t)
        uint8_t MHOnHour_read();
        void MHOnHour_write(const uint8_t value);
        uint8_t MHOnMinute_read();
        void MHOnMinute_write(const uint8_t value);
        uint8_t MHOffHour_read();
        void MHOffHour_write(const uint8_t value);
        uint8_t MHOffMinute_read();
        void MHOffMinute_write(const uint8_t value);
        uint8_t StdLightsOnHour_read();
        void StdLightsOnHour_write(const uint8_t value);
        uint8_t StdLightsOnMinute_read();
        void StdLightsOnMinute_write(const uint8_t value);
        uint8_t StdLightsOffHour_read();
        void StdLightsOffHour_write(const uint8_t value);
        uint8_t StdLightsOffMinute_read();
        void StdLightsOffMinute_write(const uint8_t value);
        uint8_t DP1Timer_read();
        void DP1Timer_write(const uint8_t value);
        uint8_t DP2Timer_read();
        void DP2Timer_write(const uint8_t value);
        uint8_t LEDPWMDaylight_read();
        void LEDPWMDaylight_write(const uint8_t value);
        uint8_t LEDPWMActinic_read();
        void LEDPWMActinic_write(const uint8_t value);
        uint8_t ATOTimeout_read();
        void ATOTimeout_write(const uint8_t value);
        uint8_t ATOHighTimeout_read();
        void ATOHighTimeout_write(const uint8_t value);
        uint8_t ATOHourInterval_read();
        void ATOHourInterval_write(const uint8_t value);
        uint8_t ATOHighHourInterval_read();
        void ATOHighHourInterval_write(const uint8_t value);
        uint8_t MHDelay_read();
        void MHDelay_write(const uint8_t value);
        uint8_t DP1OnHour_read();
        void DP1OnHour_write(const uint8_t value);
        uint8_t DP1OnMinute_read();
        void DP1OnMinute_write(const uint8_t value);
        uint8_t DP2OnHour_read();
        void DP2OnHour_write(const uint8_t value);
        uint8_t DP2OnMinute_read();
        void DP2OnMinute_write(const uint8_t value);
        uint8_t T1Pointer_read();
        void T1Pointer_write(const uint8_t value);
        uint8_t PWMSlopeStartD_read();
        void PWMSlopeStartD_write(const uint8_t value);
        uint8_t PWMSlopeEndD_read();
        void PWMSlopeEndD_write(const uint8_t value);
        uint8_t PWMSlopeDurationD_read();
        void PWMSlopeDurationD_write(const uint8_t value);
        uint8_t PWMSlopeStartA_read();
        void PWMSlopeStartA_write(const uint8_t value);
        uint8_t PWMSlopeEndA_read();
        void PWMSlopeEndA_write(const uint8_t value);
        uint8_t PWMSlopeDurationA_read();
        void PWMSlopeDurationA_write(const uint8_t value);
        uint8_t RFMode_read();
        void RFMode_write(const uint8_t value);
        uint8_t RFSpeed_read();
        void RFSpeed_write(const uint8_t value);
        uint8_t RFDuration_read();
        void RFDuration_write(const uint8_t value);

        // Functions that read/write an int
        int WM1Timer_read();
        void WM1Timer_write(const int value);
        int WM2Timer_read();
        void WM2Timer_write(const int value);
        int FeedingTimer_read();
        void FeedingTimer_write(const int value);
        int LCDTimer_read();
        void LCDTimer_write(const int value);
        int OverheatTemp_read();
        void OverheatTemp_write(const int value);
        int HeaterTempOn_read();
        void HeaterTempOn_write(const int value);
        int HeaterTempOff_read();
        void HeaterTempOff_write(const int value);
        int ChillerTempOn_read();
        void ChillerTempOn_write(const int value);
        int ChillerTempOff_read();
        void ChillerTempOff_write(const int value);
        int PHMax_read();
        void PHMax_write(const int value);
        int PHMin_read();
        void PHMin_write(const int value);
        int DP1RepeatInterval_read();
        void DP1RepeatInterval_write(const int value);
        int DP2RepeatInterval_read();
        void DP2RepeatInterval_write(const int value);
        int SalMax_read();
        void SalMax_write(const int value);

        // Functions that do the reading/writing to memory
        uint8_t read(uint8_t);
        void write(uint8_t, const uint8_t);
        int read_int(int);
        void write_int(int, const int);
        uint32_t read_dword(int);
        void write_dword(int, const uint32_t);
};

extern ReefAngel_EEPROMClass InternalMemory;

#endif  // __REEFANGEL_EEPROM_H__
