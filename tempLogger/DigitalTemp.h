// https://learn.adafruit.com/adafruit-mcp9808-precision-i2c-temperature-sensor-guide?view=all

#include <Wire.h>
#include "Adafruit_MCP9808.h"

class DigitalTemp
{
public:
  void init(int index, float mag = 1.0f, float offset = 0.0f) 
  {
    if(dt.begin(0x18 + index)) 
    {
      dt.setResolution(3); // sets the resolution mode of reading, the modes are defined in the table bellow:
      // Mode Resolution SampleTime
      //  0    0.5°C       30 ms
      //  1    0.25°C      65 ms
      //  2    0.125°C     130 ms
      //  3    0.0625°C    250 ms
      
      dt.wake();
    }

    cMag = mag;
    cOffset = offset;
  }
  
  float readTempC() 
  { 
    // apply correction factor, if provided
    return dt.readTempC() * cMag + cOffset;
  }

protected:
  float cMag;
  float cOffset;
  Adafruit_MCP9808 dt;
};
