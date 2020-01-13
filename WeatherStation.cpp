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

  WeatherStation::humidity = humidity;
  WeatherStation::temperature = temp;

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

void WeatherStation::initWiFi(char* ssid, char* password) {

    WiFi.begin(ssid, password);

    int i = 0;
    bool success = true;
    Serial.print("Trying to connect to ");
    Serial.println(ssid);
    while(WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print("*");
        i++;
        if(i > 30) {
            success = false;
            break;
        }
    }

    if(!success) {
        Serial.println("Error connecting to ");
        Serial.println(ssid);
    } else {
        Serial.println("WiFi connection successful");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
    }
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

int WeatherStation::send_request(float humidity, float temperature) {
    if(WiFi.status() == WL_CONNECTED) {

        StaticJsonBuffer<300> JSONBuffer;
        JsonObject& JSONencoder = JSONBuffer.createObject();

        JSONencoder["humidity"] = humidity;
        JSONencoder["temperature"] = temperature;
        JSONencoder["authentication"] = "authentication";

        char JSONMessageBuffer[300];
        JSONencoder.prettyPrintTo(JSONMessageBuffer, sizeof(JSONMessageBuffer));

        HTTPClient http;

        http.begin("http://192.168.2.129:8000");
        http.addHeader("Content-Type", "application/json");

        int code = http.POST(JSONMessageBuffer);

        if(code == 200) {
            const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
            DynamicJsonBuffer jsonBuffer(bufferSize);
            JsonObject& root = jsonBuffer.parseObject(http.getString());

            const char* result = root["type"];
            Serial.print("Result: ");
            Serial.println(result);

        } else {
            Serial.print("HTTP Error Code: ");
            Serial.println(code);
        }

        http.end();
    }
}

void WeatherStation::loop(int d) {
    this->measure();

    this->send_request(this->humidity, this->temperature);

    delay(d);
}

