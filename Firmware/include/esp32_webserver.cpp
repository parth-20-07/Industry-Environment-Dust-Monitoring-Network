// Reference (ESP32 Hotspot): https://lastminuteengineers.com/creating-esp32-web-server-arduino-ide/
// Reference (ESP32 Server): https://randomnerdtutorials.com/esp32-esp8266-input-data-html-form/
#include "Arduino.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "HTML_PAGE.h"
#include "Variable_Declaration.h"
#include "ESP32 Pin Connection.h"

/* ---------------------------- BASIC DEFINITION ---------------------------- */
AsyncWebServer server(80);
const char *PARAM_INPUT_1 = "ssid";
const char *PARAM_INPUT_2 = "password";

/* --------------------------- FUNCTION DEFINITION -------------------------- */
/**
 * @brief Function is used when there is error while creating Web Server
 * @param request
 */
void notFound(AsyncWebServerRequest *request)
{
    Serial.println("Cannot Create Server");
    request->send(404, "text/plain", "Not found");
}

/**
 * @brief Creates a Web Server which is used to enter new configuration settings like:
 * SSID
 * Password
 */
void launch_webserver(void)
{
    Serial.println("\nDevice Configuration Details Unavailable\nConfiguring WebServer");
    delay(2000);
    WiFi.disconnect();
    digitalWrite(WEBSERVER_INDICATION_LED_PIN, HIGH);
    uint32_t data_refresh_last_millis = millis();
    bool first_data_load = true;
    IPAddress local_ip(192, 168, 1, 1);
    IPAddress gateway(192, 168, 1, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.mode(WIFI_AP);
    String str_ssid = WEBSERVER_SSID + ' ' + NODE_ID;
    char temp_ssid[str_ssid.length()];
    strcpy(temp_ssid, str_ssid.c_str());
    char temp_pwd[] = WEBSERVER_PASSWORD;
    WiFi.softAP(temp_ssid, temp_pwd);
    WiFi.softAPConfig(local_ip, gateway, subnet);
    delay(100);
    Serial.println("Connect to:");
    Serial.println("SSID: " + (String)temp_ssid);
    Serial.println("Password: " + (String)temp_pwd);
    Serial.println("IP address: 192.168.1.1");

    // Send web page with input fields to client
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/html", index_html); });

    // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
    server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
              {
        String inputMessage;
        String inputParam;
        if (request->hasParam(PARAM_INPUT_1))
        {
            inputMessage = request->getParam(PARAM_INPUT_1)->value();
            inputParam = PARAM_INPUT_1;
            Serial.println("SSID: " + inputMessage);
            memset(ssid, 0, SSID_CHAR_LENGTH);
            strcpy(ssid, inputMessage.c_str());
        }

        if (request->hasParam(PARAM_INPUT_2))
        {
            inputMessage = request->getParam(PARAM_INPUT_2)->value();
            inputParam = PARAM_INPUT_2;
            Serial.println("Password: " + inputMessage);
            memset(password, 0, PASSWORD_CHAR_LENGTH);
            strcpy(password, inputMessage.c_str());
        }

        else
        {
            inputMessage = "No message sent";
            inputParam = "none";
        }
        String html_page = R"rawliteral(<!DOCTYPE html>
<html>

<head>
    <title>Confirmation Page</title>
    <meta name="viewport" http-equiv="Content-Type"
        content="width=device-width, initial-scale=1 text/html; charset=utf-8">
    <style>
        body {
            background-color: #0000008c;
            font-family: Georgia, "Times New Roman", Times, serif;
            color: rgb(255, 255, 255);
        }
    </style>
</head>

<body>
    <center>
        <table>
            <tr><h3>CONFIGURATION DETAILS</h3></tr>
            <tr>
                <td>SSID : </td>
                <td>
)rawliteral" + (String)ssid +
                           R"rawliteral(
                </td>
            </tr>
            <tr>
                <td>Password : </td>
                <td>
)rawliteral" + (String)password +
                           R"rawliteral(
                </td>
            </tr>
            <tr>
                <td>Node ID : </td>
                <td>
)rawliteral" + NODE_ID + R"rawliteral(
                </td>
            </tr>
        </table>
    </center>
</body>

</html>)rawliteral";
        request->send(200, "text/html", html_page); });
    server.onNotFound(notFound);
    server.begin();
    while (1)
    {
        yield();
        if (digitalRead(WEBSERVER_INTERRUPT_PIN) == LOW)
        {
            Serial.println("Stopping Webserver");
            digitalWrite(WEBSERVER_INDICATION_LED_PIN, LOW);
            break;
        }
    }
}
