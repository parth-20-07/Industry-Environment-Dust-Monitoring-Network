#ifndef Pin_Connection_h
#define Pin_Connection_h
#include "Arduino.h"

// GPIO Pins
#define ESP32_GPIO_2 2 // DEVICE_WORKING_STATE_LED_PIN
#define ESP32_GPIO_4 4 // WEBSERVER_INDICATION_LED_PIN
#define ESP32_GPIO_5 5
#define ESP32_GPIO_12 12
#define ESP32_GPIO_13 13
#define ESP32_GPIO_14 14
#define ESP32_GPIO_15 15
#define ESP32_GPIO_16 16
#define ESP32_GPIO_17 17
#define ESP32_GPIO_25 25
#define ESP32_GPIO_26 26
#define ESP32_GPIO_27 27
#define ESP32_GPIO_32 32
#define ESP32_GPIO_33 33
#define ESP32_GPIO_34 34
#define ESP32_GPIO_35 35
#define ESP32_GPIO_39 39
#define ESP32_GPIO_36 36 // WEBSERVER_INTERRUPT_PIN

// Communication Pins
#define ESP_SPI_SCK 18  // SPI Clock
#define ESP_SPI_MISO 19 // SPI Master In Slave Out
#define ESP_SPI_MOSI 23 // SPI Master Out Slave In
#define ESP_I2C_SDA 21  // I2C Serial Data
#define ESP_I2C_SCL 22  // T2C Serial Clock

/* -------------------------- WEBSERVER LAUNCH PIN -------------------------- */
#define WEBSERVER_INTERRUPT_PIN ESP32_GPIO_36

/* -------------------------------- LED PINS -------------------------------- */
#define WEBSERVER_INDICATION_LED_PIN ESP32_GPIO_4
#define DEVICE_WORKING_STATE_LED_PIN ESP32_GPIO_2

void setup_pins(void)
{
    pinMode(WEBSERVER_INTERRUPT_PIN, INPUT);
    pinMode(WEBSERVER_INDICATION_LED_PIN, OUTPUT);
    pinMode(DEVICE_WORKING_STATE_LED_PIN, OUTPUT);
    digitalWrite(WEBSERVER_INDICATION_LED_PIN, LOW);
    digitalWrite(DEVICE_WORKING_STATE_LED_PIN, LOW);
    Serial.println("Pin Setup Completed");
}
#endif