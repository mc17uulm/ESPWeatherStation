#ifndef WeatherStation_H
#define WeatherStation_H

#include <Arduino.h>
#include <DHT.h>

class WeatherStation {

  public:
    WeatherStation(uint8_t DHT_PIN);
    ~WeatherStation();
    
    void initLED(uint8_t GREEN_PIN, uint8_t YELLOW_PIN, uint8_t RED_PIN);
    void green(bool on = true);
    void yellow(bool on = true);
    void red(bool on = true);
    void measure();

  private:
    uint8_t GREEN_PIN;
    uint8_t YELLOW_PIN;
    uint8_t RED_PIN;
    uint8_t DHT_PIN;

    DHT* dht;

    void led_reset();
};

#endif
