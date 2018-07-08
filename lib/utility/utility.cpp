#include "utility.h"



uint8_t make_u8(char upper, char lower) {
	uint8_t ret;
	ret = char2num(upper) * 16 + char2num(lower);
	return ret;

}

int char2num(char in) {
	switch (in) {
	case '0':
		return 0;
	case '1':
		return 1;
	case '2':
		return 2;
	case '3':
		return 3;
	case '4':
		return 4;
	case '5':
		return 5;
	case '6':
		return 6;
	case '7':
		return 7;
	case '8':
		return 8;
	case '9':
		return 9;
	case 'A':
		return 10;
	case 'B':
		return 11;
	case 'C':
		return 12;
	case 'D':
		return 13;
	case 'E':
		return 14;
	case 'F':
		return 15;
	default:
		Serial.println("INVALID CHAR IN CONVERSION");
		return 0;
	}

}
