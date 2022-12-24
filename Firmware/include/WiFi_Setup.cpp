#ifndef WiFi_Setup_cpp
#define WiFi_Setup_cpp

#include "Arduino.h"
#include "WiFi.h"
#include "Variable_Declaration.h"

/* ----------------------------- BASIC DEFITION ----------------------------- */

/* --------------------------- FUNCTION DEFINITION -------------------------- */

/**
 * @brief Setup WiFi and NTP
 */
bool connect_to_wifi()
{
    Serial.println("Connecting to Wi-Fi");
    delay(50);

    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    int connection_attempts = 0;
    int max_attempts = 50;
    while (connection_attempts < max_attempts)
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            delay(100);
            Serial.print(".");
            connection_attempts++;
        }
        else
        {
            Serial.println("\nWi-Fi Connected!");
            break;
        }
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("\nWifi Not Connected to SSID: " + (String)ssid);
        return false;
    }
    else
        return true;
}

#endif