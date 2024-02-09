# ESP32-Motion-Sensor-with-Telegram-Notification

<img src="images/Motion_sensor_smart.png" alt="Motion sensor smart" style="width:350px;height:300px;">

## Description

This project aims to create a versatile home security system utilizing the ESP32 microcontroller, coupled with a buzzer, motion sensor, and DHT11 temperature sensor. The system is designed to be remotely controlled and monitored via Telegram bot integration.

## Key Features

Remote Control: Users can remotely activate, deactivate, and set the alarm status through commands sent via a Telegram bot interface.<br>
Motion Detection: The motion sensor detects any movement within its range. When triggered, it activates the alarm and sends a notification to the user via Telegram.<br>
Temperature Monitoring: The DHT11 sensor measures the temperature of the environment, allowing users to monitor temperature fluctuations remotely through the Telegram bot.

## Components

ESP32 Microcontroller: Acts as the main control unit, facilitating communication between sensors and the Telegram bot.<br>
Buzzer: Emits alarm sounds when triggered by motion detection.<br>
HC-SR501 Motion Sensor PIR: Detects movement and triggers the alarm system.<br>
DHT11 Temperature Sensor: Monitors ambient temperature for remote temperature tracking.

## Code

[Click here for code!](Code.ino)

## Useful links

[How to use the second esp32 core](https://www.circuitstate.com/tutorials/how-to-write-parallel-multitasking-applications-for-esp32-using-freertos-arduino/)<br>
[How to use bot telegram with esp32](https://www.youtube.com/watch?v=TOxzQSdivVI)

