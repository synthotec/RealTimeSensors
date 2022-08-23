//James Prince string thermometers Tool temp monitoring.//
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

void setup(void)
{
  Serial.begin(115200);
  sensors.begin();
}

void loop(void){
  sensors.requestTemperatures();
String outputString = "{\"TemperatureSensors\":[";
sensors.begin();
  for (uint8_t i = 0; i < sensors.getDeviceCount(); i++)
    {
    float tempC = sensors.getTempCByIndex(i);

      if(i>0)
        {
        outputString+=",";
        }
        
      outputString+="{\"Index\":";
      outputString+=i;
      outputString+=", \"CelciusValue\":";
      outputString+=tempC;
      outputString+="}";      
    }
  outputString+="]}";
  Serial.println(outputString);
  
  digitalWrite(LED_BUILTIN, LOW); 
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);


}
