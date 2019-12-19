#include <WeatherStation.h>

WeatherStation::WeatherStation(uint8_t DHT_PIN) {
  WeatherStation::DHT_PIN = DHT_PIN;
  WeatherStation::dht = new DHT(DHT_PIN, DHT11);
  WeatherStation::dht->begin();
}

WeatherStation::~WeatherStation() {}

void WeatherStation::measure() {
  float humidity = this->dht->readHumidity();
  float temp = this->dht->readTemperature();

  this->led_reset();
  
  if(humidity >= 70) {
    this->red();
  } else if (humidity >= 60 && humidity < 70) {
    this->yellow();
  } else {
    this->green();
  }

  Serial.print("Luftfeuchtigkeit: ");
  Serial.print(humidity);
  Serial.println(" %");
  Serial.print("Temperatur: ");
  Serial.print(temp);
  Serial.println(" Grad Celsius");
}

void WeatherStation::initLED(uint8_t GREEN_PIN, uint8_t YELLOW_PIN, uint8_t RED_PIN) {
  WeatherStation::GREEN_PIN = GREEN_PIN;
  WeatherStation::YELLOW_PIN = YELLOW_PIN;
  WeatherStation::RED_PIN = RED_PIN;
  
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
}

void WeatherStation::led_reset() {
  digitalWrite(WeatherStation::GREEN_PIN, LOW);
  digitalWrite(WeatherStation::YELLOW_PIN, LOW);
  digitalWrite(WeatherStation::RED_PIN, LOW);
}

void WeatherStation::green(bool on) {
  digitalWrite(WeatherStation::GREEN_PIN, on ? HIGH : LOW);
}

void WeatherStation::yellow(bool on) {
  digitalWrite(WeatherStation::YELLOW_PIN, on ? HIGH : LOW);
}

void WeatherStation::red(bool on) {
  digitalWrite(WeatherStation::RED_PIN, on ? HIGH : LOW);
}

