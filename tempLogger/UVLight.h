// https://learn.adafruit.com/adafruit-veml6075-uva-uvb-uv-index-sensor?view=all

#include <Wire.h> 
#include "Adafruit_VEML6075.h"

class UVLight
{
public:

  void init() 
  {
    if(uv.begin()) 
    {
      // Set the integration constant
      uv.setIntegrationTime(VEML6075_100MS);
      // VEML6075_50MS  // 50 ms
      // VEML6075_100MS // 100 ms
      // VEML6075_200MS // 200 ms
      // VEML6075_400MS // 400 ms
      // VEML6075_800MS // 800 ms
  
      uv.setHighDynamic(true);
      uv.setForcedMode(false);
      uv.setCoefficients(2.22, 1.33,  // UVA_A and UVA_B coefficients
                         2.95, 1.74,  // UVB_C and UVB_D coefficients
                         0.001461, 0.002591); // UVA and UVB responses
    }
  }
  
  float readUVA() { return uv.readUVA(); }
  float readUVB() { return uv.readUVB(); }
  float readUVI() { return uv.readUVI(); } 

protected:
  Adafruit_VEML6075 uv;
};
