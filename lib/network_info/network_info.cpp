#include "network_info.h"
void network_info::print(HardwareSerial* Serial) {
	Serial->println("\n printing net info start !! \n");

	/*Serial->println("\n myip: ");
	 Serial->println(_my_ip);
	 Serial->println("\n gateway ip: ");
	 Serial->println(_gateway_ip);
	 Serial->println("\n subnet mask: ");
	 Serial->println(_subnet_mask);
	 Serial->println("\n dns ip : ");
	 Serial->println(_dns_ip);*/
	Serial->println("\n ssid: ");
	Serial->println(_ssid);
	Serial->println("\n PW: ");
	Serial->println(_password);
	Serial->println("\n NETINFO end \n\n ");

}

void network_info::reset_config(ESP8266WiFiClass* Wi) {
	Wi->config(IPAddress(0, 0, 0, 0), IPAddress(0, 0, 0, 0),
			IPAddress(0, 0, 0, 0), IPAddress(0, 0, 0, 0));
}

bool network_info::search_network(HardwareSerial* Ser, ESP8266WiFiClass* Wi,
		String str) {
	Wi->mode(WIFI_STA);
	Wi->disconnect();
	int n = Wi->scanNetworks();
	Ser->println("scan done");
	if (n == 0) {
		Ser->println("no networks found");
	} else {
		Ser->println(" networks found: \n");
		for (int i = 0; i < n; ++i) {
			Ser->println(Wi->SSID(i));
		}
	}

	Ser->println("printing,done\n");
	for (int i = 0; i < n; ++i) {
		if (str == Wi->SSID(i)) {
			Wi->disconnect(true);
			return true;
		}

	}
	Wi->disconnect(true);

	return false;
}
/*
 void network_info::set_values(bool x, bool y, bool z, IPAddress mip,
 IPAddress gip, IPAddress subm, IPAddress dnsip, String ssid,String password) {
 _connected = x;
 _custom = y;
 _know_ssid_pw = z;

 _my_ip = mip;
 _gateway_ip = gip;
 _subnet_mask = subm;
 _dns_ip = IPAddress(8, 8, 8, 8);

 _ssid = ssid;
 _password = password;
 }*/
void network_info::set_name_pw(String ssid, String pass) {
	_ssid = ssid;
	_password = pass;

}

bool network_info::connect(ESP8266WiFiClass* Wi) {
	sys_status = 3;
	Wi->begin(_ssid.c_str(), _password.c_str());
	uint32_t timeout = millis();
	Serial.println("trying to connect ");
	while (Wi->status() != WL_CONNECTED) {
		if (timeout + WIFI_TIMEOUT < millis()) {
			Serial.println("ended connect process, timeout");
			Serial.println(WiFi.status());

			return false;
		}
		delay(500);
		Serial.print(".");
	}
	Serial.println("ended connect process");
	Serial.println(WiFi.status());
	return true;
}

void network_info::disconnect(bool del) {
	_Wi->disconnect(del);
}

bool network_info::is_connected() {
	uint32_t timeout = millis();
	while (timeout + 5000 < millis()) {
		if (_Wi->status() == WL_CONNECTED) {
			Serial.println("true");
			return true;
		} else {
			_Wi->disconnect(true);
			Serial.println("false");

			return false;
		}
	}
	return false;

}

bool network_info::smartconfig() {
	sys_status = 3;
	_Wi->disconnect(true);
	_Wi->mode(WIFI_STA);
	_Wi->beginSmartConfig();
	WiFi.beginSmartConfig();
	Serial.println("Waiting for SmartConfig.");
	uint32_t timeout_start = millis();
	while (_Wi->status() != WL_CONNECTED) {
	/*if (timeout_start + SMART_RESTART < millis()) {
		 Serial.println("restarting smart");
		 _Wi->stopSmartConfig();
		 delay(1000);
		 _Wi->beginSmartConfig();
		 }*/

		if (timeout_start + SMART_USER_FAIL < millis()) {
			_Wi->stopSmartConfig();
			Serial.println("timeout in smartconfig triggered");
			return false;
		}
		yield();
		Serial.print(".");
		delay(500);
	}
	Serial.println("done smartconfig");
	timeout_start = millis();

	while (_Wi->status() != WL_CONNECTED) {
		if (timeout_start + SMART_AUTH_FAIL < millis()) {

			return false;
		}

		yield();
		Serial.print("x");

	}
	_ssid = String(_Wi->SSID());
	_password = String(_Wi->psk());
	return true;
}

