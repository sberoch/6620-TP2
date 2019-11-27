#include "cache.h"
#include <string.h>

#define MAX_COUNTER_VALUE 65535

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
	uint16_t min = MAX_COUNTER_VALUE;
	unsigned int way;
	for(unsigned i = 0; i < CACHE_WAYS; ++i) {
		uint16_t fifo_counter = cache.blocks[setnum][i].counter;
		if (fifo_counter < min) {
			min = fifo_counter;
			way = i;
		}
	}
	return way;
}

void read_tocache(unsigned int blocknum, unsigned int way, unsigned int set) {
	unsigned char* mem_pos = &main_memory.main_mem[blocknum*CACHE_BLOCK_SIZE];
	memcpy(cache.blocks[set][way].data, mem_pos, CACHE_BLOCK_SIZE);

	cache.blocks[set][way].valid = true;
	cache.blocks[set][way].dirty = false;
	//TODO: tag
	//TODO: fifo counter
}

unsigned char read_byte(unsigned int address) {
	return 0;
}

void write_byte(unsigned int address, unsigned char value) {

}

float get_miss_rate() {
	return 0;
}
