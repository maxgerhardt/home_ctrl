#include "config_class.h"

void config_class::print() {
        Serial.println(title.c_str());
        Serial.println(id);

        for (std::list<module>::iterator it = modules.begin(); it != modules.end();
             it++)
                (*it).print(/*&Serial*/);

        for (std::list<sensor>::iterator it = sensors.begin(); it != sensors.end();
             it++)
                (*it).print(/*&Serial*/);

        for (std::list<scenario>::iterator it = scenarios.begin();
             it != scenarios.end(); it++)
                (*it).print(/*&Serial*/);
        for (std::list<automation>::iterator it = automations.begin();
             it != automations.end(); it++)
                (*it).print(/*&Serial*/);

}

bool config_class::pair_module(uint32_t t_id, uint32_t t_h_id) {
        Serial.println("searching for id= ");
        Serial.println(t_id);
        for (std::list<module>::iterator it = modules.begin(); it != modules.end();
             it++) {
                if ((*it).id == t_id) {
                        (*it).h_id = t_h_id;

                        // NOTE sending pairing command
                        add_to_queue_delay((*it), 150, 0);
                        (*it).print(/*&Serial*/);
                        return true;
                }
        }
        return false;
}
bool config_class::trigger_module(uint32_t t_id, uint32_t target_sens,
                                  Transmitter* sender, IRsend *irsend) {

        for (std::list<module>::iterator it = modules.begin(); it != modules.end();
             it++) {
                //Serial.println((*it).id);
                if ((*it).id == t_id) {
                        Serial.println("found and added to list");
                        //add_to_queue_immediate_switch((*it).h_id, 0, target_sens,(*it).model, (*it).decoder, (*it).ir_h_id);
                        add_to_queue_delay((*it), target_sens, 0);

                        Serial.println("returning");
                        return true;
                }
        }
        return false;
}

bool config_class::pair_sensor(uint32_t t_id, uint32_t t_h_id,
                               uint32_t t_h_ch) {
        Serial.println(
                "new sender got " + String(t_id) + " h id " + String(t_h_id)
                + " channel " + String(t_h_ch));
        for (std::list<sensor>::iterator it = sensors.begin(); it != sensors.end();
             it++) {
                //(*it).print(/*&Serial*/);
                if ((*it).id == t_id) {
                        (*it).h_id = t_h_id;
                        (*it).channel = t_h_ch;
                        return true;
                }
        }
        return false;
}

bool config_class::trigger_scenario(Transmitter* sender, uint32_t sc_id) {
        Serial.println("triggering scenario");
        for (std::list<scenario>::iterator it = scenarios.begin();
             it != scenarios.end(); it++) {
                //(*it).print(/*&Serial*/);

                if ((*it).id == sc_id) {

                        std::list<uint32_t> trigger_mod_list;
                        for (std::list<switch_entry>::iterator i =
                                     (*it).module_switch_on_list.begin();
                             i != (*it).module_switch_on_list.end(); i++) {
                                trigger_mod_list.push_back((*i).mod_id);
                        }
                        if ((*it).all_off) {
                                if (trigger_mod_list.size() > max_size_sw_all_off)
                                        sender->send_all_off();
                                else {
                                        //switch em all of
                                        Serial.println("all of is true");
                                        for (std::list<module>::iterator ot = modules.begin();
                                             ot != modules.end(); ot++) {

                                                bool found = ((std::find(trigger_mod_list.begin(),
                                                                         trigger_mod_list.end(), (*ot).id))
                                                              != trigger_mod_list.end());
                                                if (not found) {
                                                        add_to_queue_delay((*ot), 0, 0);

                                                        Serial.println("turned something off in alloff");
                                                }
                                        }
                                }

                        }
                        trigger_switch_entry_list(sender, (*it).module_switch_on_list);
                        set_scenario(sc_id);
                        Serial.println("trigger done");
                        return true;
                }
        }
        Serial.println("senario id not found");
        return false;
}

