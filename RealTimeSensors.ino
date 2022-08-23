#include <OneWire.h>
#include <DallasTemperature.h>

#define VERSION 1
#define ONE_WIRE_BUS 4




OneWire OneWire(ONE_WIRE_BUS);
DallasTemperature TemperatureSensors(&OneWire);

int LED = 2;


void setup(void)
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop(void){
  TemperatureSensors.requestTemperatures();
String outputString = "{\"Version\":";
outputString+=VERSION;

outputString+="\"TemperatureSensors\":[";
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
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);


}
