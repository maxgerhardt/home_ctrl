#ifndef config_class_h
#define config_class_h


#include <list>
#include <WString.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>
#include <time.h>

//shared
#include "../ser-deser/common_deser.h"
#include "../class_commons/class_commons.h"

//objects
#include "../module/module.h"
#include "../automation/automation.h"
#include "../sensor/sensor.h"
#include "../scenario/scenario.h"

//functionality
#include "../../transmitter/transmitter.h"
#include "../../temp_hum_sens/temp_hum_sens.h"
#include "../../infra/infra.h"
#include "../../dusk_dawn_calc/dusk_dawn_calc.h"


//values
#define max_size_sw_all_off 10
#define DEF_BUF_SIZE 20000
#define jsonsubbuffer 2000


//class
class config_class {
public:
//generall data
String title = "";
String id ="0";
// time specific data
uint32_t tz =2;
bool daylightsave = false;
float longtitute = 48.2082;
float latitute = 16.3738;

// config specific data
std::list<sensor> sensors;
std::list<scenario> scenarios;
std::list<module> modules;
std::list<automation> automations;

//running processes data
std::list<delayed_trigger> delay_triggering;
std::list<uint32_t> automation_trigger_list;
std::list<sender_queue_entry> working_queue;

// temporary data
uint32_t sunset_time = 0;
uint32_t sunrise_time = 0;

uint32_t current_temp = 0;
uint32_t current_hum = 0;
uint32_t current_time = 0;   // 604800 for one full week; 1 => Montag 00:00:01  ; 604799 => Sonntag 23:59:59

uint32_t active_scene_id = 0;

temp_state temp_cur_state= neutral;
temp_state hum_cur_state= neutral;




// timing control variables
uint32_t last_time_update_dusk_dawn = 0;
uint32_t last_time_temp = 0;
uint32_t last_time_hum = 0;
uint32_t last_time_push = 0;
uint32_t last_time_called = 0; // TODO rename to last_time_outside_called

uint32_t lt_limit_check = 0; // TODO change
//controlbits
// lists
bool sensors_valid = false;
bool scenarios_valid = false;
bool automations_valid = false;
bool modules_valid = false;
bool rooms_valid = false;
// time
bool dusk_dawn_vaid = false;
bool time_init_done = false;

// temp / hum
bool temp_valid = false;
bool hum_valid = false;
// main
bool id_valid = false;


//constructors
config_class(){
};

//destructors
~config_class(){
};


//functions
private:
bool trigger_switch_entry_list(Transmitter* sender,std::list<switch_entry> target);
bool trigger_automation(Transmitter* sender, uint32_t id);
bool update_temp(temp_hum_s * main_sens);
bool update_hum(temp_hum_s * main_sens);
bool update_time(Transmitter* sender);
void sendNTPpacket(IPAddress& address);
bool get_dusk_dawn();
bool update_extern_val();
bool check_limits(Transmitter* sender);



public:
void print();
bool pair_module(uint32_t t_id,uint32_t t_h_id);

bool trigger_module(uint32_t t_id, uint32_t target_sens,Transmitter* sender, IRsend *irsend);

bool trigger_scenario(Transmitter* sender, uint32_t sc_id);
bool pair_sensor(uint32_t t_id, uint32_t t_h_id, uint32_t t_h_ch);
bool pair_ir_mod(uint32_t id, decode_results* tmp);
void set_scenario(uint32_t new_sc) {
								temp_cur_state= neutral;
								active_scene_id = new_sc;
}
//void add_to_queue_immediate_switch(uint32_t address,	unsigned short channel, uint32_t switchOn, int type,int dect,uint64_t ir_dat);

bool trigger_sensor(Transmitter* sender, data_434_pack* data);
bool check_for_delayed_triggers(Transmitter* sender);
bool check_for_automation(Transmitter* sender,temp_hum_s * m);
bool process_working_queue(Transmitter* sender,IRsend* irsender);


void add_to_queue_delay(module i, uint32_t sensitivity_target, uint32_t delay);



bool set_up_time();

bool handle_outside_call(Transmitter* sender);
};





bool testingstuff();
class dummy{
public:
bool garbage = true;
  void print();
  dummy(){};
};


#endif
