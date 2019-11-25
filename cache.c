#include "cache.h"
#include <string.h>

main_mem_t main_memory;

cache_t cache;

void init() {
	memset(main_memory.main_mem, 0, MAIN_MEMORY_SIZE);
	memset(cache.blocks, 0, sizeof(cache.blocks));
	cache.misses = 0;
	cache.accesses = 0;
}
 
unsigned int get_offset (unsigned int address) {
	return address & OFFSET_MASK;
}

unsigned int find_set(unsigned int address) {
	return (address & INDEX_MASK) >> OFFSET_BITS;
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
