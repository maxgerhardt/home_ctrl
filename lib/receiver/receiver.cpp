#include "receiver.h"

extern receiver receiver_handler;

// END

void receiver::start_interrupt() {
	if (not init) {
		pinMode(RECV_PIN, INPUT_PULLUP);
		attachInterrupt(RECV_PIN, receiver_isr,
		CHANGE);
		init = true;
	} else {
		return;
	}

}

void receiver::stop_interrupt() {
	if (init) {
		detachInterrupt(RECV_PIN);
		init = false;
	} else {
		return;
	}
}

void ICACHE_RAM_ATTR receiver_isr(void) {
	if (!receiver_handler.init)
		return;

	//Serial.println("allowed");

	//static bool allow_interrupts = false;
	static unsigned long timestamp[3] = { 0, 0, 0 }; //Last Now Now-Last
	static bool received_pulse = false;
	static int counter = 0;
	//volatile bool flag = false;

	timestamp[0] = timestamp[1];
	timestamp[1] = micros();
	timestamp[2] = (timestamp[1] - timestamp[0]);
	while (timestamp[2] > 0x1111111)
		timestamp[2] -= 0x1111111;

	switch (received_pulse) {
	case false:
		if ((timestamp[2]
				>= (receiver_handler.t0 - receiver_handler.tolerance)
						* receiver_handler.start_pulse)
				&& ((timestamp[2]
						<= (receiver_handler.t0 + receiver_handler.tolerance)
								* receiver_handler.start_pulse))
				&& (receiver_handler.t0 |= 0) && (receiver_handler.t0 > 200)
				&& (receiver_handler.t0 <= 300)) //Check if second change is 10.44 times longer than t0 +-tolerance
				{
			counter = 0;
			received_pulse = true;
		} else {
			receiver_handler.t0 = timestamp[2];
		}

		break;
	case true:

		if (counter < receiver_handler.max_length) {
			receiver_handler.most_recent.received[counter] = timestamp[2]
					/ (receiver_handler.t0); //T might add  -tolerance

			if ((receiver_handler.most_recent.received[counter]
					>= receiver_handler.stop_pulse - 10)
					&& ((receiver_handler.most_recent.received[counter]
							<= receiver_handler.stop_pulse + 10))) //Check if received[counter ]is 40 times longer than t0 +-tolerance
					{
				receiver_handler.most_recent.received_length = counter - 1; // 1 offset because last two are stop pulse
				received_pulse = false;
				receiver_handler.most_recent.receiver_process();
				return;
			}
			// received[counter] = timestamp[2];

			counter++;
		} else {
			received_pulse = false;
		}

		break;
	}
}

void data_434_pack::receiver_process() {
	unsigned int count = 0;
	bool adress_tmp[26] = { 0 };

	bool dim_tmp[4] = { 0 };
	bool channel_tmp[4] = { 0 };
	while (count <= received_length) {
		if ((received[count] < 3) && (received[count + 1] < 3)
				&& (received[count + 2] < 3) && (received[count + 3] >= 3)) {
			received_bit[count / 4] = 0;
		}
		if ((received[count] < 3) && (received[count + 1] >= 3)
				&& (received[count + 2] < 3) && (received[count + 3] < 3)) {
			received_bit[count / 4] = 1;
		}
		count += 4;
	}
	memcpy(adress_tmp, received_bit, sizeof(adress_tmp));
	address = bitArrayToInt32(adress_tmp, 26);
	group_bit = received_bit[26];
	state_bit = received_bit[27];
	memcpy(channel_tmp, received_bit + 28, sizeof(channel_tmp));
	channel = bitArrayToInt32(channel_tmp, 4);
	if (received_length / 4 == 36) {
		memcpy(dim_tmp, received_bit + 32, sizeof(dim_tmp));
		dim = bitArrayToInt32(dim_tmp, 4);
	}

	//we'Ve received something, need to notify machine
	valid = true;
	receiver_handler.most_recent_list.push_back(*this);
	receiver_handler.got_new_sender_int = true;
	//Serial.println("done processing");

}

void receiver::print_arr() {
	if (receiver_handler.t0 > 200) {/*
	 Serial.print("Length of Array: ");
	 Serial.println(most_recent.received_length);
	 Serial.print("Time period: ");
	 Serial.print(receiver_handler.t0);
	 Serial.println("micro seconds");
	 Serial.println("Printing out Array:");
	 Serial.print("||");

	 for (int i = 0; i <= most_recent.received_length; i++) {
	 Serial.print(most_recent.received[i]);
	 Serial.print("||");
	 }
	 Serial.println();
	 Serial.println("Printing out Bit Array:");
	 Serial.print("||");

	 for (int i = 0; i < most_recent.received_length / 4; i++) {
	 Serial.print(most_recent.received_bit[i]);
	 Serial.print("||");
	 }*/

		Serial.println();
		Serial.print("Adress: ");
		Serial.print(most_recent.address);
		Serial.print(", Group_bit: ");
		Serial.print(most_recent.group_bit);
		Serial.print(", State_bit: ");
		Serial.println(most_recent.state_bit);
		Serial.print(", Channel: ");
		Serial.println(most_recent.channel);
		Serial.println();
	}
}

int bitArrayToInt32( bool arr[], int count) {
	int ret = 0;
	int tmp;
	for (int i = 0; i < count; i++) {
		tmp = arr[i];
		ret |= tmp << (count - i - 1);
	}
	return ret;
}

data_434_pack wait_for_pairing_sensor() {
	Serial.println("waiting for sensor");
	receiver_handler.got_new_sender_int = false;
	uint32_t timeout_start = millis();
	receiver_handler.start_interrupt();

	//return data_434_pack();
	int gotten_the_same = 0;
	data_434_pack compare_val;

	for (;;) {
		if ((timeout_start + user_sensor_delay) < millis()) {

			return data_434_pack();
		}

		//receiver_handler.print_arr();
		if (receiver_handler.got_new_sender_int) {
			if (compare_val == receiver_handler.most_recent) {
				gotten_the_same += 1;
				Serial.println("+1");
			} else {
				compare_val = receiver_handler.most_recent;
				Serial.println("reset");
				gotten_the_same = 0;
			}
			receiver_handler.got_new_sender_int = false;
		}
		if (gotten_the_same == repeat_sensor_times)
			return compare_val;
		delay(1);
		yield();
	}

	return data_434_pack();
}
