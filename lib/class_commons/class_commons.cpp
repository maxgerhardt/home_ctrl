#include "class_commons.h"

sensor_change_entry::sensor_change_entry(unsigned char** source_memory){
        id = deser_uint32(source_memory);
        autoSwitchOffTime = deser_uint32(source_memory);
        sensor_model tmp_mod = (sensor_model) deser_uint8(source_memory);

        if(tmp_mod == temp_sens or tmp_mod == hum_sens) {
                int limsize = deser_uint8(source_memory);
                for(int i=0; i<limsize; i++)
                        new_limits.push_back(deser_uint8(source_memory));

                        th_upper_switchOn=deser_swe_list_raw(source_memory);
                        th_upper_switchOff=deser_swe_list_raw(source_memory);
                        new_switchOn_list = deser_swe_list_raw(source_memory);
                        new_switchOff_list = deser_swe_list_raw(source_memory);
                }else{
                        new_switchOn_list = deser_swe_list_raw(source_memory);
                        new_switchOff_list = deser_swe_list_raw(source_memory);
                        new_autoSwitchOff_list = deser_swe_list_raw(source_memory);
                      }
}

void sensor_change_entry::print(){

  Serial.print(String(id) + " " + String(autoSwitchOffTime) + " \n");


  Serial.print("len: " + String(new_limits.size()) + " ");
  for (std::list<uint32_t>::iterator it = new_limits.begin();
       it != new_limits.end(); it++)
          Serial.print(String((*it)) + " ");
  Serial.print("\n");
  Serial.print("len: " + String(th_upper_switchOn.size())+ " ");

  for (std::list<switch_entry>::iterator it = th_upper_switchOn.begin();
       it != th_upper_switchOn.end(); it++)
          (*it).print();
  Serial.print("\n");
  Serial.print("len: " + String(th_upper_switchOff.size())+ " ");

  for (std::list<switch_entry>::iterator it = th_upper_switchOff.begin();
       it != th_upper_switchOff.end(); it++)
          (*it).print();
  Serial.print("\n");
  Serial.print("len: " + String(new_switchOn_list.size())+ " ");

  for (std::list<switch_entry>::iterator it = new_switchOn_list.begin();
       it != new_switchOn_list.end(); it++)
          (*it).print();
  Serial.print("\n");
  Serial.print("len: " + String(new_switchOff_list.size())+ " ");

  for (std::list<switch_entry>::iterator it = new_switchOff_list.begin();
       it != new_switchOff_list.end(); it++)
          (*it).print();
  Serial.print("\n");

  Serial.print("len: " + String(new_autoSwitchOff_list.size())+ " ");

  for (std::list<switch_entry>::iterator it = new_autoSwitchOff_list.begin();
       it != new_autoSwitchOff_list.end(); it++)
          (*it).print();
  Serial.print("\n");


}

std::list<sensor_change_entry> deser_s_change_list(unsigned char** source_mem){
  std::list<sensor_change_entry> ret;

  int s = deser_uint8(source_mem);

  Serial.print(" size of list "  + (int)s);

  for(int i=0; i< s; i++)
          ret.push_back(sensor_change_entry(source_mem));

  return ret;
}


switch_entry::switch_entry(unsigned char** source_mem){
        sens = deser_uint8(source_mem);
        mod_id = deser_uint32(source_mem);
        del = deser_uint32(source_mem);
        Serial.print("\n switch entry made: " + String(sens)+ " " + String(mod_id)+ " " +String(del) + "\n ");
}

std::list<switch_entry> deser_swe_list(unsigned char** source_mem){
        std::list<switch_entry> ret;

        int s = deser_uint8(source_mem);

        Serial.print(" size of list "  + (int)s);

        for(int i=0; i< s; i++)
                ret.push_back(switch_entry(source_mem));

        return ret;
}
std::list<switch_entry> deser_swe_list_raw(unsigned char** source_mem){
        std::list<switch_entry> ret;

        int s = deser_uint8(source_mem);

        Serial.print(" size of list "  + (int)s);

        for(int i=0; i< s; i++) {
                switch_entry tmp = switch_entry();
                tmp.sens = deser_uint8(source_mem);
                tmp.mod_id = deser_uint32(source_mem);
                tmp.del = 0;
                ret.push_back(tmp);
        }
        return ret;
}


void switch_entry::print(){
        Serial.print(String(sens) + " " + String(mod_id) + " " + String(del) + " \n");


}


delayed_trigger::delayed_trigger(std::list<switch_entry> list, uint32_t time) {
  trigger_time = time;
  switch_list = list;
}
