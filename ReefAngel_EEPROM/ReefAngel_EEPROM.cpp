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

#include "ReefAngel_EEPROM.h"
#include <ReefAngel_Globals.h>

// Byte Functions
uint8_t ReefAngel_EEPROMClass::MHOnHour_read()
{
    return read(Mem_B_MHOnHour);
}

void ReefAngel_EEPROMClass::MHOnHour_write(const uint8_t value)
{
    write(Mem_B_MHOnHour, value);
}

uint8_t ReefAngel_EEPROMClass::MHOnMinute_read()
{
    return read(Mem_B_MHOnMinute);
}

void ReefAngel_EEPROMClass::MHOnMinute_write(const uint8_t value)
{
    write(Mem_B_MHOnMinute, value);
}

uint8_t ReefAngel_EEPROMClass::MHOffHour_read()
{
    return read(Mem_B_MHOffHour);
}

void ReefAngel_EEPROMClass::MHOffHour_write(const uint8_t value)
{
    write(Mem_B_MHOffHour, value);
}

uint8_t ReefAngel_EEPROMClass::MHOffMinute_read()
{
    return read(Mem_B_MHOffMinute);
}

void ReefAngel_EEPROMClass::MHOffMinute_write(const uint8_t value)
{
    write(Mem_B_MHOffMinute, value);
}

uint8_t ReefAngel_EEPROMClass::StdLightsOnHour_read()
{
    return read(Mem_B_StdLightsOnHour);
}

void ReefAngel_EEPROMClass::StdLightsOnHour_write(const uint8_t value)
{
    write(Mem_B_StdLightsOnHour, value);
}

uint8_t ReefAngel_EEPROMClass::StdLightsOnMinute_read()
{
    return read(Mem_B_StdLightsOnMinute);
}

void ReefAngel_EEPROMClass::StdLightsOnMinute_write(const uint8_t value)
{
    write(Mem_B_StdLightsOnMinute, value);
}

uint8_t ReefAngel_EEPROMClass::StdLightsOffHour_read()
{
    return read(Mem_B_StdLightsOffHour);
}

void ReefAngel_EEPROMClass::StdLightsOffHour_write(const uint8_t value)
{
    write(Mem_B_StdLightsOffHour, value);
}

uint8_t ReefAngel_EEPROMClass::StdLightsOffMinute_read()
{
    return read(Mem_B_StdLightsOffMinute);
}

void ReefAngel_EEPROMClass::StdLightsOffMinute_write(const uint8_t value)
{
    write(Mem_B_StdLightsOffMinute, value);
}

uint8_t ReefAngel_EEPROMClass::DP1Timer_read()
{
    return read(Mem_B_DP1Timer);
}

void ReefAngel_EEPROMClass::DP1Timer_write(const uint8_t value)
{
    write(Mem_B_DP1Timer, value);
}

uint8_t ReefAngel_EEPROMClass::DP2Timer_read()
{
    return read(Mem_B_DP2Timer);
}

void ReefAngel_EEPROMClass::DP2Timer_write(const uint8_t value)
{
    write(Mem_B_DP2Timer, value);
}

uint8_t ReefAngel_EEPROMClass::LEDPWMDaylight_read()
{
    return read(Mem_B_LEDPWMDaylight);
}

void ReefAngel_EEPROMClass::LEDPWMDaylight_write(const uint8_t value)
{
    write(Mem_B_LEDPWMDaylight, value);
}

uint8_t ReefAngel_EEPROMClass::LEDPWMActinic_read()
{
    return read(Mem_B_LEDPWMActinic);
}

void ReefAngel_EEPROMClass::LEDPWMActinic_write(const uint8_t value)
{
    write(Mem_B_LEDPWMActinic, value);
}

uint8_t ReefAngel_EEPROMClass::ATOTimeout_read()
{
    return read(Mem_B_ATOTimeout);
}

void ReefAngel_EEPROMClass::ATOTimeout_write(const uint8_t value)
{
    write(Mem_B_ATOTimeout, value);
}

uint8_t ReefAngel_EEPROMClass::ATOHighTimeout_read()
{
    return read(Mem_B_ATOHighTimeout);
}

void ReefAngel_EEPROMClass::ATOHighTimeout_write(const uint8_t value)
{
    write(Mem_B_ATOHighTimeout, value);
}

uint8_t ReefAngel_EEPROMClass::ATOHourInterval_read()
{
    return read(Mem_B_ATOHourInterval);
}

void ReefAngel_EEPROMClass::ATOHourInterval_write(const uint8_t value)
{
    write(Mem_B_ATOHourInterval, value);
}

uint8_t ReefAngel_EEPROMClass::ATOHighHourInterval_read()
{
    return read(Mem_B_ATOHighHourInterval);
}

