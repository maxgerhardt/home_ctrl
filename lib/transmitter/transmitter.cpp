#include "transmitter.h"

void Transmitter::_enable() {
	digitalWrite(_enable_pin, HIGH);

}
void Transmitter::_disable() {
	digitalWrite(_enable_pin, LOW);

}

void Transmitter::sendonOff(unsigned long address, unsigned short channel,
		unsigned long sens) {
	bool switchOn;
	if (sens == 100)
		switchOn = true;
	else if (sens == 0)
		switchOn = false;
	else if (sens > 100) {
		sendonOff(address, 0, 100);
		return;
	} else
		return; // TODO dimmer send implementation put here, and return after

	if (address == 0) {
		Serial.println("!!! the addr to be sent is 0!!! cought");
		return;
	}

	sys_recv->stop_interrupt();
	Serial.println(
			"inside sender " + String(address) + " X " + String(switchOn));
	_address = address;
	_enable();
	delay(1);
	for (int i = _repeats; i >= 0; i--) {
		_sendStartPulse();

		_sendAddress();

		// No group bit
		_sendBit(false);

		// Switch on | off
		_sendBit(switchOn);

		_sendchannel(channel);

		_sendStopPulse();
	}

	delay(5);
	_disable();

	_last_time_sent = millis();
	sys_recv->start_interrupt();
}

void Transmitter::send_all_off() {
	bool switchOn = false;
	unsigned long address = 0;
	unsigned int channel = 0;
	//sys_recv->stop_interrupt();
	_address = address;
	_enable();
	delay(10);
	for (int i = _repeats; i >= 0; i--) {
		_sendStartPulse();

		_sendAddress();

		// No group bit
		_sendBit(false);

		// Switch on | off
		_sendBit(switchOn);

		_sendchannel(channel);

		_sendStopPulse();
	}
	_disable();

	_last_time_sent = millis();
	sys_recv->start_interrupt();
}
void Transmitter::sendDim(unsigned long address, unsigned short channel,
		unsigned short dimLevel) {
	if (dimLevel == 0) {
		sendonOff(address, 0, 0);
		return;
	}

	Serial.println(
			"dimmer inside " + String(address) + " X " + String(dimLevel));
	sys_recv->stop_interrupt();

	dimLevel = map(dimLevel, 0, 100, 0, 15);
	_address = address;

	_enable();
	for (int i = _repeats; i >= 0; i--) {
		_sendStartPulse();

		_sendAddress();

		// No group bit
		_sendBit(false);

		// Switch type 'dim'
		digitalWrite(_data_pin, HIGH);
		delayMicroseconds(_periodusec);
		digitalWrite(_data_pin, LOW);
		delayMicroseconds(_periodusec);
		digitalWrite(_data_pin, HIGH);
		delayMicroseconds(_periodusec);
		digitalWrite(_data_pin, LOW);
		delayMicroseconds(_periodusec);

		_sendchannel(channel);

		for (short j = 3; j >= 0; j--) {
			_sendBit(dimLevel & 1 << j);
		}

		_sendStopPulse();

	}
	_disable();
	sys_recv->start_interrupt();
}
void Transmitter::sendDim(receiver* recv, data_434_pack pack,
		unsigned short dimLevel) {
	recv->stop_interrupt();

	dimLevel = map(dimLevel, 0, 100, 0, 15);
	_address = pack.address;

	_enable();
	for (int i = _repeats; i >= 0; i--) {
		_sendStartPulse();

		_sendAddress();

		// No group bit
		_sendBit(false);

		// Switch type 'dim'
		digitalWrite(_data_pin, HIGH);
		delayMicroseconds(_periodusec);
		digitalWrite(_data_pin, LOW);
		delayMicroseconds(_periodusec);
		digitalWrite(_data_pin, HIGH);
		delayMicroseconds(_periodusec);
		digitalWrite(_data_pin, LOW);
		delayMicroseconds(_periodusec);

		_sendchannel(pack.channel);

		for (short j = 3; j >= 0; j--) {
			_sendBit(dimLevel & 1 << j);
		}

		_sendStopPulse();

	}
	_disable();
	recv->start_interrupt();

}

void Transmitter::_sendStartPulse() {
	digitalWrite(_data_pin, HIGH);
	delayMicroseconds(_periodusec);
	digitalWrite(_data_pin, LOW);
	delayMicroseconds(_periodusec * 10 + (_periodusec >> 1)); // Actually 10.5T insteat of 10.44T. Close enough.
}

void Transmitter::_sendAddress() {
	for (short i = 25; i >= 0; i--) {
		_sendBit((_address >> i) & 1);
	}
}

void Transmitter::_sendchannel(unsigned short channel) {
	for (short i = 3; i >= 0; i--) {
		_sendBit(channel & 1 << i);
	}
}

void Transmitter::_sendStopPulse() {
	digitalWrite(_data_pin, HIGH);
	delayMicroseconds(_periodusec);
	digitalWrite(_data_pin, LOW);
	delayMicroseconds(_periodusec * 40);
}

void Transmitter::_sendBit(boolean isBitOne) {
	if (isBitOne) {
		// Send '1'
		digitalWrite(_data_pin, HIGH);
		delayMicroseconds(_periodusec);
		digitalWrite(_data_pin, LOW);
		delayMicroseconds(_periodusec * 5);
		digitalWrite(_data_pin, HIGH);
		delayMicroseconds(_periodusec);
		digitalWrite(_data_pin, LOW);
		delayMicroseconds(_periodusec);
	} else {
		// Send '0'
		digitalWrite(_data_pin, HIGH);
		delayMicroseconds(_periodusec);
		digitalWrite(_data_pin, LOW);
		delayMicroseconds(_periodusec);
		digitalWrite(_data_pin, HIGH);
		delayMicroseconds(_periodusec);
		digitalWrite(_data_pin, LOW);
		delayMicroseconds(_periodusec * 5);
	}
}

