#ifndef temp_hum_sens_h
#define temp_hum_sens_h

#include <Arduino.h>
#include <Wire.h>
#define sens_mod 0x40



class temp_hum_s{
private:
	int addr;

public:
	int get_temp();
	int get_hum();

	temp_hum_s(int ad){
		addr = ad;
	}
};

#endif