bool config_class::trigger_switch_entry_list(Transmitter* sender,
                                             std::list<switch_entry> target) {
        uint32_t carry_delay = 0;
        uint32_t carry_delay_total = 0;
        for (std::list<switch_entry>::iterator ot = target.begin();
             ot != target.end(); ot++) {

                if ((*ot).del > 0) {
                        //Serial.print("delaying : ");
                        //Serial.println((*ot).del);
                        carry_delay += (*ot).del / 1000;

                } else {
                        for (std::list<module>::iterator it = modules.begin();
                             it != modules.end(); it++) { // test if these are correct
                                //(*it).print(/*&Serial*/);

                                if ((*ot).mod_id == (*it).id) {
                                        //Serial.print("triggering:  ");
                                        //Serial.println((*ot).module_id);
                                        //(*it).print(/*&Serial*/);
                                        uint32_t tmp = (*ot).sens;
//TODO IF INFRA DO SOMETHING ELSE

                                        //normal units
                                        /*if (carry_delay == 0 and carry_delay_total == 0)
                                           add_to_queue_immediate_switch((*it).h_id, 0, tmp,
                                           (*it).model, (*it).decoder, (*it).ir_h_id);
                                           else {*/
                                        add_to_queue_delay((*it), tmp,
                                                           carry_delay + carry_delay_total);

                                        carry_delay_total += carry_delay;
                                        carry_delay = 0;
                                        //Serial.println(								"current time = " + String(current_time)										+ " triggering: "										+ String(												current_time+ carry_delay_total));

                                        //Serial.println("added to queue");

                                        //}
                                }
                        }

                        //(*it).self_trigger(sender,this);
                }

        }
        return true;

}

bool config_class::trigger_sensor(Transmitter* sender, data_434_pack* data) {
        yield();
        if (active_scene_id != 0) {
                Serial.println("we have active scenario");
                for (std::list<scenario>::iterator ot = scenarios.begin();
                     ot != scenarios.end(); ot++) {
                        if ((*ot).id == active_scene_id) {
                                Serial.println("found active scenario");
                                for (std::list<sensor_change_entry>::iterator s =
                                             (*ot).sensor_changing_list.begin();
                                     s != (*ot).sensor_changing_list.end(); s++) {
                                        for (std::list<sensor>::iterator se = sensors.begin();
                                             se != sensors.end(); se++) {
                                                //Serial.println(								"checking: " + String((*se).h_id) + " against "										+ String(data->address));
                                                if ((*se).h_id == data->address) {
                                                        Serial.println("adress hit");
                                                        if (data->state_bit) {
                                                                trigger_switch_entry_list(sender,
                                                                                          (*s).new_switchOn_list);
                                                                delay_triggering.push_back(
                                                                        delayed_trigger(
                                                                                (*s).new_autoSwitchOff_list,
                                                                                (((*s).autoSwitchOffTime) * 1000
                                                                                 + millis())));
                                                        } else
                                                                trigger_switch_entry_list(sender,
                                                                                          (*s).new_autoSwitchOff_list);
                                                        Serial.println(
                                                                "sensor triggered with respect to scenario");
                                                        return true;
                                                }
                                        }

                                }

                        }
                }
        }
        for (std::list<sensor>::iterator ot = sensors.begin(); ot != sensors.end();
             ot++) {
                Serial.println(
                        "checking: " + String(data->address) + String(" against : ")
                        + String((*ot).h_id) + " channel bit = "
                        + String((*ot).channel) + " against "
                        + String(data->channel));
                if ((*ot).h_id == data->address)
                        Serial.println("address is the same,"
                                       ",");
                if ((*ot).h_id == data->address and ((*ot).channel == data->channel)) {
                        Serial.println("Found adress in sensor list");
                        if (data->state_bit) {
                                trigger_switch_entry_list(sender, (*ot).switch_on_list);
                                delay_triggering.push_back(
                                        delayed_trigger((*ot).autoSwitchOff_list,
                                                        (((*ot).autoSwitchOffTime) * 1000 + millis())));

                        } else
                                trigger_switch_entry_list(sender, (*ot).switch_off_list);

                        Serial.println("triggered sensor");
                        return true;
                }

        }
        Serial.println("sensor received but not found in list");
        return false;

}

bool config_class::check_for_delayed_triggers(Transmitter* sender) {
        for (std::list<delayed_trigger>::iterator ot = delay_triggering.begin();
             ot != delay_triggering.end(); ot++) {
                if ((*ot).trigger_time < millis()) {
                        trigger_switch_entry_list(sender, (*ot).switch_list);
                        delay_triggering.erase(ot);
                        Serial.println("triggered and removed entry");
                        return true;
                }
        }

        return false;
}

