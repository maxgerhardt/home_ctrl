/* old
 #ifndef eeprom_h
#define eeprom_h

// NEEDS
#include <Wire.h>
//#include <stdlib.h>
#include <Arduino.h>
#include <WString.h>
#include <ArduinoJson.h>
#include "../network_info/network_info.h"
#include "../configuration_class/config_class.h"
#include <EEPROM.h>
// DEFINES

#define eeprom_sda D2
#define eeprom_scl D3
#define disk1 0x50    //Address of 24LC256 eeprom chip, if  this is defined, we have different functions
#define MAX_BYTE_COUNT 65536
#define MAX_SSID_SIZE 32
#define MAX_PW_SIZE 32
#define internal_eeprom_size 4096
#define VALID_CHAR 0x58
#define INVALID_CHAR 0x50

#define default_conf_size 3

//Objects

class fs_eeprom {
private:

	//String _current_config;
	EEPROMClass* _internal_eeprom = &EEPROM;

	int write_ui(unsigned long target_id, int start_addr);
	unsigned long read_ui(int start_addr, int * counter);

	int write_hex_num(int type, int start_addr);
	int read_hex_num(int start_addr, int * counter);

	int serialise_switch_entry(std::list<switch_entry>::iterator* target,
			int starting_addr);

	switch_entry deserialise_switch_entry(int starting_addr, int *counter);
	int serialise_room(std::list<room>::iterator* target, int starting_addr);
	room deserialise_room(int starting_addr, int * counter);

	int serialise_module(std::list<module>::iterator* target,
			int starting_addr);
	module deserialise_module(int starting_addr, int * adress_change);

	int serialise_ui_list(std::list<unsigned long> *, int start_addr);
	std::list<unsigned long> deserialise_ui_list(int starting_addr,
			int *counter);

	int serialise_sensor(std::list<sensor>::iterator* target,
			int starting_addr);
	sensor deserialise_sensor(int starting_addr, int * adress_change);

	std::list<switch_entry> deserialise_switch_entry_list(int starting_addr,
			int * counter);
	int serialise_switch_entry_list(std::list<switch_entry>* target,
			int starting_addr);

	int serialise_sensor_list(std::list<sensor>* target, int starting_addr);
	std::list<sensor> deserialise_sensor_list(int starting_addr, int * counter);

	sensor_change_entry deserialise_sensor_change_entry(int starting_addr,
			int *counter);
	int serialise_sensor_change_entry(
			std::list<sensor_change_entry>::iterator* target,
			int starting_addr);

	int serialise_sensor_change_entry_list(
			std::list<sensor_change_entry>* target, int starting_addr);
	std::list<sensor_change_entry> deserialise_sensor_change_entry_list(
			int starting_addr, int * counter);

	int serialise_scenario(std::list<scenario>::iterator* target,
			int starting_addr);
	scenario deserialise_scenario(int starting_addr, int *counter);

	int serialise_scenario_list(std::list<scenario>* target, int starting_addr);
	std::list<scenario> deserialise_scenario_list(int starting_addr,
			int * counter);

	int serialise_automation(std::list<automation>::iterator* target,
			int starting_addr);
	automation deserialise_automation(int starting_addr, int *counter);

	int serialise_automation_list(std::list<automation>* target,
			int starting_addr);
	std::list<automation> deserialise_automation_list(int starting_addr,
			int * counter);

	String deserialise_string(int starting_addr, int * counter);
	int serialise_string(String* target, int starting_addr);
	int serialise_room_list(std::list<room>* target, int starting_addr);
	std::list<room> deserialise_room_list(int starting_addr, int * counter);
	std::list<module> deserialise_module_list(int starting_addr, int * counter);
	int serialise_module_list(std::list<module>* target, int starting_addr);

	int serialise_ui16(uint16_t target_id, int start_addr);
	uint16_t deserialise_ui16(int start_addr, int* counter);

	int serialise_ui16_list(std::list<uint16_t>* target,
			int starting_addr);
	std::list<uint16_t> deserialise_ui16_list(int starting_addr,
			int *counter);

public:
	fs_eeprom() {

	}
	void eepromsetup();
	bool write_net_config(HardwareSerial* ser,network_info * net);
	bool read_net_config(HardwareSerial* ser,network_info * net);
	bool save_json_string_into_eeprom(JsonObject *to_be_saved);

	bool read_eeprom_config(config_class* config);
	bool remove_net_config();
	bool remove_string_config();

	config_class deserialise_config_class(int starting_addr, int * counter);
	int serialise_config_class(config_class* target, int starting_addr);

	//String config="";
	//String id="";
	//bool have_valid_json = false;

};

// HAS
void eepromsetup();
#ifndef disk1
	void _writeEEPROM(int deviceaddress, unsigned int eeaddress, byte data );
	byte _readEEPROM(int deviceaddress, unsigned int eeaddress );
#endif
#ifdef disk1
	void _writeEEPROM(unsigned int eeaddress, byte data);
	byte _readEEPROM(unsigned int eeaddress);
#endif
#endif

*/

// new
#ifndef eeprom_h
#define eeprom_h

//**** NEEDS
#include <Wire.h>
#include <EEPROM.h>

//#include <stdlib.h>
#include <Arduino.h>
//#include <WString.h>
#include "../network_info/network_info.h"
//#include "../obj/config_class/config_class.h"
//*** DEFINES

#define eeprom_sda D2
#define eeprom_scl D3
#define disk1 0x50    //Address of 24LC256 eeprom chip, if  this is defined, we have different functions
#define MAX_BYTE_COUNT 65536
#define MAX_SSID_SIZE 32
#define MAX_PW_SIZE 32
#define internal_eeprom_size 4096
#define VALID_CHAR 0x58
#define INVALID_CHAR 0x50

#define default_conf_size 3

//Objects

class fs_eeprom {
private:

//String _current_config;
EEPROMClass* _internal_eeprom = &EEPROM;



public:
fs_eeprom() {

}
void eepromsetup();
bool write_net_config(HardwareSerial* ser,network_info * net);
bool read_net_config(HardwareSerial* ser,network_info * net);

bool remove_net_config();
bool remove_String_config();



};

//*** HAS
void eepromsetup();
#ifndef disk1
void _writeEEPROM(int deviceaddress, uint32_t eeaddress, byte data );
byte _readEEPROM(int deviceaddress, uint32_t eeaddress );
#endif
#ifdef disk1
void _writeEEPROM(uint32_t eeaddress, byte data);
byte _readEEPROM(uint32_t eeaddress);
#endif
#endif

