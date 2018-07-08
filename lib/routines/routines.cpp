#include "routines.h"

bool routine_con_WiFi(fs_eeprom * eeprom, network_info* net) {
	if (not eeprom->read_net_config(&Serial, net)) {
		//net->print(&Serial);
		sys_status = 4;

		if (not net->smartconfig()) {
			return false;
		}
	} else {
		Serial.println("read valid conf");
		if (net->search_network(&Serial, &WiFi, net->_ssid)) {
			if (not net->connect(&WiFi)) {
				if (net->smartconfig()) {
					eeprom->write_net_config(&Serial, net);

					return true;
				} else
					return false;
			}
		} else {
			if (net->smartconfig()) {
				eeprom->write_net_config(&Serial, net);

				return true;
			} else
				return false;
		}
	}
	eeprom->write_net_config(&Serial, net);
	return true;
}

bool wifi_check(ESP8266WiFiClass* Wi) {
	if (Wi->status() == WL_CONNECTED)
		return true;
	else
		return false;
}

bool tcp_recv_loop(fs_eeprom* eeprom, receiver* recv_hand, config_class *conf,
		Transmitter* sender, IRrecv*irrecv, IRsend *irsend,
		temp_hum_s* main_sens) {

	WiFiClient client;

	WiFiServer server(localTcpPort);
	WiFiUDP Udp;
	Udp.begin(localUdpPort);

	Serial.println("user loop");

	if (WiFi.status() != WL_CONNECTED) {
		return false;
	}

	//recv_hand->receiver_setup();

	server.begin();

	/*	for (;;) {
	 yield();
	 Serial.print("P");
	 delay(1000);
	 }*/
	client = server.available();
	unsigned long last_time_stat_update = millis();
	for (;;) {
		//led_ticker_fkt();
		yield();
		if (not wifi_check(&WiFi))
			return false;
		if (client and conf->time_init_done) { // TODO receiver ausschalten wenn tcp und wieder ein danach
			//recv_hand->stop_interrupt();
			Serial.println("we got a client");
			uint32_t start_timeout_tcp = millis();
			while (client.connected()) {

				if ((start_timeout_tcp + 10000) < millis()) {
					Serial.println("breaking from tcp socket");
					client.stop();
					//server.end();
					server.close();
					server.begin();
				}

				while (client.available()) {
					String TCP_Buf;
					uint32_t threshhold = millis();
					while (client.available() or (millis() < (threshhold + 200))) {
						char a = client.read();
						if (a > 0)
							TCP_Buf += a;
						//Serial.println(a, HEX);
						delay(1);
					}
					//String TCP_Buf(client.readStringUntil('\r'));
					/*for (int o = 0; o < TCP_Buf.length(); o++)
					 Serial.println(TCP_Buf.c_str()[o], HEX);*/
					//String TCP_Buf(client.readStringUntil('\n'));
					//String TCP_Buf(client.readStringUntil('X'));
					//TCP_Buf.setCharAt(TCP_Buf.length() - 1, '\0');
					//Serial.println(TCP_Buf.c_str());
					if (not handleReceivedMessage(eeprom, TCP_Buf, &client,
							conf, sender, recv_hand, irrecv, irsend))
						Serial.println("config invalid");

					client.stop();
					TCP_Buf = "";
					Serial.println("client processing done");

					start_timeout_tcp = millis();
					if (conf->id_valid) {
						Serial.println("we have a valid config");
					}
					if (conf->id_valid)
						return true; //TODO REMOVE THIS!!!!!

				}

			}
		} else {
			recv_hand->start_interrupt();
			client = server.available();
			int packetSize = Udp.parsePacket();
			if (packetSize) {
				char incomingPacket[255];
				Serial.printf("Received %d bytes from %s, port %d\n",
						packetSize, Udp.remoteIP().toString().c_str(),
						Udp.remotePort());
				Serial.println();
				int len = Udp.read(incomingPacket, 255);
				if (len > 0) {
					incomingPacket[len] = 0;
				}
				Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
				String replyPacket = "{\"id\":\"";
				String id, title;
				if (conf->id_valid) {
					id = String(conf->id);
					title = String(conf->title);
				} else {
					id = String(0);
					title = String("empty name");
				}
				replyPacket += id;
				replyPacket += "\",\"name\":\"";
				replyPacket += title;
				replyPacket += "\",\"c\":\"inter\"}";

				Serial.println(replyPacket.c_str());
				Udp.write(replyPacket.c_str());
				Udp.endPacket();
			}
			if (conf->time_init_done and conf->id_valid) {
				if ((millis() - last_time_stat_update) > 20000) {
					sys_status = 0;
					last_time_stat_update = millis();

				}

				if (!recv_hand->most_recent_list.empty()) {
					//Serial.println("triggering");
					conf->trigger_sensor(sender,
							&(recv_hand->most_recent_list.front()));
					recv_hand->most_recent_list.pop_front();
				}

				if (conf->process_working_queue(sender, irsend))
					Serial.println("triggered something from list");

				if (conf->check_for_automation(sender, main_sens))
					Serial.println("automation checked");

				if (conf->check_for_delayed_triggers(sender))
					Serial.println("triggered delay");

				if (conf->handle_outside_call(sender))
					Serial.println("handled outside call");

				/*decode_results results;
				 if (irrecv->decode(&results)) {
				 Serial.println("ir received");
				 if (results.overflow)
				 Serial.printf(
				 "WARNING: IR code is too big for buffer (>= %d). "
				 "This result shouldn't be trusted until this is resolved. "
				 "Edit & increase CAPTURE_BUFFER_SIZE.\n",
				 CAPTURE_BUFFER_SIZE);
				 Serial.println(resultToSourceCode(&results));
				 Serial.println(results.rawlen);
				 //for (int i = 0; i < results.rawlen; i++)
				 //Serial.println(results.rawbuf[i]);
				 Serial.println(
				 uint64ToString(getCorrectedRawLength(&results),
				 10));

				 uint16_t arr[200];
				 Serial.println(uint64ToString(UINT16_MAX));
				 String output = "";

				 for (uint16_t i = 1; i < results.rawlen; i++) {
				 uint32_t usecs;
				 for (usecs = results.rawbuf[i] * RAWTICK;
				 usecs > UINT16_MAX; usecs -= UINT16_MAX) {
				 // output += uint64ToString(UINT16_MAX);
				 if (i % 2)
				 output += ", 0,  ";
				 else
				 output += ",  0, ";
				 }
				 }
				 output += uint64ToString(usecs, 10);
				 arr[i] = usecs;

				 //for (int i = 1; i < results.rawlen; i++)
				 //Serial.print(uint64ToString(arr[i]) + String(" "));

				 Serial.println();

				 }*/

			}

		}

		if (!conf->time_init_done) { // TODO or 2 days have passed !
			sys_status = 5;
			conf->set_up_time();

		} else if (!conf->id_valid)
			sys_status = 6;
	}
	server.close();
	return true;

}