bool config_class::check_for_automation(Transmitter* sender,
                                        temp_hum_s * main_sens) {
        if (!time_init_done)
                return false;
        if (update_time(sender))
                return true;

        temp_valid = update_temp(main_sens);
        hum_valid = update_hum(main_sens);

        if (temp_valid)
                check_limits(sender);

        if (temp_valid and hum_valid)
                update_extern_val();

        return false;
}

bool config_class::check_limits(Transmitter* sender) {
        if ((millis() - lt_limit_check) < 10000)
                return true;
        else
                lt_limit_check = millis();
        //Serial.println("checking limits");
        if (active_scene_id == 0) {

                for (std::list<sensor>::iterator ot = sensors.begin();
                     ot != sensors.end(); ot++) {
                        if ((*ot).model == temp_sens) {
                                if (current_temp < (*ot).limits.front()) {
                                        if (temp_cur_state == under)
                                                return true;
                                        temp_cur_state = under;
                                        trigger_switch_entry_list(sender, (*ot).switch_on_list);
                                        trigger_switch_entry_list(sender, (*ot).switch_off_list);
                                } else if (current_temp > (*ot).limits.back()) {
                                        if (temp_cur_state == over)
                                                return true;
                                        temp_cur_state = over;
                                        trigger_switch_entry_list(sender, (*ot).th_upper_switchOn);
                                        trigger_switch_entry_list(sender, (*ot).th_upper_switchOff);
                                } else

                                        temp_cur_state = neutral;
                        }
                        if ((*ot).model == hum_sens) {
                                if (current_hum < (*ot).limits.front()) {
                                        if (hum_cur_state == under)
                                                return true;
                                        hum_cur_state = under;
                                        trigger_switch_entry_list(sender, (*ot).switch_on_list);
                                        trigger_switch_entry_list(sender, (*ot).switch_off_list);
                                } else if (current_hum > (*ot).limits.back()) {
                                        if (hum_cur_state == over)
                                                return true;
                                        hum_cur_state = over;
                                        trigger_switch_entry_list(sender, (*ot).th_upper_switchOn);
                                        trigger_switch_entry_list(sender, (*ot).th_upper_switchOff);
                                } else

                                        hum_cur_state = neutral;
                        }
                }
        } else {
                for (std::list<scenario>::iterator ot = scenarios.begin();
                     ot != scenarios.end(); ot++) {
                        if ((*ot).id == active_scene_id) {
                                for (std::list<sensor>::iterator it = sensors.begin();
                                     it != sensors.end(); it++) {
                                        for (std::list<sensor_change_entry>::iterator in =
                                                     (*ot).sensor_changing_list.begin();
                                             in != (*ot).sensor_changing_list.end(); in++) {

                                                if ((*in).id == (*it).id) {

                                                        if ((*it).model == temp_sens) {
                                                                if (current_temp < (*in).new_limits.front()) { // TODO limits and neutral state temp
                                                                        Serial.println("under t");
                                                                        if (temp_cur_state == under)
                                                                                return true;
                                                                        temp_cur_state = under;

                                                                        trigger_switch_entry_list(sender,
                                                                                                  (*in).new_switchOn_list);
                                                                        trigger_switch_entry_list(sender,
                                                                                                  (*in).new_switchOff_list);
                                                                } else if (current_temp
                                                                           > (*in).new_limits.back()) {
                                                                        Serial.println("over t ");

                                                                        if (temp_cur_state == over)
                                                                                return true;
                                                                        temp_cur_state = over;

                                                                        trigger_switch_entry_list(sender,
                                                                                                  (*in).th_upper_switchOn);
                                                                        trigger_switch_entry_list(sender,
                                                                                                  (*in).th_upper_switchOff);
                                                                } else
                                                                        temp_cur_state = neutral;
                                                        }
                                                        if ((*it).model == hum_sens) {
                                                                if (current_hum < (*in).new_limits.front()) {
                                                                        Serial.println("under");

                                                                        if (hum_cur_state == under)
                                                                                return true;
                                                                        hum_cur_state = under;
                                                                        trigger_switch_entry_list(sender,
                                                                                                  (*in).new_switchOn_list);
                                                                        trigger_switch_entry_list(sender,
                                                                                                  (*in).new_switchOff_list);
                                                                } else if (current_hum
                                                                           > (*in).new_limits.back()) {
                                                                        if (hum_cur_state == over)
                                                                                return true;
                                                                        hum_cur_state = over;
                                                                        trigger_switch_entry_list(sender,
                                                                                                  (*in).th_upper_switchOn);
                                                                        trigger_switch_entry_list(sender,
                                                                                                  (*in).th_upper_switchOff);
                                                                } else

                                                                        hum_cur_state = neutral;
                                                        }

                                                }

                                        }

                                }

                        }

                }

        }
        return true;
}

