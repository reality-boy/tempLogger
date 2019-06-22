// uncomment to use lux over uv sensor
#define USE_LUX

// how often to log data, in milliseconds
#define LOG_INTERVAL (1 * 60UL * 1000UL) // log once a minute

#include "DataLogger.h"
#include "AnalogTemp.h"
#include "DigitalTemp.h"

#ifdef USE_LUX
# include "LuxLight.h"
#else
# include "UVLight.h"
#endif

DataLogger logger;

const int ANALOG_TEMP_COUNT = 4;
AnalogTemp analogTemp[ANALOG_TEMP_COUNT];
float aTempMag[ANALOG_TEMP_COUNT] = {1.0961f, 1.0979f,  1.0972f,  1.0875f};
float aTempOff[ANALOG_TEMP_COUNT] = {-0.9220f, -1.5225f, -1.3282f, -1.5116f};

const int DIGITAL_TEMP_COUNT = 1; //8;
DigitalTemp digitalTemp[DIGITAL_TEMP_COUNT];

#ifdef USE_LUX
LuxLight luxLight;
#else
UVLight uvLight;
#endif

void setup() 
{
  Serial.begin(115200);

  logger.init();

  for(int i=0; i<ANALOG_TEMP_COUNT; i++)
  {
    analogTemp[i].init(i, aTempMag[i], aTempOff[i]);
    logger.addString("aRaw_", i);
    logger.addString("aTempC_", i);
  }

  for(int i=0; i<DIGITAL_TEMP_COUNT; i++)
  {
    digitalTemp[i].init(i);
    logger.addString("dTempC_", i);
  }

#ifdef USE_LUX
  luxLight.init();
  logger.addString("luxLux");
  logger.addString("luxWhite");
  logger.addString("luxALS");
#else
  uvLight.init();
  logger.addString("uvUVA");
  logger.addString("uvUVB");
  logger.addString("uvUVI");
#endif

  logger.endLine();
}

void loop() 
{
  // calculate time to next log
  unsigned long logNext = millis() + LOG_INTERVAL;
  
  logger.beginLine();
  
  for(int i=0; i<ANALOG_TEMP_COUNT; i++)
  {
    int r = analogTemp[i].readTempRaw();
    logger.addInt(r);
    logger.addFloat(analogTemp[i].convertTempC(r));
  }

  for(int i=0; i<DIGITAL_TEMP_COUNT; i++)
    logger.addFloat(digitalTemp[i].readTempC());

#ifdef USE_LUX
  logger.addFloat(luxLight.readLux());
  logger.addFloat(luxLight.readWhite());
  logger.addInt(luxLight.readALS());
#else
  logger.addFloat(uvLight.readUVA());
  logger.addFloat(uvLight.readUVB());
  logger.addFloat(uvLight.readUVI());
#endif

  logger.endLine();

  // delay till next log
  // do it this way so loop time does not effect interval
  unsigned long logDelay = logNext - millis();
  if(logDelay > 0)
    delay(logDelay);
}




//----------------
