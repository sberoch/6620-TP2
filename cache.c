#include "cache.h"
#include <string.h>
#include <stdio.h>

main_mem_t main_memory;
cache_t cache;

void init() {
	//Init main_mem
	memset(main_memory.main_mem, 0, MAIN_MEMORY_SIZE);

	//Init cache
	memset(cache.blocks, 0, sizeof(cache.blocks));
	cache.misses = 0;
	cache.accesses = 0;
	cache.globalCounter = 0;
}
 
unsigned int get_offset (unsigned int address) {
	return address & OFFSET_MASK;
}

unsigned int find_set(unsigned int address) {
	return (address & INDEX_MASK) >> OFFSET_BITS;
}

unsigned int select_oldest(unsigned int setnum) {
	uint16_t min = MAX_COUNTER_VALUE;
	unsigned int way = 0;

	for(unsigned i = 0; i < CACHE_WAYS; ++i) {
		uint16_t fifo_counter = cache.blocks[setnum][i].counter;
		if (fifo_counter < min) {
			min = fifo_counter;
			way = i;
		}
	}
	return way;
}

void write_tomem (unsigned int way, unsigned int set){
	unsigned int tag = cache.blocks[set][way].tag;
	unsigned int addr = (tag << (INDEX_BITS + OFFSET_BITS)) + (set << OFFSET_BITS);
	memcpy(&main_memory.main_mem[addr], cache.blocks[set][way].data, CACHE_BLOCK_SIZE);
}

void read_tocache(unsigned int blocknum, unsigned int way, unsigned int set) {
	unsigned char* mem_pos = &main_memory.main_mem[blocknum*CACHE_BLOCK_SIZE];
	unsigned int tag = (blocknum*CACHE_BLOCK_SIZE >> OFFSET_BITS)>> INDEX_BITS;
	
	if(cache.blocks[set][way].valid && cache.blocks[set][way].dirty){
		write_tomem(way, set);
	}
	memcpy(cache.blocks[set][way].data, mem_pos, CACHE_BLOCK_SIZE);

	cache.globalCounter ++;
	cache.blocks[set][way].valid = true;
	cache.blocks[set][way].dirty = false;
	cache.blocks[set][way].tag = tag;
	cache.blocks[set][way].counter = cache.globalCounter;
}

unsigned char read_byte(unsigned int address) {
	unsigned int offset = get_offset(address);
	unsigned int set = find_set(address);
	unsigned int tag = ((address & TAG_MASK) >> OFFSET_BITS)>> INDEX_BITS;
	bool found = false;
	unsigned char resultado;
	unsigned int way;

	for(unsigned i = 0; i < CACHE_WAYS; ++i){
		if(cache.blocks[set][i].valid && cache.blocks[set][i].tag == tag){
			way = i;
			found = true;
		}
	}
	if(!found){
		way = select_oldest(set);
		unsigned int blocknum = address >> OFFSET_BITS;
		read_tocache(blocknum, way, set);
		cache.misses ++;
		printf("%s\n", "Miss R");
	} else {
		printf("%s\n", "Hit R");
	}
	resultado = cache.blocks[set][way].data[offset]; 
	cache.accesses ++;

	return resultado;
}

void write_byte(unsigned int address, unsigned char value) {
	unsigned int offset = get_offset(address);
	unsigned int set = find_set(address);
	unsigned int way;
	unsigned int tag = ((address & TAG_MASK) >> OFFSET_BITS) >> INDEX_BITS;
	bool found = false;

	for(unsigned i = 0; i < CACHE_WAYS; ++i){
		if(cache.blocks[set][i].valid && cache.blocks[set][i].tag == tag){
			found = true;
			way = i;
		}
	}
	if(!found){
		way = select_oldest(set);
		if(cache.blocks[set][way].valid && cache.blocks[set][way].dirty){
			write_tomem(way, set);
		}
		unsigned int blocknum = address >> OFFSET_BITS;
		read_tocache(blocknum, way, set);
		cache.misses ++;
		printf("%s\n", "Miss W");

	} else {
		printf("%s\n", "Hit W");
	}

	cache.blocks[set][way].data[offset] = value;
	cache.blocks[set][way].dirty = true;
	cache.accesses ++;
}

float get_miss_rate() {
	return (float) cache.misses / (float) cache.accesses;
}
