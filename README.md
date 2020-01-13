# DHT11 WeatherStation

### Usage

```c++
#include <WeatherStation.h>

#define DHT_PIN D1
#define GREEN_LED D2
#define RED_LED D3
#define YELLOW_LED D4 

char* ssid = "ssid";
char* password = "password";

WeatherStation station(DHT_PIN);

void setup() {

  Serial.begin(9600);

  station.initWiFi(ssid, password);
  station.initLED(GREEN_LED, YELLOW_LED, RED_LED);

}

void loop() {

  station.loop(10000);

}
```