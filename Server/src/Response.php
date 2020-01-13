<?php

namespace ESP8266WeatherStation;

class Response
{

    private static function send(array $payload) : void
    {
        header("Content-Type: application/json");
        $json = json_encode($payload);
        echo $json;
        die();
    }

    public static function send_error(string $msg) : void
    {
        $payload = array(
            "type" => "error",
            "msg" => $msg
        );
        self::send($payload);
    }

    public static function send_success($payload = "") : void
    {
        self::send(array(
            "type" => "success",
            "msg" => $payload
        ));
    }

}