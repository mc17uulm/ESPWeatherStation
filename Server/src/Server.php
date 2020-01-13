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
            if($authentication !== "authentication") {
                Response::send_error("Invalid payload");
            }

            $temperature = $json["temperature"];
            $humidity = $json["humidity"];
            $date = date('D. j M G:i:s', time());

            $data = "$date | Temperature: $temperature °C | Humidity: $humidity %\n";

            file_put_contents(__DIR__ . "/../log.txt", $data, FILE_APPEND);

            Response::send_success(array(
                "humidity" => $humidity,
                "temperature" => $temperature
            ));

        } catch (\JsonException $e) {
            Response::send_error("Invalid payload");
        }

    }

}
