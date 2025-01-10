#include <OneWire.h>
#include <DallasTemperature.h>

#define Version 19

//######### ZONE SETUP ##########
#define PinCount 5
int PinNumbers[] = { D2, D3, D4, D5, D6 }; // Add multiple entries in this format: {D1, D2, D3}
int ZoneNumbers[] = { 0, 1, 2, 3, 4 }; // Add multiple entries in this format: {0, 1, 2}
String ZoneNames[] = { "No Zones","Water (Return)","Water (Flow)","Fixed Half","Moving Half" };
// Z0 - No Zones %DEPRECATED%
// Z1 = Water (Return)
// Z2 = Water (Flow)
// Z3 = Fixed Half
// Z4 = Moving Half
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

bool ErrorOccurred;

void loop(void)
{

	ErrorOccurred = false;

	String JsonString = "{\"Version\":";
	JsonString += Version;
	JsonString += ",\"TemperatureSensors\":[";

	int TotalSensorIndex = 0;

	for (int PinIndex = 0; PinIndex <= (PinCount - 1); PinIndex++)
	{
		DallasTemperatures[PinIndex].begin();
		DallasTemperatures[PinIndex].requestTemperatures();

		for (int SensorIndex = 0; SensorIndex < DallasTemperatures[PinIndex].getDeviceCount(); SensorIndex++)
		{

			if (TotalSensorIndex > 0)
			{
				JsonString += ",";
			}

			JsonString += "{\"Index\":";
			JsonString += TotalSensorIndex;

			JsonString += ", \"Zone\":";
			JsonString += ZoneNumbers[PinIndex];

			JsonString += ", \"ZoneName\":";
			JsonString += "\"";
			JsonString += ZoneNames[PinIndex];
			JsonString += "\"";

			float TempC = DallasTemperatures[PinIndex].getTempCByIndex(SensorIndex);

			if ((TempC <= -127) || (TempC <= 85))
			{
				ErrorOccurred = true;
			}

			JsonString += ", \"CelsiusValue\":";
			JsonString += TempC;

			JsonString += "}";

			TotalSensorIndex++;
		}
	}
	JsonString += "]}";

	if (!ErrorOccurred)
	{
		Serial.println(JsonString);
	}

	digitalWrite(LED_BUILTIN, LOW);
	delay(5000);
	digitalWrite(LED_BUILTIN, HIGH);
	delay(1000);
}
