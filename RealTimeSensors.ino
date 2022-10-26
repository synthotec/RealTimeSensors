#include <OneWire.h>
#include <DallasTemperature.h>

#define Version 7

//######### ZONE SETUP ##########
#define PinCount 1
int PinNumbers[] = {D2}; // Add multiple entries in this format: {D1, D2, D3}
int ZoneNumbers[] = {0};	 // Add multiple entries in this format: {0, 1, 2}
//########## ZONE SETUP ##########

OneWire OneWires[PinCount];
DallasTemperature DallasTemperatures[PinCount];

void setup(void)
{
	for (int PinIndex = 0; PinIndex <= (PinCount - 1); PinIndex++)
	{
		OneWires[PinIndex] = OneWire(PinNumbers[PinIndex]);
		DallasTemperatures[PinIndex] = DallasTemperature(&OneWires[PinIndex]);
		DallasTemperatures[PinIndex].begin();
	}

	pinMode(LED_BUILTIN, OUTPUT);
	Serial.begin(115200);
}

void loop(void)
{

	String JsonString = "{\"TemperatureZones\":[";

	for (int PinIndex = 0; PinIndex <= (PinCount - 1); PinIndex++)
	{

		if (PinIndex > 0)
		{
			JsonString += ",";
		}

		JsonString += "{\"Zone\":";
		JsonString += ZoneNumbers[PinIndex];
		JsonString += ", \"Sensors\":[";

		DallasTemperatures[PinIndex].begin();
		DallasTemperatures[PinIndex].requestTemperatures();

		for (uint8_t SensorIndex = 0; SensorIndex < DallasTemperatures[PinIndex].getDeviceCount(); SensorIndex++)
		{

			if (SensorIndex > 0)
			{
				JsonString += ",";
			}

			JsonString += "{\"Index\":";
			JsonString += SensorIndex;

			JsonString += ", \"CelciusValue\":";
			JsonString += DallasTemperatures[PinIndex].getTempCByIndex(SensorIndex);
			JsonString += "}";
		}
		JsonString += "]}";
	}

	JsonString += "]";
	JsonString += ",\"Version\":";
	JsonString += Version;
	JsonString += "}";
	Serial.println(JsonString);

	digitalWrite(LED_BUILTIN, LOW);
	delay(1000);
	digitalWrite(LED_BUILTIN, HIGH);
	delay(1000);
}