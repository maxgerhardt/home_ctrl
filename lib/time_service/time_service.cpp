#include "time_service.h"

extern config_class sys_config;
extern int sys_status;
extern int blink_segment;

void time_trigger_updater() {
	if (sys_config.id_valid) {
		if (sys_config.time_init_done) {
			sys_config.current_time += 1;

			for (std::list<sender_queue_entry>::iterator k =
					sys_config.working_queue.begin();
					k != sys_config.working_queue.end(); k++) {
				//(*k).print();
				//Serial.println(sys_config.current_time);
				if (sys_config.current_time >= (*k).time_to_trigger) {

					//Serial.println("ttt is true");
					(*k).send_now = true;
				}

			}

			for (std::list<automation>::iterator ot =
					sys_config.automations.begin();
					ot != sys_config.automations.end(); ot++) {
				if ((*ot).trigger == time_trigger) {
					//Search if value is correct and then add it
					bool found = false;

					for (std::list<uint32_t>::iterator t =
							(*ot).trigger_vals.begin();
							t != (*ot).trigger_vals.end(); t++) {
						//Serial.println("checking: " + String(sys_config.current_time)+" against: " + (*t));
						if ((*t) == sys_config.current_time)
							found = true;
					}
					if (found) {
						sys_config.automation_trigger_list.push_back((*ot).id);
						//Serial.println("added something to automations");
					}
					//Serial.println("nothing added for automations");
				}

				if ((sys_config.current_time
						== (sys_config.sunrise_time + (24 * 60 * 60) * 0))
						or (sys_config.current_time
								== (sys_config.sunrise_time + (24 * 60 * 60) * 1))
						or (sys_config.current_time
								== (sys_config.sunrise_time + (24 * 60 * 60) * 2))
						or (sys_config.current_time
								== (sys_config.sunrise_time + (24 * 60 * 60) * 3))
						or (sys_config.current_time
								== (sys_config.sunrise_time + (24 * 60 * 60) * 4))
						or (sys_config.current_time
								== (sys_config.sunrise_time + (24 * 60 * 60) * 5))
						or (sys_config.current_time
								== (sys_config.sunrise_time + (24 * 60 * 60) * 6))) {
					if ((*ot).trigger == sunrise) {
						sys_config.automation_trigger_list.push_back((*ot).id);
					}
				}
				if ((sys_config.current_time
						== (sys_config.sunset_time + (24 * 60 * 60) * 0))
						or (sys_config.current_time
								== (sys_config.sunset_time + (24 * 60 * 60) * 1))
						or (sys_config.current_time
								== (sys_config.sunset_time + (24 * 60 * 60) * 2))
						or (sys_config.current_time
								== (sys_config.sunset_time + (24 * 60 * 60) * 3))
						or (sys_config.current_time
								== (sys_config.sunset_time + (24 * 60 * 60) * 4))
						or (sys_config.current_time
								== (sys_config.sunset_time + (24 * 60 * 60) * 5))
						or (sys_config.current_time
								== (sys_config.sunset_time + (24 * 60 * 60) * 6))) {
					if ((*ot).trigger == sunset) {
						sys_config.automation_trigger_list.push_back((*ot).id);
					}
				}

			}

		}
		if (sys_config.current_time >= 604800)
			sys_config.current_time = 0;
	}
}

#define whole_blink_time 4950
void led_ticker_fkt() {
	if (blink_segment % 2 == 0) {
		//gerade
		if (blink_segment < 2 * sys_status) {
			digitalWrite(red_led, HIGH);
			digitalWrite(blue_led, HIGH);
		} else {
			digitalWrite(red_led, LOW);
			digitalWrite(blue_led, LOW);
		}
	} else {
		digitalWrite(red_led, LOW);
		digitalWrite(blue_led, LOW);
	}
	//Serial.println(blink_segment);

	blink_segment++;
	if (blink_segment >= 33)
		blink_segment = 0;
}
