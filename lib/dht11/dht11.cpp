#include "dht11.h"

// There is a timeout if voltage on pin does not change in 10k loop passes time

bool ack(int pin, int mode) {
	for(unsigned int loopCnt = 10000; digitalRead(pin) == mode; loopCnt--) {
		if (loopCnt <= 0) return false;
	}
	return true;
}

int dht11::read(int pin) {

	uint8_t bits[5];
	uint8_t cnt = 7;
	uint8_t idx = 0;

	for (int i = 0; i < 5; i++) {
		bits[i] = 0;
	}

	// REQUEST SAMPLE
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
	delay(18);
	digitalWrite(pin, HIGH);
	delayMicroseconds(40);
	pinMode(pin, INPUT);

	if(!ack(pin, LOW)) return DHTLIB_ERROR_TIMEOUT;
	if(!ack(pin, HIGH)) return DHTLIB_ERROR_TIMEOUT;

	// DHT sends 5 bytes ie. 40 bits as output
	for (int i = 0; i < 40; i++) {

		if(!ack(pin, LOW)) return DHTLIB_ERROR_TIMEOUT;

		unsigned long t = micros();

		if(!ack(pin, HIGH)) return DHTLIB_ERROR_TIMEOUT;

		// Below is 0, above is 1
		if((micros() - t) > 40) {
			bits[idx] |= (1 << cnt);
		}

		// If counter = 0 move to the next byte,
		// otherwise decrese counter to set next bit of requested bits

		if(cnt == 0) {
			cnt = 7;
			idx++;
		}	else {
			cnt--;
		}

	}

  // Since bits[1] and bits[3] are always zero they are omitted
	humidity    = bits[0];
	temperature = bits[2];

	uint8_t sum = bits[0] + bits[2];

	if (bits[4] != sum) return DHTLIB_ERROR_CHECKSUM;
	return DHTLIB_OK;

}
