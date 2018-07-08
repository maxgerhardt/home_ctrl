#ifndef receiver_h
#define receiver_h
//************************NEEDS
#include <Arduino.h>
#include <list>
//************************DEFINES
/*not needed anymore
 *#define start_pulse 10.44 // length of start pulse in micro secondes
 *#define stop_pulse 40     // length of stop pulse in micro secondes
 *#define max_timer 17895697
 */
//Typical values

#define default_tolerance 10 //shouldnt not be higher than 3 because of erro
#define default_start_pulse 10.44
#define default_stop_pulse 40
#define default_max_length 140
#define default_max_timer 17895697
#define default_t0 0

#define user_sensor_delay 15000
#define repeat_sensor_times 3

//needed defines
#define RECV_PIN D1 // D1
#define MAX_LENGTH 140 // 32*4 == max number of changes + tolerance

//bool received_bit[max_length / 4] = { 0 };
//int t0 = 0; // t0 == length of one period

//bool got_new_sender_int = false;

//************************MAKES

class data_434_pack {
public:
	unsigned int received_length;
	unsigned int received[MAX_LENGTH];bool received_bit[MAX_LENGTH / 4];bool group_bit =
			0;bool state_bit = 0;
	int address = 0;
	int dim = 0;
	int channel = 0;bool valid = false;
	data_434_pack() {
		received_length = 0;
		memset(received, 0, MAX_LENGTH);
		memset(received_bit, false, MAX_LENGTH / 4);
	}
	void print_all() {
		Serial.println(received_length);
		for (int i = 0; i < MAX_LENGTH; i++)
			Serial.print(received[i]);
		Serial.println();
		for (int i = 0; i < MAX_LENGTH / 4; i++)
			Serial.print(received_bit[i]);
		Serial.println();
		Serial.println(group_bit);
		Serial.println(state_bit);
		Serial.println(address);
		Serial.println(dim);
		Serial.println(channel);

	}
	bool operator ==(data_434_pack comp) {
		if (address == comp.address and channel == comp.channel
				and group_bit == comp.group_bit and state_bit == state_bit)
			return true;
		else
			return false;
	}

	void receiver_process();
};

class receiver {
public:
	float start_pulse;
	float stop_pulse;
	int tolerance;
	int recv_pin;
	int max_length;
	int max_timer;bool got_new_sender_int;
	//volatile bool allow;
	int t0 = default_t0;
	data_434_pack most_recent;bool init = false;
	std::list<data_434_pack> most_recent_list;

	receiver(int _tolerance = default_tolerance) {
		start_pulse = default_start_pulse;
		stop_pulse = default_stop_pulse;
		tolerance = _tolerance;
		recv_pin = RECV_PIN;
		max_length = default_max_length;
		max_timer = default_max_timer;
		got_new_sender_int = false;
		//allow = false;
		data_434_pack most_recent();

	}
	//void receiver_setup();
	void start_interrupt();
	void stop_interrupt();
	void print_arr();
	//void detach();
	data_434_pack get_most_recent() {
		return most_recent;
	}
	void print_all() {
		Serial.println("=========");
		Serial.println(start_pulse);
		Serial.println(stop_pulse);
		Serial.println(tolerance);
		Serial.println(recv_pin);
		Serial.println(max_length);
		Serial.println(max_timer);
		Serial.println(default_t0);
		got_new_sender_int ?
				Serial.println("new int") : Serial.println("no new int");
		//allow ? Serial.println("allowed") : Serial.println("not allowed");
		Serial.println("%%%%");

		most_recent.print_all();
		Serial.println("%%%%");

		Serial.println("=========");

	}

};
/* #define default_start_pulse 10.44
 * #define default stop_pulse
 * #define default_max_length 140
 * #define max_timer 17895697
 * #define default_t0
 *
 */

//****** PROTOTYPES
int bitArrayToInt32(bool arr[], int count);
void ICACHE_RAM_ATTR receiver_isr();
data_434_pack wait_for_pairing_sensor();
//******** NEEDED FOR FUNCTION

extern receiver receiver_handler;
extern data_434_pack most_recent;

#endif
