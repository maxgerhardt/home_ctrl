#ifndef sensor_h
#define sensor_h


#include <list>
#include <WString.h>



#include "../class_commons/class_commons.h"

#include "../ser-deser/common_deser.h"

class sensor {
public:
	//String title;
	uint32_t id = 0;
	uint32_t h_id = 0;
	uint32_t channel = 0;
	sensor_model model = u_def_sens; // lightSensor magnetSensor moveSensor

	uint32_t autoSwitchOffTime; // sekunden

	// temp / hum
	std::list<uint32_t> limits;
	std::list<switch_entry> th_upper_switchOn;
	std::list<switch_entry> th_upper_switchOff;

	std::list<switch_entry> switch_on_list;
	std::list<switch_entry> switch_off_list;
	std::list<switch_entry> autoSwitchOff_list;

sensor(unsigned char ** source_memory);
void print();
};

std::list<sensor> deserialise_sens_list(unsigned char* source_memory);

#endif
