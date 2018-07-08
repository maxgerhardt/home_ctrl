#ifndef network_info_h
#define network_info_h
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WString.h>
#include "../debug/debug.h"

#define SMART_TIMEOUT 50000
#define SMART_RESTART 20000
#define SMART_USER_FAIL 120000
#define SMART_AUTH_FAIL 5000
#define WIFI_TIMEOUT 60000

extern int sys_status;

class network_info {
public:
	//bool _connected;bool _custom;bool _know_ssid_pw;
	/*IPAddress _my_ip;
	 IPAddress _gateway_ip;
	 IPAddress _subnet_mask;
	 IPAddress _dns_ip;*/
	String _ssid;
	String _password;
	ESP8266WiFiClass* _Wi;
	//void set_values(bool, bool, bool, IPAddress, IPAddress, IPAddress,IPAddress, const char*, const char*);

	void reset_config(ESP8266WiFiClass* Wi);
	void set_config(ESP8266WiFiClass* Wi);

	void print(HardwareSerial* Serial);bool search_network(HardwareSerial* Ser,
			ESP8266WiFiClass* Wi, String str);
	void set_name_pw(String ssid, String pw);
	bool connect(ESP8266WiFiClass* Wi);
	void disconnect(bool del);
	bool is_connected();
	String get_ssid(){
		return _ssid;
	}
	String get_pw(){
		return _password;
	}
	network_info(const char*ssid,const char*pw) {
		_Wi = &WiFi;
		_ssid =String(ssid);
		_password = String(ssid);

	}
	network_info(){
		_Wi = &WiFi;
		_ssid = "";
		_password="";
	}
	bool smartconfig();
};


#endif
