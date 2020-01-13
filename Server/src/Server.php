<?php

namespace ESP8266WeatherStation;

class Server
{

    public static function run() : void {

        date_default_timezone_set('Europe/Berlin');

        $content_type = $_SERVER["CONTENT_TYPE"] ?? "text/plain";
        if($content_type !== "application/json") {
            Response::send_error("Invalid content type");
        }

        if($_SERVER["REQUEST_METHOD"] !== "POST") {
            Response::send_error("Invalid request method");
        }

        $payload = file_get_contents("php://input");
        $keys = array(
            "temperature",
            "humidity",
            "authentication"
        );

        try {

            $json = json_decode($payload, true, 512, JSON_THROW_ON_ERROR);

            if(count(array_diff($keys, array_keys($json))) !== 0) {
                Response::send_error("Invalid payload");
            }

            $authentication = $json["authentication"];
            $key = getenv("AUTHORIZATION_KEY");
            if($authentication !== $key) {
                Response::send_error("Invalid payload");
            }

            $db = new \PDO(
                'mysql:host=localhost;dbname=' . getenv("DATABASE_NAME"),
                getenv("DATABASE_USER"),
                getenv("DATABASE_PASSWORD")
            );

            $temperature = $json["temperature"];
            $humidity = $json["humidity"];

            $sql = "INSERT INTO log (date, temperatue, humidity) VALUES (NOW(), ?, ?)";
            $stmt = $db->prepare($sql);
            $stmt->execute(array(
                $temperature,
                $humidity
            ));

            Response::send_success();

        } catch (\JsonException $e) {
            Response::send_error("Invalid payload");
        } catch (\PDOException $e) {
            Response::send_error("Invalid Request");
        }

    }

}
