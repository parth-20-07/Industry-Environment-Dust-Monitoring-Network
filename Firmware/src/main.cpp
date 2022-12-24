/* ----------------------------- PRIMARY INCLUDE ---------------------------- */
#include <Arduino.h>
#include "EEPROM.h"

/* ----------------------------- CUSTOM INCLUDE ----------------------------- */
#include "ESP32 Pin Connection.h"
#include "Variable_Declaration.h"
#include "esp32_webserver.cpp"
#include "Sensor_Data_Collection.cpp"
#include "WiFi_Setup.cpp"
#include "MQTT_Connection.cpp"

/* --------------------------- FUNCTION DEFINITION -------------------------- */
void IRAM_ATTR webserver_start_request(void);
void fetch_ssid_password(void);
void fetch_new_ssid_password_from_webserver(void);
void fetch_node_id(void);

/* -------------------------- FUNCTION DECLARATION -------------------------- */
void fetch_ssid_password(void)
{
  uint8_t ssid_size = EEPROM.read(EEPROM_SSID_SIZE_LOCATION);
  uint8_t password_size = EEPROM.read(EEPROM_PASSWORD_SIZE_LOCATION);
  if ((ssid_size > SSID_CHAR_LENGTH) || (password_size > PASSWORD_CHAR_LENGTH) || ((ssid_size * password_size) == 0))
    fetch_new_ssid_password_from_webserver();
  else
  {
    for (size_t i = 0; i < ssid_size; i++)
      ssid[i] = (char)EEPROM.read(EEPROM_SSID_VALUE_LOCATION + i);
    for (size_t i = 0; i < password_size; i++)
      password[i] = (char)EEPROM.read(EEPROM_PASSWORD_VALUE_LOCATION + i);
    Serial.println("SSID: " + (String)ssid);
    Serial.println("PASSWORD: " + (String)password);
  }
}

void fetch_new_ssid_password_from_webserver(void)
{
  launch_webserver();
  uint8_t ssid_size = ((String)ssid).length();
  uint8_t password_size = ((String)password).length();
  EEPROM.write(EEPROM_SSID_SIZE_LOCATION, ssid_size);
  EEPROM.write(EEPROM_PASSWORD_SIZE_LOCATION, password_size);
  for (size_t i = 0; i < ssid_size; i++)
    EEPROM.write(EEPROM_SSID_VALUE_LOCATION + i, ssid[i]);
  for (size_t i = 0; i < password_size; i++)
    EEPROM.write(EEPROM_PASSWORD_VALUE_LOCATION + i, password[i]);
  EEPROM.commit();
  Serial.println("Save Complete\nRestarting Device");
  delay(10);
  ESP.restart();
}

void fetch_node_id(void)
{
  String node_id[NODE_ID_SIZE];
  uint8_t first_id = EEPROM.read(EEPROM_NODE_ID_VALUE_LOCATION);
  Serial.println("Node ID[0]: " + (String)first_id);
  if ((first_id > 9) || first_id == 0)
  {
    Serial.println("Generating New Node ID");
    for (size_t i = 0; i < NODE_ID_SIZE; i++)
    {
      uint8_t id = random(1, 10);
      Serial.println("Write Node ID [" + (String)i + "]: " + (String)id);
      EEPROM.write(EEPROM_NODE_ID_VALUE_LOCATION + i, id);
    }
    EEPROM.commit();
  }

  for (size_t i = 0; i < NODE_ID_SIZE; i++)
  {
    node_id[i] = (String)EEPROM.read(EEPROM_NODE_ID_VALUE_LOCATION + i);
    NODE_ID += node_id[i];
  }
  Serial.println("Node ID: " + NODE_ID);
}

void setup()
{
  Serial.begin(115200);
  EEPROM.begin(512);
  setup_pins();
  // fetch_node_id();
#ifndef DEBUG_MODE
  fetch_ssid_password();
#endif // DEBUG_MODE
  setup_sensors();
  if (connect_to_wifi())
  {
    digitalWrite(DEVICE_WORKING_STATE_LED_PIN, HIGH);
    aws_setup();
    collect_dust_quality_data();
    if (pm_ae_2_5 != 0)
    {
      if (!sendData(NODE_ID, pm_ae_1_0, pm_ae_2_5, pm_ae_10_0))
      {
        Serial.println("AWS Send Failure");
        ESP.restart();
      }
      WiFi.disconnect();
      digitalWrite(DEVICE_WORKING_STATE_LED_PIN, LOW);
    }
    else
      ESP.restart();
  }
  last_data_update_millis = millis();
}

void loop()
{
#ifndef DEBUG_MODE
  if (digitalRead(WEBSERVER_INTERRUPT_PIN) == LOW)
    fetch_new_ssid_password_from_webserver();
#endif // DEBUG_MODE

  if ((millis() - last_data_update_millis) > DATA_REFRESH_RATE)
    ESP.restart();
}