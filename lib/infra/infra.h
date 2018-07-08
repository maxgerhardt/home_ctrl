// INFRAROT
#ifndef infra_h
#define infra_h

#include <ESP8266WiFi.h>
#include <list>



#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#if DECODE_AC
#include <ir_Daikin.h>
#include <ir_Fujitsu.h>
#include <ir_Gree.h>
#include <ir_Haier.h>
#include <ir_Kelvinator.h>
#include <ir_Midea.h>
#include <ir_Toshiba.h>

#endif  // DECODE_AC

#define IR_LED D0
#define IRRECV_PIN D5
#define BAUD_RATE 115200
#define CAPTURE_BUFFER_SIZE 1024
#if DECODE_AC
#define TIMEOUT 50U
#else  // DECODE_AC
#define TIMEOUT 15U  // Suits most messages, while not swallowing many repeats.
#endif  // DECODE_AC
#define MIN_UNKNOWN_SIZE 12

#define infra_repeats 2
#define ir_timeout 10000


decode_results pair_infra(IRrecv*irrecv);
bool send_infra(IRsend* sender, std::list<uint16_t> l);

#endif
