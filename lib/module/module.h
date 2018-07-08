#ifndef module_h
#define module_h


#include <list>
#include <WString.h>



#include "../ser-deser/common_deser.h"
#include "../class_commons/class_commons.h"




class module {
public:
  //data
//String title;
uint32_t id = 0;
uint32_t h_id = 0;
module_model model = (module_model) 5;  // = onOffSwitch;//enum jalousieSwitch dimmer onOffSwitch infrared, das model ist ein enum.
std::list<uint16_t> ir_h_id;  // ist nur relevant wenn das model ein IR model ist.

//constructors
module(){};
module(int t_id,int t_h_id,int t_m);
module(unsigned char** source_memory_ptr);

//functions
void print();
unsigned char* serialise(unsigned char* target_memory);
int serialise_r_h_id(unsigned char* target_memory);
std::list<uint16_t> deserlise_r_h_id(unsigned char** source_memory);



};
unsigned char* serialise_mod_list(unsigned char* target_memory,std::list<module>* target_list);
std::list<module> deserialise_mod_list(unsigned char* target_memory);



class sender_queue_entry {
public:
  //data
	uint32_t time_to_trigger = 0;
	module mod_to_trigger=module();
	uint32_t sensitivity = 0;
	bool send_now = false;
  bool is_all_off = false;

// constructors
sender_queue_entry(){};
	sender_queue_entry(uint32_t t, module i, uint32_t sens,bool ao);

// operators
	bool operator ==(sender_queue_entry comp);
	bool operator <(sender_queue_entry comp);

// functions

  /*void print() {
		String s;
		if (send_now)
			s = "Send_now";
		else
			s = "no send now";
		Serial.println(
				s + " sender queue entry : " + String(time_to_trigger) + " hid "
						+ String(mod_to_trigger.h_id) + " sens "
						+ String(sensitivity) + " triggering at time: "
						+ String(time_to_trigger));
	}*/
};

#endif
