#ifndef WeatherStation_H
#define WeatherStation_H

#include <Arduino.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

class WeatherStation {

  public:
    WeatherStation(uint8_t DHT_PIN);
    ~WeatherStation();
    
    void initLED(uint8_t GREEN_PIN, uint8_t YELLOW_PIN, uint8_t RED_PIN);
    void initWiFi(char* ssid, char* password);
    void green(bool on = true);
    void yellow(bool on = true);
    void red(bool on = true);
    void measure();
    void loop(int delay = 30000);
    //void connect_to_wifi(const char* ssid, const char* password);

  private:
    uint8_t GREEN_PIN;
    uint8_t YELLOW_PIN;
    uint8_t RED_PIN;
    uint8_t DHT_PIN;

    const char* ssid;
    const char* password;

    float humidity;
    float temperature;

    DHT* dht;

    void led_reset();
    int send_request(float humidity, float temperature);
};

#endif
