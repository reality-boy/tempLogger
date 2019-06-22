// https://learn.adafruit.com/adafruit-veml7700?view=all

#include <Wire.h>
#include "Adafruit_VEML7700.h"

class LuxLight
{
public:
  void init() 
  {
    if(veml.begin())
    {
      veml.setGain(VEML7700_GAIN_1);
      // VEML7700_GAIN_1_8: // 1/8
      // VEML7700_GAIN_1_4  // 1/4
      // VEML7700_GAIN_1    // 1
      // VEML7700_GAIN_2    // 2
  
      veml.setIntegrationTime(VEML7700_IT_800MS);
      // VEML7700_IT_25MS  // 25 ms
      // VEML7700_IT_50MS  // 50 ms
      // VEML7700_IT_100MS // 100 ms
      // VEML7700_IT_200MS // 200 ms
      // VEML7700_IT_400MS // 400 ms
      // VEML7700_IT_800MS // 800 ms
  
      //veml.powerSaveEnable(true);
      //veml.setPowerSaveMode(VEML7700_POWERSAVE_MODE4);
    }
  }

  float readLux() { return veml.readLux(); }
  float readWhite() { return veml.readWhite(); }
  int readALS() { return veml.readALS(); }

protected:
  Adafruit_VEML7700 veml;
};
