

// how often to log data, in minutes
const float logMinutes = 1.0f; // log once a minute

// log temps as read, or apply magnitude and offset correctoin
const bool applyCorrection = true;

// uncomment to use various sensors
#define USE_DIGITAL
#define USE_ANALOG
//#define USE_LUX
//#define USE_UV




#ifdef USE_ANALOG
#include "AnalogTemp.h"
const int analogTempCount = 4;
const float aTempMag[analogTempCount] = { 1.051f,  1.046f,  1.058f,  1.053f };
const float aTempOff[analogTempCount] = {-1.462f, -2.042f, -2.200f, -2.111f };
AnalogTemp analogTemp[analogTempCount];
const bool logRaw = false; // enable to log raw DAC values
#endif

#ifdef USE_DIGITAL
#include "DigitalTemp.h"
const int digitalTempCount = 7; //8;
const float dTempMag[digitalTempCount] = { 1.000f,  1.001f,  1.014f,  1.009f,  1.009f,  1.015f,  1.007f };
const float dTempOff[digitalTempCount] = { 0.000f, -0.222f, -0.447f, -0.538f, -0.601f, -0.750f, -0.450f };
DigitalTemp digitalTemp[digitalTempCount];
#endif

#ifdef USE_LUX
#include "LuxLight.h"
LuxLight luxLight;
#endif

#ifdef USE_UV
#include "UVLight.h"
UVLight uvLight;
#endif

#include "DataLogger.h"
const unsigned long logMills = (unsigned long)(logMinutes * 60UL * 1000UL);
DataLogger logger;



void setup() 
{
  Serial.begin(115200);

  logger.init();

#ifdef USE_ANALOG
  for(int i=0; i<analogTempCount; i++)
  {
    if(applyCorrection)
      analogTemp[i].init(i, aTempMag[i], aTempOff[i]);
    else
      analogTemp[i].init(i);

    if(logRaw)
      logger.addString("aRaw_", i);
    else
      logger.addString("aTempC_", i);
  }
#endif

#ifdef USE_DIGITAL
  for(int i=0; i<digitalTempCount; i++)
  {
    if(applyCorrection)
      digitalTemp[i].init(i, dTempMag[i], dTempOff[i]);
    else
      digitalTemp[i].init(i);

    logger.addString("dTempC_", i);
  }
#endif

#ifdef USE_LUX
  luxLight.init();
  logger.addString("luxLux");
  logger.addString("luxWhite");
  logger.addString("luxALS");
#endif

#ifdef USE_UV
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
  unsigned long logNext = millis() + logMills;
  
  logger.beginLine();

#ifdef USE_ANALOG
  for(int i=0; i<analogTempCount; i++)
  {
    int r = analogTemp[i].readTempRaw();
    if(logRaw)
      logger.addInt(r);
    else
      logger.addFloat(analogTemp[i].convertTempC(r));
  }
#endif

#ifdef USE_DIGITAL
  for(int i=0; i<digitalTempCount; i++)
    logger.addFloat(digitalTemp[i].readTempC());
#endif

#ifdef USE_LUX
  logger.addFloat(luxLight.readLux());
  logger.addFloat(luxLight.readWhite());
  logger.addInt(luxLight.readALS());
#endif

#ifdef USE_UV
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
