#include "scenario.h"


std::list<scenario> deserialise_scen_list(unsigned char* target_memory){
        std::list<scenario> ret;
        int scen_size = deser_uint8(&target_memory);
        for(int i=0; i<scen_size; i++) {
                ret.push_back(scenario(&target_memory));
        }

        return ret;
}


scenario::scenario(unsigned char** source_mem){
        id = deser_uint32(source_mem);
        all_off = deser_uint8(source_mem);
        module_switch_on_list = deser_swe_list(source_mem);
        sensor_changing_list = deser_s_change_list(source_mem);
}

void scenario::print(){
        Serial.print(" \n scene: \n");
        Serial.print("\n " + String(id) + " "+ String(all_off) + " entries: \n");
        Serial.print("length: " + String(module_switch_on_list.size()) + " \n");
        for (std::list<switch_entry>::iterator it = module_switch_on_list.begin();
             it != module_switch_on_list.end(); it++) {
                (*it).print();
                Serial.print(" ");
        }

        Serial.print("length: " + String(sensor_changing_list.size()) + " \n");
        for (std::list<sensor_change_entry>::iterator it = sensor_changing_list.begin();
             it != sensor_changing_list.end(); it++) {
                (*it).print();
                Serial.print(" ");
        }
}
