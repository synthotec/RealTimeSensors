#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4

OneWire OneWire(ONE_WIRE_BUS);

DallasTemperature TemperatureSensors(&OneWire);

void setup(void)
{
  Serial.begin(115200);
}

void loop(void){
  TemperatureSensors.requestTemperatures();
String outputString = "{\"TemperatureSensors\":[";
TemperatureSensors.begin();
  for (int index = 0; index < TemperatureSensors.getDeviceCount(); index++)
    {
    float tempC = TemperatureSensors.getTempCByIndex(index);

      if(index>0)
        {
        outputString+=",";
        }
        
      outputString+="{\"Index\":";
      outputString+=index;
      outputString+=", \"CelciusValue\":";
      outputString+=tempC;
      outputString+="}";      
    }
  outputString+="]}";
  Serial.println(outputString);
  
  digitalWrite(LED_BUILTIN, LOW); 
  delay(800);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(200);


}
