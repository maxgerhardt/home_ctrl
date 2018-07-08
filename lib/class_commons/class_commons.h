#ifndef class_commons_h
#define class_commons_h

/*#include <list>
#include <vector>
#include <WString.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <WiFiUdp.h>
#include <time.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include "../transmitter/transmitter.h"
#include "../dusk_dawn_calc/dusk_dawn_calc.h"
#include "../temp_hum_sens/temp_hum_sens.h"
#include "../infra/infra.h"*/

#include "../ser-deser/common_deser.h"
// enums
//

enum autom_trigger_type {
        sunrise = 0, sunset = 1, time_trigger = 2, u_def_a_trig = 3 // TODO humidity above / humidity below
};


enum sensor_model {
	lightSensor = 0,
	magnetSensor = 1,
	moveSensor = 2,
	temp_sens = 3,
	hum_sens = 4,
	u_def_sens = 5
};
enum module_model {
        jalousieSwitch = 0, dimmer = 1, onOffSwitch = 2, infrarot = 3, gong = 4,u_def_mod = 5
};
enum temp_state {
	under = -1, neutral = 0, over = 1
};

//classes
class switch_entry{
public:
  uint32_t sens;
  uint32_t mod_id;
  uint32_t del;

  switch_entry(uint32_t sensi, uint32_t id, uint32_t dela = 0) {
		sens = sensi;
		mod_id = id;
		del = dela;
	}
	switch_entry(){};
switch_entry(unsigned char** source_mem);

void print();
};


class sensor_change_entry { // used for scenario module changes / sensor changes
public:
	uint32_t id;
	uint32_t autoSwitchOffTime;
	std::list<switch_entry> new_switchOn_list;
	std::list<switch_entry> new_switchOff_list;
	std::list<switch_entry> new_autoSwitchOff_list;

	std::list<uint32_t> new_limits;
	std::list<switch_entry> th_upper_switchOn;
	std::list<switch_entry> th_upper_switchOff;

	sensor_change_entry(unsigned char** source_memory);



void print();
};

std::list<switch_entry> deser_swe_list(unsigned char** source_mem);
std::list<switch_entry> deser_swe_list_raw(unsigned char** source_mem);

std::list<sensor_change_entry> deser_s_change_list(unsigned char** source_mem);



class delayed_trigger {
public:
	std::list<switch_entry> switch_list;
	uint32_t trigger_time;
	delayed_trigger(std::list<switch_entry> list, uint32_t time);
};
#endif
