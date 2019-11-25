#include "cache.h"
#include <string.h>

void init() {
	memset(main_memory, 0, MAIN_MEMORY_SIZE);
}
 
unsigned int get_offset (unsigned int address) {
	return 0;
}

unsigned int find_set(unsigned int address) {
	return 0;
}

unsigned int select_oldest(unsigned int setnum) {
	return 0;
}

void read_tocache(unsigned int blocknum, unsigned int way, unsigned int set) {

}

unsigned char read_byte(unsigned int address) {
	return 0;
}

void write_byte(unsigned int address, unsigned char value) {

}

float get_miss_rate() {
	return 0;
}
