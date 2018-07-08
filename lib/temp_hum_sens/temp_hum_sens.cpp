#include "temp_hum_sens.h"

int temp_hum_s::get_temp() {
	unsigned int data[2];

	Wire.beginTransmission(addr);

	Wire.write(0xE0);

	Wire.endTransmission();
	delay(20);

	Wire.requestFrom(addr, 2);


	if (Wire.available() == 2) {
		data[0] = Wire.read();
		data[1] = Wire.read();
	}else{
		data[1]=100000;
		data[0]=100000;
	}

	float temp = ((data[0] * 256.0) + data[1]);
	float ctemp = ((175.72 * temp) / 65536.0) - 46.85;
	return (int) ctemp;

}

int temp_hum_s::get_hum() {
	unsigned int data[2];

	Wire.beginTransmission(addr);
	Wire.write(0xF5);
	Wire.endTransmission();
	delay(20);

	Wire.requestFrom(addr, 2);

	if (Wire.available() == 2) {
		data[0] = Wire.read();
		data[1] = Wire.read();
	}else{
		data[1]=100000;
		data[0]=100000;
	}

	float humidity = ((data[0] * 256.0) + data[1]);
	humidity = ((125 * humidity) / 65536.0) - 6;

	return (int) humidity;
}
