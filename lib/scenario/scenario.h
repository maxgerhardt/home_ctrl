#ifndef scenario_h
#define scenario_h


#include <list>
#include <WString.h>



#include "../class_commons/class_commons.h"

#include "../ser-deser/common_deser.h"


class scenario{
public:
	//String title;
	uint32_t id;
	bool all_off; // ob alle Anderen aus gehören

scenario(unsigned char** source_mem);
	std::list<switch_entry> module_switch_on_list; // beim parsen switchOn ignorieren. nur sensitivity verwenden. +
	std::list<sensor_change_entry> sensor_changing_list;
void print();
};


std::list<scenario> deserialise_scen_list(unsigned char* target_memory);

#endif
