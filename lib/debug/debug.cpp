#include "debug.h"
#include "WString.h"

void serialsetup(HardwareSerial* Serial ){
  Serial->begin(SERIAL_BAUD, SERIAL_8N1, SERIAL_TX_ONLY);
   while (!Serial);
  Serial->setDebugOutput(true);
  Serial->print(ESP.getSdkVersion());
  Serial->println("\n");
}

/*void print_WL_info(ESP8266WiFiClass* w) {
  switch (w->status()) {
    case 255:
      Serial.println("WL_NO_SHIELD");
      break;
    case 0:
      Serial.println("WL_IDLE_STATUS");
      break;
    case 1:
      Serial.println("WL_NO_SSID_AVAIL");
      break;
    case 2:
      Serial.println("WL_SCAN_COMPLETED");
      break;
    case 3:
      Serial.println("WL_CONNECTED");
      break;
    case 4:
      Serial.println("WL_CONNECT_FAILED");
      break;
    case 5:
      Serial.println("WL_CONNECTION_LOST");
      break;
    case 6:
      Serial.println("WL_DISCONNECTED");
      break;
    default:
      Serial.println("unknown wifi status!!");

  }
}*/

void assert_error(HardwareSerial* Serial, char* error) {
  Serial->println("Error asserted: \n\n");
  Serial->print(*error);
  Serial->println("\n\n");
  memcpy(error_reason,error, ERROR_CODE_LENGTH);
}

void setup_debug(char** err){
	*err = (char*)malloc(ERROR_CODE_LENGTH*sizeof(char));

}
void full_stop(){
	uint32_t reboot=millis();
	Serial.println("restarting ESP");
	for(;;){
		delay(500);
		yield();
		if(reboot + 3000 < millis())
			ESP.restart();
	}
}
