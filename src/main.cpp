

#include <Arduino.h>
#include <eeprom2.h>

#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <Ticker.h>
#include <WiFiUdp.h>
/*
#include <debug.h>
#include <receiver.h>
#include <transmitter.h>
#include <eeprom.h>
#include <network_info.h>
#include <config_class/config_class.h>
#include <temp_hum_sens.h>
#include <routines.h>
#include <utility.h>
#include <time_service.h>
#include <web_utils.h>
*/

/*

#include "lib/debug/debug.h"
#include "lib/receiver/receiver.h"
#include "lib/transmitter/transmitter.h"
#include "lib/eeprom/eeprom.h"
#include "lib/network_info/network_info.h"
#include "lib/obj/config_class/config_class.h"
#include "lib/temp_hum_sens/temp_hum_sens.h"
#include "lib/routines/routines.h"
#include "lib/utility/utility.h"
#include "lib/time_service/time_service.h"
#include "lib/web_utils/web_utils.h"

*/
#include "../lib/debug/debug.h"
#include "../lib/receiver/receiver.h"
#include "../lib/transmitter/transmitter.h"
#include "../lib/network_info/network_info.h"
#include "../lib/config_class/config_class.h"
#include "../lib/temp_hum_sens/temp_hum_sens.h"
#include "../lib/routines/routines.h"
#include "../lib/utility/utility.h"
#include "../lib/time_service/time_service.h"
#include "../lib/web_utils/web_utils.h"

#include "../lib/config_class/testing.h"

/*
void setup(){}
void loop(){

	ydummy y;
	y.print();

}
*/
/*
 DAta control is a cool thing, so i tried it here :)
 Author: Dom V.
 -> ZweiEuro
*/


#define SERIAL_BAUD 115200


const char* s = "";
const char* p = "";

int sys_status = 0;
int blink_segment = 0;


config_class sys_config;
network_info net(s, p);

fs_eeprom eeprom;
receiver receiver_handler;
Transmitter sender(&receiver_handler);

//Infrarot 38 kHz
IRrecv irrecv(IRRECV_PIN, CAPTURE_BUFFER_SIZE, TIMEOUT, true);
IRsend irsend(IR_LED);
decode_results results;

//decode_results results;
temp_hum_s main_sens(sens_mod);

Ticker main_ticker, led_tick;




#define time_bt_blinks 0.150

void test(config_class* c);

void setup() {

	Wire.begin(eeprom_sda, eeprom_scl);
	WiFi.mode(WIFI_STA);
	//WiFi.forceSleepBegin();
	WiFi.disconnect(true);
	Serial.begin(SERIAL_BAUD, SERIAL_8N1, SERIAL_TX_ONLY);
	while (!Serial)
		// Wait for the serial connection to be establised.
		;

	//
	Serial.println("booted");
	//eeprom.remove_string_config();

	receiver_handler = receiver();
	sender = Transmitter(&receiver_handler);
	int bytes_read = 0;
	//try {
	//sys_config = eeprom.deserialise_config_class(bytes_read, &bytes_read);
	//} // TODO}
	//catch (const std::exception& e) { // reference to the base of a polymorphic object
	//	Serial.println(e.what()); // information from length_error printed
	//}
	Serial.println("read bytes = ");
	Serial.println(bytes_read);

	pinMode(red_led, OUTPUT);
	pinMode(blue_led, OUTPUT);
	main_ticker.attach(1, time_trigger_updater);
	led_tick.attach(time_bt_blinks, led_ticker_fkt);
//	eeprom.remove_net_config();
//	eeprom.remove_string_config();

	//Infrared
#if DECODE_HASH
	// Ignore messages with less than minimum on or off pulses.
	irrecv.setUnknownThreshold(MIN_UNKNOWN_SIZE);
#endif  // DECODE_HASH
	irrecv.enableIRIn();

	irsend.begin();

}
void loop() {
	sys_status = 1;
	if (WiFi.status() != WL_CONNECTED) {
		Serial.println("connecting");
		sys_status = 3;

		if (not routine_con_WiFi(&eeprom, &net)) {
			Serial.println("failed");

			full_stop();

		}
	}
testingstuff();
dummy x  =dummy();
x.print();
	Serial.println("success");
	//full_stop();
	for (;;) {
		if (not tcp_recv_loop(&eeprom, &receiver_handler, &sys_config, &sender,
				&irrecv, &irsend, &main_sens)) {
			Serial.println("user loop broke");
			break;
		} else if (sys_config.id_valid) {
			test(&sys_config);

		} else {
		}
	}

}



void test(config_class* c) {
	String tmp_id = c->id;
	uint32_t last_time_called =0 ;
	while (true) {

		while ((millis() - last_time_called) < 5000) {
			yield();
			delay(1);
		}
		get_SQL_res("getModules", tmp_id);
		get_SQL_res("getScenarios", tmp_id);
		get_SQL_res("getAutomations", tmp_id);
		get_SQL_res("getSensors", tmp_id);

	}
}
