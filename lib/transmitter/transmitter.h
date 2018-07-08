
#ifndef transmitter_h
#define transmitter_h
#include "Arduino.h"
#include "../receiver/receiver.h"

#define trans_pin D8
#define power_trans D7 // power transistor pin
#define default_periodic_duration 260
#define default_repeats 2

class Transmitter {
public:

	Transmitter(receiver* x, int data_pin = trans_pin, int enable_pin =
	power_trans, unsigned int periodusec = default_periodic_duration,
			unsigned short repeats = default_repeats) {
		sys_recv = x;
		_data_pin = data_pin;
		_enable_pin = enable_pin;
		_periodusec = periodusec;
		_repeats = (1 << repeats) - 1; // I.e. _repeats = 2^repeats - 1
		_last_time_sent = 0;

		pinMode(_data_pin, OUTPUT);
		pinMode(_enable_pin, OUTPUT);
		digitalWrite(_enable_pin, LOW);

	}

	void sendonOff(unsigned long address, unsigned short channel,
			unsigned long sens);
	void send_all_off();
	void sendDim(unsigned long address, unsigned short unit,
			unsigned short dimLevel);
	unsigned long get_last_time() {
		return _last_time_sent;
	}
protected:
	unsigned long _address = 0;   // Address to be transmitted
	int _data_pin;    // Transmitter output pin
	unsigned int _periodusec; // Oscillator period in microseconds
	unsigned short _repeats;  // Number over repetitions of one telegram
	int _enable_pin; //transistor enablepin
	unsigned long _last_time_sent;
	receiver* sys_recv;

	void _sendStartPulse();

	void _sendAddress();

	void _sendchannel(unsigned short channel);

	void _sendStopPulse();

	void _sendBit(boolean isBitOne);

	void _enable();
	void _disable();
	void sendDim(receiver* recv, data_434_pack pack, unsigned short dimLevel);

};
#endif

