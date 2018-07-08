#ifndef routines_h
#define routines_h

#include <eeprom2.h>
#include "../debug/debug.h"
#include "../receiver/receiver.h"
#include "../transmitter/transmitter.h"
#include "../network_info/network_info.h"
#include "../config_class/config_class.h"
#include "../temp_hum_sens/temp_hum_sens.h"

#ifndef overrule_ports
#define localTcpPort  60581
#define localUdpPort 60580
#endif

bool routine_con_WiFi(fs_eeprom * eeprom,network_info* net);
bool wifi_check(ESP8266WiFiClass* Wi);
bool tcp_recv_loop(fs_eeprom* eeprom, receiver* recv_hand, config_class *conf,
		Transmitter* sender,IRrecv*irrecv,IRsend *irsend,temp_hum_s* main_sens);
bool handleReceivedMessage(fs_eeprom* eeprom,String message, WiFiClient *client,
		config_class *conf,Transmitter* sender,receiver* recv_hand, IRrecv*irrecv, IRsend *irsend);
//void led_ticker_fkt();
#endif
