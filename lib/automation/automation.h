#ifndef automation_h
#define automation_h

#include <list>
#include <Arduino.h>


#include "../class_commons/class_commons.h"




class automation {
public:
uint32_t id=0;
uint32_t scen_id=0;
autom_trigger_type trigger =(autom_trigger_type) 3;
std::list<uint32_t> trigger_vals;


automation(unsigned char** source_memory);
void print();
};

std::list<automation> deserialise_auto_list(unsigned char* target_memory);



#endif
