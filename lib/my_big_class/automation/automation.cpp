#include "automation.h"


automation::automation(unsigned char** source_memory){
  Serial.print(" \n creating new automation: \n " );
        id = deser_uint32(source_memory);
        scen_id = deser_uint32(source_memory);
        trigger = (autom_trigger_type) deser_uint8(source_memory);

        if(trigger == time_trigger) {
                trigger_vals = deser_small_uint32_list(source_memory);

        }
}

void automation::print(){
        Serial.print(" \n autom \n " + String((int) id) + " " +  String((int) scen_id) + " " +  String((int)trigger) + " " );
        if(trigger == time_trigger){
          for (std::list<uint32_t>::iterator it = trigger_vals.begin();
               it != trigger_vals.end(); it++)
                  Serial.print(String((int)(*it)));

        }
       Serial.println();
}

std::list<automation> deserialise_auto_list(unsigned char* target_memory){
        std::list<automation> ret;
int autom_size = deser_uint8(&target_memory);
        for(int i=0; i<autom_size; i++) {
                ret.push_back(automation(&target_memory));
        }

        return ret;
}
