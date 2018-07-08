#include "web_utils.h"

void get_SQL_res(String set, String t_id) {
	const char* host = "intertechno-smarthome-api.esy.es";

	WiFiClient client;
	const int httpPort = 80;
	if (!client.connect(host, httpPort)) {
		Serial.println("connection failed");
		return;
	}

	// We now create a URI for the request
	String url = "/api/v1/config/" + set + "/" + t_id;

	Serial.print("Requesting dataset: ");
	Serial.println(url);

	// This will send the request to the server
	client.print(
			String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n"
					+ "Connection: close\r\n\r\n");
	uint32_t timeout = millis();
	while (client.available() == 0) {
		if (millis() - timeout > 5000) {
			Serial.println(">>> Client Timeout !");
			client.stop();
			return;
		}
	}

	// Read all the lines of the reply from server and print them to Serial
	while (client.available()) {
		String line = client.readStringUntil('\r');
		if (line.c_str()[0] == 0xA and line.c_str()[1] == 0x20) {
			/*Serial.print("\ndata set");
			 Serial.print("len: " + line.length());
			 Serial.println();*/
			for (int i = 2; i < line.length(); i++) {
				Serial.print(" ");
				Serial.print(line.c_str()[i], HEX);
				Serial.print(" ");
			}
			Serial.println("\n my data set:");
			Serial.print("uint8_t test_memory[]={");
			for (int i = 2; i < line.length(); i += 2) {
				Serial.print("0x");
				Serial.print(make_u8(line.c_str()[i], line.c_str()[i + 1]),
				HEX);
				if (i >= line.length() - 2) {

				} else
					Serial.print(",");
			}
			Serial.print("};");
		} else {
		}

	}
	Serial.println("\nrequests done");
	client.stop();
}