bool handleReceivedMessage(fs_eeprom* eeprom, String message,
		WiFiClient *client, config_class *conf, Transmitter* sender,
		receiver* recv_hand, IRrecv*irrecv, IRsend *irsend) {

//Serial.println("raw print");
	Serial.println(message);
//Serial.println("raw print");
	Serial.print(ESP.getFreeHeap());
	Serial.println(
			"message size: " + String(message.length()) + " X "
					+ String(message.length() * 3));
	DynamicJsonBuffer jsonBuffer(/*message.length() * 3*/20000);
	JsonObject& parsed = jsonBuffer.parseObject(message); //Parse message

	if (!parsed.success()) {   //Check for errors in parsing

		Serial.println("Parsing failed");
		sys_status = 7;

		return false;

	} else {
		Serial.println("Parsing ok");
	}

//parsed.prettyPrintTo(Serial);

	const char* event_type = parsed["event"];

	if (String(event_type) == String("restart")) {
		eeprom->remove_net_config();
		eeprom->remove_String_config();
		ESP.restart();
	} else if (String(event_type) == String("print_time")) {
		conf->print();
	} else if (String(event_type) == String("pair_cmd")) { // für module pairen
		if (not conf->id_valid)
			return false;
		sys_status = 1;

		Serial.println("pair cmd");
		const char* module_id = parsed["module_id"]; // "1525078165890"
		const char* h_id = parsed["h_id"]; // "40272832"
		unsigned long tmp_id = atoll(module_id);
		unsigned long tmp_h_id = atoll(h_id);
		Serial.println("pair module funktion");

		if (conf->pair_module(tmp_id, tmp_h_id)) {
			Serial.println("sending on command");
			//sender->sendUnit(tmp_h_id, 0, true);
			//conf->add_to_queue_immediate_switch(tmp_h_id, 0, 100,-1,onOffSwitch,0);
			/*int bytes_written = eeprom->serialise_config_class(conf, 0);
			 Serial.print("Bytes Written = ");
			 Serial.println(bytes_written);*/

			return true;

		} else
			Serial.println("pairing failed");
		return true;
	} else if (String(event_type) == String("pair_cmd_old")) { // für module pairen
		if (not conf->id_valid)
			return false;
		//const char* module_id = parsed["module_id"]; // "1525078165890"
		//const char* h_id = parsed["h_id"]; // "M14"   // erster char ist immer nur 1 char der rest is was anderes, auf string casten und concat
		//unsigned long tmp_id = atoll(module_id);
		//unsigned long tmp_h_id = atoll(h_id);
		//if (conf->pair_module(tmp_id, tmp_h_id)) {
		//sender->sendUnit(tmp_h_id, 0, true);
		//return true;

		//}
		return false;
	} else if (String(event_type) == String("user_conf")) { // DONE
		sys_status = 8;

		Serial.println("received new user config");
		//JsonObject& data = parsed["data"];

		const char* data_id = parsed["data"]["id"]; // "153087166825714064547"
		if (data_id == NULL)
			return false;
		*conf = config_class();
		conf->id = String(data_id);
		conf->id_valid = true;
		Serial.println("ID = " + conf->id);
		client->println("{\"event\":\"user_conf\",\"success\":\"1\"}");

		//int bytes_written = eeprom->serialise_config_class(conf, 0);
		//Serial.println("Bytes Written = "+ bytes_written);

		//conf->print(&Serial);
		return true;
	} else if (String(event_type) == String("module")) { // ein aus commando von der app
		Serial.println("module command");
		if (not conf->id_valid)
			return false;
		sys_status = 1;

		const char* tmp_id = parsed["id"]; // "40272832"
		unsigned long tmp_num = atoll(tmp_id);

		bool t_stat = parsed["state"];
		unsigned long tmp_sens;
		if (t_stat)
			tmp_sens = 100;
		else
			tmp_sens = 0;
		Serial.print("module command triggering: ");
		Serial.println(tmp_id);
		conf->trigger_module(tmp_num, tmp_sens, sender, irsend);
		return true;
	} else if (String(event_type) == String("dimmer")) { // ein aus commando von der app
		if (not conf->id_valid)
			return false;
		sys_status = 1;

		Serial.println("manual dimmer");

		const char* tmp_id = parsed["id"]; // "40272832"
		unsigned long tmp_num = atoll(tmp_id);

		unsigned long tmp_sens = parsed["state"];

		conf->trigger_module(tmp_num, tmp_sens, sender, irsend);

	} else if (String(event_type) == String("pair_sensor")) { // ein aus commando von der app
		if (not conf->id_valid)
			return false;
		sys_status = 1;

		//return true; // TODO remove this, hardware bug, receiving does not work
		Serial.println("trying to copple sensor");

		//sensor_id -> unique id from object
		//connect to app via open socket, pair_sensor öffnen einen socket, der wird NICHT gesvhlossen, über den kommt die neue h_id vom sensor zurück zur app.
		data_434_pack new_data = wait_for_pairing_sensor();
		Serial.println("new data pack");
		const char* sensor_id = parsed["sensor_id"];	// "1525078165890"
		unsigned long tmp_id = atoll(sensor_id);
		//!!!!!///
		Serial.println("\n new_adress received");
		if (not new_data.valid) {
			Serial.println("sensor not found ");
			sys_status = 9;

		} else {
			Serial.println("success, got adress");
			if (conf->pair_sensor(tmp_id, new_data.address, new_data.channel)) {
				Serial.println("successfully added  \n");
			} else {
				Serial.println("something failed adding he adress to the json");
				return false;
			}
			String tmp0("{\"event\":\"pair_sensor\",\"h_id\":\"");
			tmp0 += String(new_data.address);
			tmp0 += String("\",\"channel\":\"");
			tmp0 += String(new_data.channel);
			tmp0 += String("\"}");
			Serial.println("processing client");
			Serial.println(tmp0.c_str());
			client->println(tmp0);
			Serial.println("done");

			/*int bytes_written = eeprom->serialise_config_class(conf, 0);
			Serial.print("Bytes Written = ");
			Serial.println(bytes_written);*/
			return true;
			//client.stop();
		}

		return false;

	}

	else if (String(event_type) == String("delete_config")) { // ein aus commando von der app
		if (not conf->id_valid)
			return false;
		eeprom->remove_String_config();

	} else if (String(event_type) == String("delete_network")) { // ein aus commando von der app
		eeprom->remove_net_config();
		ESP.restart();

	} else if (String(event_type) == String("scenario")) { // ein aus commando von der app
		if (not conf->id_valid)
			return false;
		sys_status = 1;
		if (recv_hand->init)
			Serial.println("is active");
		else
			Serial.println("is not active");
		Serial.println("triggering scenario");
		const char* scenario_id = parsed["scenario_id"]; // "1612516761"
		Serial.println(scenario_id);
		if (String(scenario_id) == String("default")) {
			conf->set_scenario(0);
			return true;
		}
		unsigned long tmp_scen = atoll(scenario_id);

		conf->set_scenario(tmp_scen);
		if (conf->trigger_scenario(sender, tmp_scen))
			Serial.println("scene success trigger");
		return true;
	} else if (String(event_type) == String("print_config")) { // ein aus commando von der app
		if (not conf->id_valid)
			return false;
		conf->print();

	} else if (String(event_type) == String("pair_infrared")) {
		if (not conf->id_valid)
			return false;
		sys_status = 1;

		const char* sensor_id = parsed["sensor_id"];	// "1525078165890"
		unsigned long tmp_id = atoll(sensor_id);

		Serial.println("triggered infra pairing");
		decode_results resp = pair_infra(irrecv);
		if (resp.decode_type <= 0) {
			Serial.println("unknown decoder type");
			sys_status = 10;

			return false;
		} else {
			Serial.println("trying to save");
			resultToHumanReadableBasic(&resp);
			conf->pair_ir_mod(tmp_id, &resp);

			String tmp0("{\"event\":\"pair_sensor\",\"h_id\":\"");
			String output = "{\\\"ir\\\":[";
//String output = "";

			for (uint16_t i = 1; i < resp.rawlen; i++) {
				uint32_t usecs;
				for (usecs = resp.rawbuf[i] * RAWTICK; usecs > UINT16_MAX;
						usecs -= UINT16_MAX) {
					output += uint64ToString(UINT16_MAX);
					if (i % 2)
						output += ",0,";
					else
						output += ",0,";
				}
				output += uint64ToString(usecs, 10);
				if (i < resp.rawlen - 1)
					output += ",";  // ',' not needed on the last one
				if (i % 2 == 0)
					output += "";  // Extra if it was even.
			}

			output += "]}";
			tmp0 += output;

			tmp0 += String("\"}");
			Serial.println(tmp0.c_str());
			client->println(tmp0);
			Serial.println("done");

			return true;
		}
	} else if (String(event_type) == String("deserialise")) {
		int bytes_read = 0;

		//eeprom->deserialise_config_class(bytes_read, &bytes_read);
		Serial.println("read : " + String(bytes_read));
	}
	jsonBuffer.clear();
	return false;

}
