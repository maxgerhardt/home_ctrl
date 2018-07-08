#include <eeprom2.h>
//volatile static bool wire_init_done = false;

#ifndef disk1
void fs_eeprom::_writeEEPROM(int deviceaddress, uint32_t eeaddress, byte data )
{
	if(!wire_init_done)
	eepromsetup();
	Wire.beginTransmission(deviceaddress);
	Wire.write((int)(eeaddress >> 8));   // MSB
	Wire.write((int)(eeaddress & 0xFF));// LSB
	Wire.write(data);
	Wire.endTransmission();

	delay(5);//<- VERY IMPORTANT FOR THE EEPROM
}

byte fs_eeprom::_readEEPROM(int deviceaddress, uint32_t eeaddress )
{
	if(!wire_init_done)
	eepromsetup();

	byte rdata = 0xFF;

	Wire.beginTransmission(deviceaddress);
	Wire.write((int)(eeaddress >> 8));   // MSB
	Wire.write((int)(eeaddress & 0xFF));// LSB
	Wire.endTransmission();

	Wire.requestFrom(deviceaddress,1);

	if (Wire.available()) rdata = Wire.read();

	return rdata;

}
#endif
#ifdef disk1
void /*fs_eeprom::*/_writeEEPROM(uint32_t eeaddress, byte data) {

	int deviceaddress = disk1;
	Wire.beginTransmission(deviceaddress);
	Wire.write((int) (eeaddress >> 8));   // MSB
	Wire.write((int) (eeaddress & 0xFF)); // LSB
	Wire.write(data);
	Wire.endTransmission();

	delay(10);  //<- VERY IMPORTANT FOR THE EEPROM
}

byte /*fs_eeprom::*/_readEEPROM(uint32_t eeaddress) {

	int deviceaddress = disk1;

	byte rdata = 0xFF;

	Wire.beginTransmission(deviceaddress);
	Wire.write((int) (eeaddress >> 8));   // MSB
	Wire.write((int) (eeaddress & 0xFF)); // LSB
	Wire.endTransmission();

	Wire.requestFrom(deviceaddress, 1, 1);
	if (Wire.available())
		rdata = Wire.read();
	delay(10);

	return rdata;

}
#endif

bool fs_eeprom::write_net_config(HardwareSerial * ser, network_info * net) {
	// 2 byte adresse pro start
	//net->save_current_net();

	int pw_len = net->_password.length() + 1;
	char tmp_pw[pw_len];
	net->_password.toCharArray(tmp_pw, pw_len);
	//for (int i = 0; i < pw_len; i++)
	///Serial.println(tmp_pw[i]);

	int ssid_len = net->_ssid.length() + 1;
	char tmp_ssid[ssid_len];
	net->_ssid.toCharArray(tmp_ssid, ssid_len);
	//for (int i = 0; i < ssid_len; i++)
	// Serial.println(tmp_ssid[1]);

	//Serial.println("STOP !!!");

	_internal_eeprom->begin(internal_eeprom_size);
	uint16_t pw_max_addr = pw_len;
	uint8_t pw_max_addr8[2];

	pw_max_addr8[0] = (pw_max_addr & 0xff);
	pw_max_addr8[1] = (pw_max_addr >> 8);

	_internal_eeprom->write(0x01, pw_max_addr8[0]);
	_internal_eeprom->write(0x02, pw_max_addr8[1]);
	_internal_eeprom->commit();

	uint16_t ssid_max_addr = ssid_len;
	uint8_t ssid_max_addr8[2];
	ssid_max_addr8[0] = (ssid_max_addr & 0xff);
	ssid_max_addr8[1] = (ssid_max_addr >> 8);

	_internal_eeprom->write(0x03, ssid_max_addr8[0]);
	_internal_eeprom->write(0x04, ssid_max_addr8[1]);

	for (int i = 0; i < pw_len; i++) {
		_internal_eeprom->write(0x05 + i, tmp_pw[i]);

	}
	for (int i = 0; i < ssid_len; i++) {
		_internal_eeprom->write((0x05 + pw_len + i), tmp_ssid[i]);

	}

	_internal_eeprom->write(0x00, VALID_CHAR);
	_internal_eeprom->commit();

	return true;
}

bool fs_eeprom::read_net_config(HardwareSerial * ser, network_info * net) {
	_internal_eeprom->begin(internal_eeprom_size);

	if (EEPROM.read(0) == VALID_CHAR) {
		Serial.println("valids");
		uint16_t wdp;
		wdp = ((uint16_t) EEPROM.read(2) + 8) | EEPROM.read(1);
		char _tmp_arr_pw[(int) wdp];
		//Serial.println(wdp);

		for (int i = 0; i <= (int) wdp; i++) { //TODO delete this if json doesn'T parse on the way out
			_tmp_arr_pw[i] = EEPROM.read(5 + i);
		}
		net->_password = String(_tmp_arr_pw);

		uint16_t wd = ((uint16_t) EEPROM.read(4) + 8) | EEPROM.read(3);
		char _tmp_arr_ssid[(int) wd];

		for (int i = 0; i <= (int) wd; i++) {
			_tmp_arr_ssid[i] = EEPROM.read((int) wdp + i + 5);
		}
		//Serial.println(wd);

		net->_ssid = String(_tmp_arr_ssid);
		//Serial.println("ssid found : ");
//Serial.println(_tmp_arr_ssid);
		return true;

	} else {
		Serial.println("invalids");
		net->_ssid = String("");

		net->_password = String("");
		return false;
	}
}
bool fs_eeprom::remove_net_config() {
	_internal_eeprom->begin(internal_eeprom_size);
	_internal_eeprom->write(0x00, INVALID_CHAR);
	_internal_eeprom->commit();

	return true;
}
bool fs_eeprom::remove_String_config() {
	_writeEEPROM(0, INVALID_CHAR);
	Serial.println(_readEEPROM(0),HEX);
	return true;
}
