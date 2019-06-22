#include <SPI.h>
#include <SD.h>

#include <Wire.h>
#include "RTClib.h"

class DataLogger
{
public:
  void init()
  {
    //----------------
    // init local variables
    
    //logFile = NULL;
    syncTime = 0;

    //----------------
    // setup date/time

    //Wire.begin();  
    if(!rtc.begin()) 
      Serial.println("Real time clock failed to initialize!");
  
    if (! rtc.isrunning()) 
    {
      Serial.println("Real time clock is NOT running!");
      // following line sets the RTC to the date & time this sketch was compiled
      // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    //----------------
    // setup and open file

    if(!SD.begin(chipSelect)) 
      Serial.println("SD card failed, or not present");

    createFile();
  }

  void beginLine()
  {
    unsigned long ms = millis();

    // log milliseconds since starting
    logFile.print(ms / 1000);
    Serial.print(ms / 1000);

    DateTime now = rtc.now();

    // log date
    logFile.print(",");
    logFile.print(now.year(), DEC);
    logFile.print("/");
    logFile.print(now.month(), DEC);
    logFile.print("/");
    logFile.print(now.day(), DEC);

    Serial.print(",");
    Serial.print(now.year(), DEC);
    Serial.print("/");
    Serial.print(now.month(), DEC);
    Serial.print("/");
    Serial.print(now.day(), DEC);

    // log time
    logFile.print(",");
    logFile.print(now.hour(), DEC);
    logFile.print(":");
    logFile.print(now.minute(), DEC);
    logFile.print(":");
    logFile.print(now.second(), DEC);

    Serial.print(",");
    Serial.print(now.hour(), DEC);
    Serial.print(":");
    Serial.print(now.minute(), DEC);
    Serial.print(":");
    Serial.print(now.second(), DEC);
  }
  
  void addFloat(float f)
  {
    logFile.print(",");
    logFile.print(f);

    Serial.print(",");
    Serial.print(f);
  }
  
  void addInt(int i)
  {
    logFile.print(",");
    logFile.print(i);

    Serial.print(",");
    Serial.print(i);
  }

  void addString(const char *s, int i=-1)
  {
    logFile.print(",");
    logFile.print(s);
    if(i >= 0)
      logFile.print(i);

    Serial.print(",");
    Serial.print(s);
    if(i >= 0)
      Serial.print(i);
  }

  void endLine()
  {
    logFile.println();

    Serial.println();

    // Now we write data to disk! 
    // Don't sync too often - requires 2048 bytes of I/O to SD card
    // which uses a bunch of power and takes time
    if(millis() > syncTime)
    {
      syncTime = millis() + syncInterval;
      logFile.flush();
    }
  }

protected:
  void createFile()
  {
    // create a new file
    char filename[] = "LOGGER00.CSV";
    for(int i=0; i<100; i++) 
    {
      filename[6] = i / 10 + '0';
      filename[7] = i % 10 + '0';
      if(!SD.exists(filename)) 
      {
        // only open a new file if it doesn't exist
        logFile = SD.open(filename, FILE_WRITE); 
        break;  // leave the loop!
      }
    }
  
    // log file name
    if(!logFile)
    {
      Serial.print("Error: could not create file");
      Serial.println(filename);
    }
    else
    {
      Serial.print("Logging to: ");
      Serial.println(filename);

      // fill in beginning of header
      logFile.write("sec,date,time_HMS");

      Serial.write("sec,date,time_HMS");
    }
  }

  RTC_DS1307 rtc;
  File logFile;
  unsigned long syncTime;

  // for the data logging shield, we use digital pin 10 for the SD cs line
  const int chipSelect = 10; //4;
 
  // flush file every 30 seconds 
  const int syncInterval = 30 * 1000; 
};
