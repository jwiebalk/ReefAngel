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

#include "ReefAngel_RF.h"
#include <ReefAngel_Globals.h>
#include <Wire.h>

ReefAngel_RFClass::ReefAngel_RFClass()
{
}

void ReefAngel_RFClass::SetMode(byte mode, byte speed, byte duration)
{
  Wire.beginTransmission(I2CRF); // transmit to device #16
  Wire.send('$');              // sends $ 
  Wire.send('$');              // sends $ 
  Wire.send('$');              // sends $ 
  Wire.send(mode);              // sends mode 
  Wire.send(speed);              // sends speed 
  Wire.send(duration);              // sends duration 
  Wire.endTransmission();    // stop transmitting
}

byte ReefAngel_RFClass::RFCheck()
{
  Wire.requestFrom(I2CRF,1);
  if (Wire.available())
  {
    return Wire.receive();
  }
}

