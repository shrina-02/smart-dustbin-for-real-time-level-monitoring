# Smart Dustbin with Ultrasonic Sensor for Real-Time Level Monitoring and Email Notification

An IoT-based smart dustbin that uses an ultrasonic sensor to monitor the distance between the sensor and the waste. The ESP8266 processes the sensor readings, displays the distance on a 16×2 I2C LCD, and sends an automated email alert when the waste reaches a predefined threshold.

## Features

- Real-time waste level monitoring
- Ultrasonic distance measurement using HC-SR04
- ESP8266-based control and Wi-Fi connectivity
- 16×2 I2C LCD display
- Automatic email notification
- Configurable waste-level threshold
- Serial Monitor output for debugging

## Hardware Components

- ESP8266
- HC-SR04 Ultrasonic Sensor
- 16×2 I2C LCD Display
- Jumper Wires
- Breadboard
- Smart Dustbin Prototype

## Software and Libraries

- Arduino IDE
- ESP8266WiFi
- LiquidCrystal_I2C
- ESP Mail Client

## How It Works

1. The ultrasonic sensor sends an ultrasonic pulse toward the waste.
2. The ESP8266 measures the time taken for the echo to return.
3. The distance is calculated from the measured time.
4. The current distance is displayed on the LCD.
5. If the measured distance falls below the configured threshold, the ESP8266 sends an email notification.
6. The system continues monitoring the dustbin at regular intervals.

## System Configuration

The current firmware uses a distance threshold of: 10cm 
