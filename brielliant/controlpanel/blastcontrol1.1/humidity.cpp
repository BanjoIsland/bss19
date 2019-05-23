#include "Arduino.h"
#include "humidity.h"

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

sensors_event_t event;
float humidityBasePt;

DHT_Unified dht(DHT_PIN, DHT_TYPE);

void humSetup() {
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
}

void humSetBasePt()
{
  // for some reason, the dht11 needs a few reads WITH a few ms
  // in between to clear out its old readings ...
  for(int i = 0; i < HUM_NUM_READS; ++i) { 
    dht.humidity().getEvent(&event);
    humidityBasePt = event.relative_humidity;
    delay(HUM_WAIT_MS);
  }
}

bool humCheck()
{
  // Get humidity event
  dht.humidity().getEvent(&event);
  float currentHumidity = event.relative_humidity; 

  delay(HUM_WAIT_MS);
  Serial.println("base is: " + String(humidityBasePt));  
  Serial.println("current is: " + String(currentHumidity));

  if (currentHumidity > humidityBasePt + HUM_COMPARE) {
    return true;
  } else {
    return false;
  }
}