void ReefAngel_EEPROMClass::ATOHighHourInterval_write(const uint8_t value)
{
    write(Mem_B_ATOHighHourInterval, value);
}

uint8_t ReefAngel_EEPROMClass::MHDelay_read()
{
    return read(Mem_B_MHDelay);
}

void ReefAngel_EEPROMClass::MHDelay_write(const uint8_t value)
{
    write(Mem_B_MHDelay, value);
}

uint8_t ReefAngel_EEPROMClass::DP1OnHour_read()
{
    return read(Mem_B_DP1OnHour);
}

void ReefAngel_EEPROMClass::DP1OnHour_write(const uint8_t value)
{
    write(Mem_B_DP1OnHour, value);
}

uint8_t ReefAngel_EEPROMClass::DP1OnMinute_read()
{
    return read(Mem_B_DP1OnMinute);
}

void ReefAngel_EEPROMClass::DP1OnMinute_write(uint8_t value)
{
    write(Mem_B_DP1OnMinute, value);
}

uint8_t ReefAngel_EEPROMClass::DP2OnHour_read()
{
    return read(Mem_B_DP2OnHour);
}

void ReefAngel_EEPROMClass::DP2OnHour_write(const uint8_t value)
{
    write(Mem_B_DP2OnHour, value);
}

uint8_t ReefAngel_EEPROMClass::DP2OnMinute_read()
{
    return read(Mem_B_DP2OnMinute);
}

void ReefAngel_EEPROMClass::DP2OnMinute_write(const uint8_t value)
{
    write(Mem_B_DP2OnMinute, value);
}

uint8_t ReefAngel_EEPROMClass::T1Pointer_read()
{
    return read(T1Pointer);
}

void ReefAngel_EEPROMClass::T1Pointer_write(const uint8_t value)
{
    write(T1Pointer, value);
}

uint8_t ReefAngel_EEPROMClass::PWMSlopeStartD_read()
{
    return read(Mem_B_PWMSlopeStartD);
}

void ReefAngel_EEPROMClass::PWMSlopeStartD_write(const uint8_t value)
{
    write(Mem_B_PWMSlopeStartD, value);
}

uint8_t ReefAngel_EEPROMClass::PWMSlopeEndD_read()
{
    return read(Mem_B_PWMSlopeEndD);
}

void ReefAngel_EEPROMClass::PWMSlopeEndD_write(const uint8_t value)
{
    write(Mem_B_PWMSlopeEndD, value);
}

uint8_t ReefAngel_EEPROMClass::PWMSlopeDurationD_read()
{
    return read(Mem_B_PWMSlopeDurationD);
}

void ReefAngel_EEPROMClass::PWMSlopeDurationD_write(const uint8_t value)
{
    write(Mem_B_PWMSlopeDurationD, value);
}

uint8_t ReefAngel_EEPROMClass::PWMSlopeStartA_read()
{
    return read(Mem_B_PWMSlopeStartA);
}

void ReefAngel_EEPROMClass::PWMSlopeStartA_write(const uint8_t value)
{
    write(Mem_B_PWMSlopeStartA, value);
}

uint8_t ReefAngel_EEPROMClass::PWMSlopeEndA_read()
{
    return read(Mem_B_PWMSlopeEndA);
}

void ReefAngel_EEPROMClass::PWMSlopeEndA_write(const uint8_t value)
{
    write(Mem_B_PWMSlopeEndA, value);
}

uint8_t ReefAngel_EEPROMClass::PWMSlopeDurationA_read()
{
    return read(Mem_B_PWMSlopeDurationA);
}

void ReefAngel_EEPROMClass::PWMSlopeDurationA_write(const uint8_t value)
{
    write(Mem_B_PWMSlopeDurationA, value);
}

uint8_t ReefAngel_EEPROMClass::RFMode_read()
{
    return read(Mem_B_RFMode);
}

void ReefAngel_EEPROMClass::RFMode_write(const uint8_t value)
{
    write(Mem_B_RFMode, value);
}

uint8_t ReefAngel_EEPROMClass::RFSpeed_read()
{
    return read(Mem_B_RFSpeed);
}

void ReefAngel_EEPROMClass::RFSpeed_write(const uint8_t value)
{
    write(Mem_B_RFSpeed, value);
}

uint8_t ReefAngel_EEPROMClass::RFDuration_read()
{
    return read(Mem_B_RFDuration);
}

void ReefAngel_EEPROMClass::RFDuration_write(const uint8_t value)
{
    write(Mem_B_RFDuration, value);
}

// Int Functions
int ReefAngel_EEPROMClass::WM1Timer_read()
{
    return read_int(Mem_I_WM1Timer);
}

