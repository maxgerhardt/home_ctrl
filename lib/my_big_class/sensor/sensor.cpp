#include "sensor.h"

sensor::sensor(unsigned char** source_memory){
        id = deser_uint32(source_memory);
        h_id = deser_uint32(source_memory);
        channel = deser_uint8(source_memory);
        autoSwitchOffTime = deser_uint32(source_memory);
        model= (sensor_model)deser_uint8(source_memory);
        if(model == temp_sens or model == hum_sens) {
                int limsize = deser_uint8(source_memory);
                for(int i=0; i<limsize; i++)
                        limits.push_back(deser_uint8(source_memory));

                th_upper_switchOn=deser_swe_list_raw(source_memory);
                th_upper_switchOff=deser_swe_list_raw(source_memory);
                switch_on_list = deser_swe_list_raw(source_memory);
                switch_off_list = deser_swe_list_raw(source_memory);
        }else{
                switch_on_list = deser_swe_list_raw(source_memory);
                switch_off_list = deser_swe_list_raw(source_memory);
                autoSwitchOff_list = deser_swe_list_raw(source_memory);
        }
}


std::list<sensor> deserialise_sens_list(unsigned char* source_memory){
        std::list<sensor> ret;
        int scen_size = deser_uint8(&source_memory);
        for(int i=0; i<scen_size; i++) {
                ret.push_back(sensor(&source_memory));
        }
        return ret;
}

void sensor::print(){
        Serial.print("\n " + String(id) + " "+ String(h_id) + " " + String(channel) + " " + String(autoSwitchOffTime) + " " + String((int) model) + " entries: \n");
        Serial.print("len: " + String(limits.size()) + " ");
        for (std::list<uint32_t>::iterator it = limits.begin();
             it != limits.end(); it++)
                Serial.print(String((*it)) + " ");
        Serial.print( "\n");
        Serial.print("len: " + String(th_upper_switchOn.size())+ " ");

        for (std::list<switch_entry>::iterator it = th_upper_switchOn.begin();
             it != th_upper_switchOn.end(); it++)
                (*it).print();
        Serial.print( "\n");
        Serial.print("len: " + String(th_upper_switchOff.size())+ " ");

        for (std::list<switch_entry>::iterator it = th_upper_switchOff.begin();
             it != th_upper_switchOff.end(); it++)
                (*it).print();
        Serial.print( "\n");
        Serial.print("len: " + String(switch_on_list.size())+ " ");

        for (std::list<switch_entry>::iterator it = switch_on_list.begin();
             it != switch_on_list.end(); it++)
                (*it).print();
        Serial.print( "\n");
        Serial.print("len: " + String(switch_off_list.size())+ " ");

        for (std::list<switch_entry>::iterator it = switch_off_list.begin();
             it != switch_off_list.end(); it++)
                (*it).print();
        Serial.print( "\n");

        Serial.print("len: " + String(autoSwitchOff_list.size())+ " ");

        for (std::list<switch_entry>::iterator it = autoSwitchOff_list.begin();
             it != autoSwitchOff_list.end(); it++)
                (*it).print();
        Serial.print( "\n");

}
