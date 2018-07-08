#ifndef debug_h
#define debug_h

#include "Arduino.h"

#define ERROR_CODE_LENGTH 255
#define SERIAL_BAUD 115200         //baud rate for serial monitor

static char* error_reason;



void serialsetup(HardwareSerial* Serial );
void setup_debug(char** err=&error_reason);
void assert_error(HardwareSerial* Serial, char* error);
void full_stop();
//void print_WL_info(ESP8266WiFiClass* w);
#endif
