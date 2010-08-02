
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

int ReefAngel_EEPROMClass::ScreenSaverTimer_read()
{
    return read_int(Mem_I_ScreenSaverTimer);
}

void ReefAngel_EEPROMClass::ScreenSaverTimer_write(const int value)
{
    write_int(Mem_I_ScreenSaverTimer, value);
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

// Private functions
int ReefAngel_EEPROMClass::read_int(int address)
{
    int value = 0;
    uint8_t* p = (uint8_t*)&value;
    int i;
    for ( i = 0; i < sizeof(int); i++ )
    {
        *p++ = read(address++);
    }
    return value;
}

void ReefAngel_EEPROMClass::write_int(int address, const int value)
{
    uint8_t* p = (uint8_t*)&value;
    for ( int i = 0; i < sizeof(int); i++ )
    {
        write(address++, *p++);
    }
    return;
}
