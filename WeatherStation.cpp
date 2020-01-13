#include <WeatherStation.h>

WeatherStation::WeatherStation(uint8_t INDICATION_PIN, uint8_t DHT_PIN) {

    WeatherStation::INDICATION_PIN = INDICATION_PIN;
    WeatherStation::DHT_PIN = DHT_PIN;
    WeatherStation::dht = new DHT(DHT_PIN, DHT11);
    WeatherStation::dht->begin();
    WeatherStation::authentication_key = AUTHENTICATION_KEY;
    WeatherStation::fingerprint = SSL_FINGERPRINT;

    pinMode(INDICATION_PIN, OUTPUT);

    this->status_off();
    this->initWiFi();

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

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" Degree Celsius");
}

void WeatherStation::initLED(uint8_t GREEN_PIN, uint8_t YELLOW_PIN, uint8_t RED_PIN) {
  WeatherStation::GREEN_PIN = GREEN_PIN;
  WeatherStation::YELLOW_PIN = YELLOW_PIN;
  WeatherStation::RED_PIN = RED_PIN;
  
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
}

void WeatherStation::initWiFi() {

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    int i = 0;
    bool success = true;
    Serial.print("Trying to connect to ");
    Serial.println(WIFI_SSID);
    while(WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print("*");
        i++;
        i % 2 == 0 ? this->status_on() : this->status_off();
        if(i > 30) {
            success = false;
            break;
        }
    }

    if(!success) {
        this->status_off();
        Serial.println("Error connecting to ");
        Serial.println(WIFI_SSID);
    } else {
        this->status_on();
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

void WeatherStation::status_on() {
    digitalWrite(WeatherStation::INDICATION_PIN, HIGH);
}

void WeatherStation::status_off() {
    digitalWrite(WeatherStation::INDICATION_PIN, LOW);
}

void WeatherStation::loop(int d) {

    this->measure();

    String payload = "{\"humidity\":";
    payload += this->humidity;
    payload += ",\"temperature\":";
    payload += this->temperature;
    payload += ",\"authentication\":\"";
    payload += this->authentication_key;
    payload += "\"}";

    this->send_https_request(SERVER_HOST, payload);

    delay(d);
}

void WeatherStation::send_https_request(char* host, String payload) {

    if(WiFi.status() == WL_CONNECTED) {
        WiFiClientSecure client;

        client.setFingerprint(this->fingerprint);
        client.setTimeout(15000);
        delay(500);

        int r = 0;
        while((!client.connect(host, 443)) && (r < 30)) {
            delay(100);
            r++;
        }

        if(r == 30) {
            Serial.println("Connection failed");
        } else {

            String request = "POST /sensor/ HTTP/1.1\r\nHost: ";
            request += host;
            request += "\r\nContent-Type: application/json\r\nContent-Length: ";
            request += payload.length();
            request += "\r\nConnection: close\r\n\r\n";
            request += payload;

            client.print(request);

            while(client.connected()) {
                String line = client.readStringUntil('\n');
                if(line == "\r") {
                    break;
                }
            }

            String line = "";
            while(client.available()) {
                String tmp = client.readStringUntil('\n');
                if(tmp.startsWith("{")) {
                    line = tmp;
                }
            }

            const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
            DynamicJsonBuffer jsonBuffer(bufferSize);
            JsonObject& root = jsonBuffer.parseObject(line);

            const char* result = root["type"];
            Serial.print("Result: ");
            Serial.println(result);

            String token = "success";

            if(token.equals(result)) {
                this->status_on();
            } else {
                this->status_off();
            }

        }
    }

}

