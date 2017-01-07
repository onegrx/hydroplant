#include "dht11.h"

// There is a timeout if voltage on pin does not change in 10k loop passes time
bool acknowledge(int pin, int mode) {
	for(unsigned int loopCnt = 10000; digitalRead(pin) == mode; loopCnt--) {
		if (loopCnt <= 0) return false;
	}
	return true;
}

int dht11::read(int pin) {

	uint8_t buffer[5];
	uint8_t bit_shift = 7;
	uint8_t current_byte = 0;

	for (int i = 0; i < 5; i++) {
		buffer[i] = 0;
	}

	// Handshake
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
	delay(18);
	digitalWrite(pin, HIGH);
	delayMicroseconds(40);
	pinMode(pin, INPUT);

	if(!acknowledge(pin, LOW)) return DHTLIB_ERROR_TIMEOUT;
	if(!acknowledge(pin, HIGH)) return DHTLIB_ERROR_TIMEOUT;

	// DHT sends 5 bytes ie. 40 bits as output
	for (int i = 0; i < 40; i++) {

		if(!acknowledge(pin, LOW)) return DHTLIB_ERROR_TIMEOUT;

		unsigned long t = micros();

		if(!acknowledge(pin, HIGH)) return DHTLIB_ERROR_TIMEOUT;

		// Below is 0, above is 1
		if((micros() - t) > 40) {
			buffer[current_byte] |= (1 << bit_shift);
		}

		// If counter = 0 move to the next byte,
		// otherwise decrese counter to set next bit of requested bits

		if(bit_shift == 0) {
			bit_shift = 7;
			current_byte++;
		}	else {
			bit_shift--;
		}

	}

  // Since bits[1] and bits[3] are always zero they are omitted
	humidity    = buffer[0];
	temperature = buffer[2];

	uint8_t sum = buffer[0] + buffer[2];

	if (buffer[4] != sum) return DHTLIB_ERROR_CHECKSUM;
	return DHTLIB_OK;

}