bool config_class::update_extern_val() {
        if (millis() - last_time_push < 20000)
                return true;

        HTTPClient httpr;

        httpr.begin(
                "http://intertechno-smarthome-api.esy.es/api/v1/config/update_temp_humidity/"
                + id);
        httpr.addHeader("Content-Type", "application/json");
        int httpCode = -1;
        do {
                int httpCode = httpr.POST(
                        "{\"temperature\": \"" + String(current_temp)
                        + "\",\"humidity\": \"" + String(current_hum) + "\"}");
        } while(httpCode < 0 );
        httpr.end();
        last_time_push = millis();
        return true;
}

// TODO defines move it somewhere else
#define time_port 2390

const char* ntpServerName = "time.nist.gov";
#define NTP_PACKET_SIZE 48 // NTP time stamp is in the first 48 bytes of the messa+ge
WiFiUDP time_udp;
// END def TODO
bool config_class::update_time(Transmitter* sender) {

        for (std::list<uint32_t>::iterator ot =
                     automation_trigger_list.begin();
             ot != automation_trigger_list.end(); ot++) {
                Serial.println("time list auto " + String((*ot)));
                trigger_automation(sender, (*ot));
                automation_trigger_list.erase(ot);
                return true;
        }

//Serial.println("current time : " +String(current_time));
        return false;
}

bool config_class::set_up_time() {
        IPAddress timeServerIP;
        byte packetBuffer[ NTP_PACKET_SIZE];

        WiFi.hostByName(ntpServerName, timeServerIP);
        sendNTPpacket(timeServerIP);
        time_udp.begin(time_port);
        int cb = time_udp.parsePacket();

        uint32_t timeout = millis();
        while (!cb) {
                if ((millis() - timeout) > 1000) {
                        //Serial.println("time request timeout");
                        return false;
                }
                delay(10);
                cb = time_udp.parsePacket();

        }
        if (cb) {

                time_udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
                uint32_t highWord = word(packetBuffer[40], packetBuffer[41]);
                uint32_t lowWord = word(packetBuffer[42], packetBuffer[43]);
                uint32_t secsSince1900 = highWord + 16 | lowWord;
                const uint32_t seventyYears = 2208988800UL;
                uint32_t epoch = secsSince1900 - seventyYears;
                time_t t = epoch;
                struct tm *tm = localtime(&t);
                uint32_t ot;
                ot += (tm->tm_wday - 1) * 24 * 60 * 60;
                ot += (tm->tm_hour + tz) * 60 * 60;
                ot += (tm->tm_min * 60);
                ot += (tm->tm_sec);

                //Dusk2Dawn local_times(latitute, longtitute, tz);
                Dusk2Dawn local_times(longtitute, latitute, +2);
                uint32_t laSunrise = local_times.sunrise(2018, 05, 25,
                                                         daylightsave);
                sunrise_time = laSunrise * 60;
                uint32_t laSunset = local_times.sunset(tm->tm_year + 1900,
                                                       tm->tm_mon, tm->tm_mday, daylightsave);
                sunrise_time = laSunset * 60;

                current_time = ot;

                Serial.println("time set up");
                time_init_done = true;
                return true;

        } else
                return false;

        return false;
}
#define time_between_temp_hum_update 5000 // 5 minutes

