#ifndef web_utils_h
#define web_utils_h

#include <list>
#include <WString.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>
#include <time.h>

#include "../utility/utility.h"

void get_SQL_res(String set, String t_id);

#endif
