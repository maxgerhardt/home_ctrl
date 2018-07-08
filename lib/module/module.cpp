#include "module.h"





module::module(int t_id,int t_h_id,int t_m){
        id = (uint32_t) t_id;
        h_id = (uint32_t) t_h_id;
        model = (module_model) t_m;

        if(model == infrarot) { // give infrared test values
                for(int i=0; i<200; i++)
                        ir_h_id.push_back(i);
        }
}

module::module(unsigned char** source_memory_ptr){

//id

        id = deser_uint32(source_memory_ptr);
        Serial.print( "deserialisierer id = " + String(id) + "\n");

// model
        model = (module_model) deser_uint8(source_memory_ptr);
        Serial.print( "deserialisierer model = " + String(model) + "\n");

// h_id
        if(model != infrarot) {
                h_id = deser_uint32(source_memory_ptr);

                Serial.print( "deserialisierer hid = " + String(h_id) + "\n");

        }else{
                //Serial.print( "model is infra \n");
                h_id =0;
                ir_h_id = deser_uint16_list(source_memory_ptr);
        }



}


void module::print(){
        Serial.print( " id: " + String((uint32_t) id)   + "; model = " + (int) model + "; h_id = " +  String((uint32_t) h_id) + "\n");
}




std::list<module> deserialise_mod_list(unsigned char* target_memory){
        std::list<module> ret;

        int s = deser_uint8(&target_memory);

        Serial.print( " size of list " +  (int)s);

//target_memory+=1;
        for(int i=0; i< s; i++)
                ret.push_back(module(&target_memory));


        return ret;

}


sender_queue_entry::sender_queue_entry(uint32_t t, module i, uint32_t sens,bool ao) {

  time_to_trigger = t;
  mod_to_trigger = i;
  sensitivity = sens;

  is_all_off = ao;
  if(t == 0){
    send_now=true;
  }else
    send_now=false;
  }


  bool sender_queue_entry::operator ==(sender_queue_entry comp) {
    if (comp.mod_to_trigger.id == mod_to_trigger.id
        and comp.is_all_off == is_all_off and comp.send_now == send_now
        and comp.sensitivity == sensitivity
        and comp.time_to_trigger == comp.time_to_trigger)
      return true;
    else
      return false;
  }
  bool sender_queue_entry::operator <(sender_queue_entry comp) {
    if (comp.time_to_trigger == comp.time_to_trigger)
      return true;
    else
      return false;
  }
