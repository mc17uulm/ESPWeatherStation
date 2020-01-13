<?php

require_once __DIR__ . "/vendor/autoload.php";

use ESP8266WeatherStation\Server;
use Dotenv\Dotenv;

$dotenv = Dotenv::createImmutable(__DIR__);
$dotenv->load();

Server::run();