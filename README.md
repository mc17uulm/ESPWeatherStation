# ESP8266 DHT11 WeatherStation

### Usage

#### Arduino IDE
```c++
#include <WeatherStation.h>

#define DHT_PIN D1
#define GREEN_LED D2
#define RED_LED D3
#define YELLOW_LED D4 
#define INDICATION_PIN D5

WeatherStation station(INDICATION_PIN, DHT_PIN);

void setup() {

  Serial.begin(9600);

  station.initLED(GREEN_LED, YELLOW_LED, RED_LED);

}

void loop() {

  station.loop(60000);

}
```

#### Docker image

Runs server as docker container on port 8000.

```shell script
docker-compose up -d
```

### Libraries

Required libraries are:

* Arduino
* DHT
* ESP8266WiFi
* ESP8266HTTPClient
* WiFiClientSecure (for SSL connection to server)
* ArduinoJson