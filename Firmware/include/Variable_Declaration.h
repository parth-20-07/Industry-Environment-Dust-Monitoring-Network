#ifndef Variable_Declaration_h
#define Variable_Declaration_h

/* ----------------------- BASIC DEVICE CONFIGURATION ----------------------- */
// #define DEBUG_MODE
#define NODE_ID_SIZE 5
#define EEPROM_NODE_ID_VALUE_LOCATION 500
String NODE_ID;

/* ---------------------- ESP32 WEBSERVER CONFIGURATION --------------------- */
String WEBSERVER_SSID = "ECOLOGIC DUST MONITOR";
#define WEBSERVER_PASSWORD "123456789"
bool launch_webserver_flag = false;

/* --------------------------- WIFI CONFIGURATION --------------------------- */
#define SSID_CHAR_LENGTH 50
#define PASSWORD_CHAR_LENGTH 100
#ifdef DEBUG_MODE
char ssid[SSID_CHAR_LENGTH] = "abc";
char password[PASSWORD_CHAR_LENGTH] = "123@54321a";
#endif // DEBUG_MODE
#ifndef DEBUG_MODE
char ssid[SSID_CHAR_LENGTH];
char password[PASSWORD_CHAR_LENGTH];
#endif // DEBUG_MODE
#define EEPROM_SSID_SIZE_LOCATION 0
#define EEPROM_PASSWORD_SIZE_LOCATION 1
#define EEPROM_SSID_VALUE_LOCATION 100
#define EEPROM_PASSWORD_VALUE_LOCATION 200

/* ------------------------ SENSOR DATA CONFIGURATION ----------------------- */
uint32_t last_data_update_millis;
#ifndef DEBUG_MODE
#define DATA_REFRESH_RATE 180000 // Refresh rate of sensor data is 3 mins
#endif                           // DEBUG_MODE
#ifdef DEBUG_MODE
#define DATA_REFRESH_RATE 20000 // Refresh rate of sensor data is 20 sec
#endif                          // DEBUG_MODE

/* --------------------------- TIME CONFIGURATION --------------------------- */
uint16_t year;
uint8_t month, date, hour, minutes, seconds;

#endif