void ReefAngel_EEPROMClass::WM1Timer_write(const int value)
{
    write_int(Mem_I_WM1Timer, value);
}

int ReefAngel_EEPROMClass::WM2Timer_read()
{
    return read_int(Mem_I_WM2Timer);
}

void ReefAngel_EEPROMClass::WM2Timer_write(const int value)
{
    write_int(Mem_I_WM2Timer, value);
}

int ReefAngel_EEPROMClass::FeedingTimer_read()
{
    return read_int(Mem_I_FeedingTimer);
}

void ReefAngel_EEPROMClass::FeedingTimer_write(int value)
{
    write_int(Mem_I_FeedingTimer, value);
}

int ReefAngel_EEPROMClass::LCDTimer_read()
{
    return read_int(Mem_I_LCDTimer);
}

void ReefAngel_EEPROMClass::LCDTimer_write(const int value)
{
    write_int(Mem_I_LCDTimer, value);
}

int ReefAngel_EEPROMClass::OverheatTemp_read()
{
    return read_int(Mem_I_OverheatTemp);
}

void ReefAngel_EEPROMClass::OverheatTemp_write(const int value)
{
    write_int(Mem_I_OverheatTemp, value);
}

int ReefAngel_EEPROMClass::HeaterTempOn_read()
{
    return read_int(Mem_I_HeaterTempOn);
}

void ReefAngel_EEPROMClass::HeaterTempOn_write(const int value)
{
    write_int(Mem_I_HeaterTempOn, value);
}

int ReefAngel_EEPROMClass::HeaterTempOff_read()
{
    return read_int(Mem_I_HeaterTempOff);
}

void ReefAngel_EEPROMClass::HeaterTempOff_write(int value)
{
    write_int(Mem_I_HeaterTempOff, value);
}

int ReefAngel_EEPROMClass::ChillerTempOn_read()
{
    return read_int(Mem_I_ChillerTempOn);
}

void ReefAngel_EEPROMClass::ChillerTempOn_write(const int value)
{
    write_int(Mem_I_ChillerTempOn, value);
}

int ReefAngel_EEPROMClass::ChillerTempOff_read()
{
    return read_int(Mem_I_ChillerTempOff);
}

void ReefAngel_EEPROMClass::ChillerTempOff_write(int value)
{
    write_int(Mem_I_ChillerTempOff, value);
}

int ReefAngel_EEPROMClass::PHMax_read()
{
    return read_int(Mem_I_PHMax);
}

void ReefAngel_EEPROMClass::PHMax_write(const int value)
{
    write_int(Mem_I_PHMax, value);
}

int ReefAngel_EEPROMClass::PHMin_read()
{
    return read_int(Mem_I_PHMin);
}

void ReefAngel_EEPROMClass::PHMin_write(const int value)
{
    write_int(Mem_I_PHMin, value);
}

int ReefAngel_EEPROMClass::DP1RepeatInterval_read()
{
	return read_int(Mem_I_DP1RepeatInterval);
}

void ReefAngel_EEPROMClass::DP1RepeatInterval_write(const int value)
{
	write_int(Mem_I_DP1RepeatInterval, value);
}

int ReefAngel_EEPROMClass::DP2RepeatInterval_read()
{
	return read_int(Mem_I_DP2RepeatInterval);
}

void ReefAngel_EEPROMClass::DP2RepeatInterval_write(const int value)
{
	write_int(Mem_I_DP2RepeatInterval, value);
}

int ReefAngel_EEPROMClass::SalMax_read()
{
    return read_int(Mem_I_SalMax);
}

void ReefAngel_EEPROMClass::SalMax_write(const int value)
{
    write_int(Mem_I_SalMax, value);
}

// Private functions
uint8_t ReefAngel_EEPROMClass::read(uint8_t address)
{
    return eeprom_read_byte((unsigned char *) address);
}

void ReefAngel_EEPROMClass::write(uint8_t address, const uint8_t value)
{
    eeprom_write_byte((unsigned char *) address, value);
}

int ReefAngel_EEPROMClass::read_int(int address)
{
    return eeprom_read_word((const uint16_t *) address);
}

void ReefAngel_EEPROMClass::write_int(int address, const int value)
{
    eeprom_write_word((uint16_t *) address, (uint16_t) value);
}

uint32_t ReefAngel_EEPROMClass::read_dword(int address)
{
	return eeprom_read_dword((const uint32_t *)address);
}

void ReefAngel_EEPROMClass::write_dword(int address, const uint32_t value)
{
	eeprom_write_dword((uint32_t *) address, (uint32_t) value);
}

ReefAngel_EEPROMClass InternalMemory;
