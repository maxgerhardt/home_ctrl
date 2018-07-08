#include "infra.h"

decode_results pair_infra(IRrecv*irrecv) {
	decode_results res;
	decode_results buffer;
	unsigned long time = millis();
	int repeats = 0;

	while (true) {
		yield();
		delay(1);
		if (irrecv->decode(&res)) {
			Serial.print(resultToHumanReadableBasic(&res));
			if (buffer.value == res.value and buffer.address == res.address
					and not buffer.decode_type <= 0) {
				repeats++;
			} else {
				buffer = res;
				repeats = 0;
			}

			if (repeats == infra_repeats)
				return res;
			if ((millis() - time) > ir_timeout) {
				Serial.println("triggered infra timeout");
				decode_results tmp = decode_results();
				tmp.decode_type = UNKNOWN;
				return tmp;

			}

		}
	}
}

bool send_infra(IRsend* sender, std::list<uint16_t> l) {

	if (l.empty())
		return false;
	//Serial.println("building cmd");
	uint16_t tmp[l.size()];

	std::copy(l.begin(), l.end(), tmp);

	/*for (int i = 1; i < l.size(); i++)
		Serial.print(uint64ToString(tmp[i]) + String(" "));*/

sender->sendRaw(tmp,l.size(),38);

	Serial.println("ir sent.");
	return true;
}
