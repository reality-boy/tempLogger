// https://www.jameco.com/Jameco/workshop/TechTip/temperature-measurement-ntc-thermistors.html

class AnalogTemp
{
public:
  void init(int index, float mag = 1.0f, float offset = 0.0f)
  {
    analogReference(EXTERNAL);
    
    switch(index)
    {
    default:
    case 0: tpin = A0; break;
    case 1: tpin = A1; break;
    case 2: tpin = A2; break;
    case 3: tpin = A3; break;
    case 4: tpin = A4; break;
    case 5: tpin = A5; break;
    }

    cMag = mag;
    cOffset = offset;
  }

  float readTempC()
  {
    return convertTempC(readTempRaw());
  }
  
  float readTempRaw()
  {
    // read multiple times and average to reduce noise
    float val = 0.0f;
    for(int j=0; j<numSamples; j++)
    {
      val += analogRead(tpin);
      //delay(1);  // 10
    }
    val /= numSamples;
    
    return val;
  }

  float convertTempC(float Vo)
  {
    // temp conversion units
    const static int ROther_ohm = 10000; // resistance of other resistor
    const static int RtNom_ohm = 10000; // resistance at 25 C
    const static int tmpNom_C = 25; // 25 C
    const static int BCoef = 3950; // beta coeficient of thermistor
    
    // convert to resistance
    // Vs is source voltage, Vo is output voltage
    // in this case we use unitless 0-1023 range
    // RTherm_ohm = R0 * (( Vs / Vo ) - 1) 
    float RTherm_ohm = ROther_ohm / ((1023.0f / Vo) - 1.0f);
  
    // simple conversion from resistance to temp
    // could do better with more terms
    // using a modified Steinhart equation
    // 1/T = 1/T0 + 1/B * ln( ( adcMax / adcVal ) – 1 )
    float s = 1.0f / (tmpNom_C + 273.15f) + log(RTherm_ohm / RtNom_ohm) / BCoef; 
    float k = 1.0f / s; // invert to get kelvin
    float c = k - 273.15f; // convert to C

    // apply correction factor, if provided
    return c * cMag + cOffset;
  }

protected:  
  float cMag;
  float cOffset;
  const int numSamples = 5;
  int tpin;
};