bool config_class::update_temp(temp_hum_s * main_sens) {
//Serial.println("updated temp");

        if ((millis() - last_time_temp) < time_between_temp_hum_update
            and temp_valid)
                return true;
        int tmp = main_sens->get_temp();
        if (tmp > 50 or tmp < 0)
                return false;
        else
                current_temp = tmp;
//Serial.println("got temp: " + String(current_temp));
        //Serial.println("tmp updated");
        last_time_temp = millis();

        return true;
}
bool config_class::update_hum(temp_hum_s * main_sens) {
//Serial.println("updated hum");
        if ((millis() - last_time_hum) < time_between_temp_hum_update and hum_valid)
                return true;
        int tmp = main_sens->get_hum();
        if (tmp > 100 or tmp < 0)
                return false;
        else
                current_hum = tmp;
        //Serial.println("hum   updated");
        last_time_hum = millis();
//Serial.println("got hum: " + String(current_hum));
        return true;
}

void config_class::sendNTPpacket(IPAddress& address) {
//Serial.println("sending NTP packet...");
        byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
// set all bytes in the buffer to 0
        memset(packetBuffer, 0, NTP_PACKET_SIZE);
// Initialize values needed to form NTP request
// (see URL above for details on the packets)
        packetBuffer[0] = 0b11100011; // LI, Version, Mode
        packetBuffer[1] = 0; // Stratum, or type of clock
        packetBuffer[2] = 6; // Polling Interval
        packetBuffer[3] = 0xEC; // Peer Clock Precision
// 8 bytes of zero for Root Delay & Root Dispersion
        packetBuffer[12] = 49;
        packetBuffer[13] = 0x4E;
        packetBuffer[14] = 49;
        packetBuffer[15] = 52;

// all NTP fields have been given values, now
// you can send a packet requesting a timestamp:
        time_udp.beginPacket(address, 123); //NTP requests are to port 123
        time_udp.write(packetBuffer, NTP_PACKET_SIZE);
        time_udp.endPacket();
}

bool config_class::trigger_automation(Transmitter* sender, uint32_t id) {
        for (std::list<automation>::iterator ot = automations.begin();
             ot != automations.end(); ot++) {

                if (id == (*ot).id)
                        trigger_scenario(sender, (*ot).scen_id);

        }
        return true;
}

void config_class::add_to_queue_delay(module i,
                                      uint32_t sensitivity_target, uint32_t delay) {
        Serial.println(
                "added to queue with delay " + String(i.id) + " del : "
                + String(delay) + " sens: " + String(sensitivity_target)
                + " plus " + String(current_time + delay));

        working_queue.push_back(
                sender_queue_entry(current_time + delay, i, sensitivity_target,
                                   false));

        /*for (std::list<sender_queue_entry>::iterator ot = working_queue.begin();
           ot != working_queue.end(); ot++)
           (*ot).print();*/
}

bool config_class::process_working_queue(Transmitter* sender,
                                         IRsend* irsender) {
        if ((millis() - sender->get_last_time()) < 10) {
                //Serial.println("blocked call");
                return false;
        }
//working_queue.sort();
//working_queue.unique();
//if (working_queue.size() <= 0)
//	return false;
        for (std::list<sender_queue_entry>::iterator ot = working_queue.begin();
             ot != working_queue.end(); ot++) {

                //Serial.println("searching");
                if ((*ot).send_now) {
                        if ((*ot).is_all_off) {
                                sender->send_all_off();
                                working_queue.erase(ot);
                                return true;
                        } else {
                                Serial.println(
                                        "sending" + String((*ot).mod_to_trigger.h_id) + " sens "
                                        + String((*ot).sensitivity) + " type: "
                                        + String((*ot).mod_to_trigger.model));

                                //different send types
                                uint32_t tmp1, tmp2;
                                switch ((*ot).mod_to_trigger.model) {
                                case dimmer:
                                        Serial.println("triggered dimmer");

                                        tmp1 = (*ot).mod_to_trigger.h_id;
                                        tmp2 = (*ot).sensitivity;
                                        if (tmp2 > 100)
                                                sender->sendonOff(tmp1, 0, 100);
                                        else
                                                sender->sendDim(tmp1, 0, tmp2);
                                        break;

                                case infrarot:
                                        Serial.println("triggered infra");

                                        if (not send_infra(irsender, (*ot).mod_to_trigger.ir_h_id))
                                                Serial.println("ir array had no data");
                                        Serial.println("sending infra");

                                        break;
                                case jalousieSwitch:
                                        Serial.println("triggered jal sw");
                                        tmp1 = (*ot).mod_to_trigger.h_id;
                                        tmp2 = (*ot).sensitivity;
                                        sender->sendonOff(tmp1, 0, tmp2);
                                        break;
                                case onOffSwitch:
                                        tmp1 = (*ot).mod_to_trigger.h_id;
                                        tmp2 = (*ot).sensitivity;
                                        Serial.println(
                                                "triggered normal mod " + String(tmp1) + " X "
                                                + String(tmp2));

                                        if (tmp2 >= 100)
                                                sender->sendonOff(tmp1, 0, 100);
                                        else

                                                sender->sendonOff(tmp1, 0, tmp2);
                                        Serial.println("sent");
                                        break;
                                case gong:
                                        tmp1 = (*ot).mod_to_trigger.h_id;
                                        tmp2 = (*ot).sensitivity;
                                        Serial.println(
                                                "triggered normal mod " + String(tmp1) + " X "
                                                + String(tmp2));

                                        sender->sendonOff(tmp1, 0, 100);
                                        Serial.println("gong sent");
                                        break;
                                default:
                                        Serial.println("undefined model type ...");
                                        break;
                                }

                                //(*ot).mod_to_trigger.print(/*&Serial*/);
                                Serial.println("done triggering");
                                working_queue.erase(ot);
                                return true;
                        }
                } //else
                  //(*ot).print(); Serial.println("is waiting for " + String(current_time));
        }
        return false;
}

