#ifndef WeatherStation_H
#define WeatherStation_H

#include <Arduino.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

#include <Config.h>

class WeatherStation {

  public:
    WeatherStation(uint8_t INDICATION_PIN, uint8_t DHT_PIN);
    ~WeatherStation();
    
    void initLED(uint8_t GREEN_PIN, uint8_t YELLOW_PIN, uint8_t RED_PIN);
    void green(bool on = true);
    void yellow(bool on = true);
    void red(bool on = true);
    void measure();
    void loop(int delay = 30000);

  private:
    uint8_t INDICATION_PIN;
    uint8_t GREEN_PIN;
    uint8_t YELLOW_PIN;
    uint8_t RED_PIN;
    uint8_t DHT_PIN;

    const char* ssid;
    const char* password;

    float humidity;
    float temperature;
    const char* authentication_key;
    const char* fingerprint;

    DHT* dht;

    void led_reset();
    void send_https_request(char* host, String payload);
    void status_on();
    void status_off();
    void initWiFi();
};

#endif