bool config_class::handle_outside_call(Transmitter* sender) {
        if ((millis() - last_time_called) < 5000)
                return false;
//Serial.println("outside call");
        HTTPClient http;
        HTTPClient httpr;
        http.setReuse(true);
        http.begin(
                "http://intertechno-smarthome-api.esy.es/api/v1/scenarios/get/"
                + id);
        int httpCode = http.GET();
//Serial.println(httpCode);
        if(httpCode > 0) {
                String payload = http.getString(); //Get the request response payload
//Serial.println(payload);

                DynamicJsonBuffer jsonBuffer(DEF_BUF_SIZE);
                JsonArray& root = jsonBuffer.parseArray(payload);

                JsonObject& root_0 = root[0];
                //  const char* root_0_id = root_0["id"]; // "1523988908530"
                //  const char* root_0_event = root_0["event"]; // "scenario"
                const char* root_0_scenario_id = root_0["scenario_id"]; // "1525080897948"
                const char* root_0_new_cmd = root_0["new_cmd"]; // "0"
                //  const char* root_0_is_updating = root_0["is_updating"]; // "0"

                if (String(root_0_new_cmd).equals("1")) {
                        trigger_scenario(sender, atol(root_0_scenario_id));
                        httpr.begin(
                                "http://intertechno-smarthome-api.esy.es/api/v1/scenarios/update_new_cmd/"
                                + id);
                        httpr.addHeader("Content-Type", "application/json");
                        httpCode = httpr.POST("{\"new_cmd\":\"0\"}");
                        return true;

                }
        }
        httpr.end();
        http.end();
        last_time_called = millis();
        return false;
}

#define max_ir_cmd_length 200
bool config_class::pair_ir_mod(uint32_t t_id, decode_results* tmp) {
        Serial.println("searching for id= ");
        Serial.println(t_id);
        for (std::list<module>::iterator it = modules.begin(); it != modules.end();
             it++) {

                if ((*it).id == t_id) {
                        //(*it).ir_h_id = tmp->value; TODO give this thing a value! ir has no vlaue currently after pairing
                        if (tmp->rawlen > max_ir_cmd_length) {
                                Serial.println("ir command too large");
                                return false;
                        } else {
                                for (uint16_t i = 1; i < tmp->rawlen; i++) {
                                        uint32_t usecs;
                                        for (usecs = tmp->rawbuf[i] * RAWTICK; usecs > UINT16_MAX;
                                             usecs -= UINT16_MAX) {
                                                /*// output += uint64ToString(UINT16_MAX);
                                                   if (i % 2)
                                                   output += ", 0,  ";
                                                   else
                                                   output += ",  0, ";
                                                   }*/
                                        }
                                        (*it).ir_h_id.push_back(usecs);
                                }
                        }

                        //(*it).decoder = tmp->decode_type;
                        (*it).print(/*&Serial*/);

                        return true;
                }
        }
        return false;
}


bool testingstuff(){
	return false;
}

void dummy::print(){
	Serial.println("garbage");
}
