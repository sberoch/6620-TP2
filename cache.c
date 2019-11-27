
#include "cache.h"
#include <string.h>

main_mem_t main_memory;

cache_t cache;

void init() {
	memset(main_memory.main_mem, 0, MAIN_MEMORY_SIZE);
	memset(cache.blocks, 0, sizeof(cache.blocks));
	cache.misses = 0;
	cache.accesses = 0;

	for(unsigned i = 0; i < CACHE_SETS; ++i){
		for(unsigned j = 0; j < CACHE_WAYS; ++j) {
			cache.blocks[i][j].valid = false;
		}
	}

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

void write_tomem (unsigned int blocknum, unsigned int way, unsigned int set){
	unsigned char* mem_pos = &main_memory.main_mem[blocknum*CACHE_BLOCK_SIZE];
	memcpy(mem_pos, cache.blocks[set][way].data, CACHE_BLOCK_SIZE);
} 

void read_tocache(unsigned int blocknum, unsigned int way, unsigned int set) {
	unsigned char* mem_pos = &main_memory.main_mem[blocknum*CACHE_BLOCK_SIZE];
	unsigned int tag = ((mem_pos & TAG_MASK) >> OFFSET_BITS)>> INDEX_BITS;
	
	if(cache.blocks[set][way].valid && cache.blocks[set][way].dirty){
		write_tomem(blocknum, way, set);
	}
	memcpy(cache.blocks[set][way].data, mem_pos, CACHE_BLOCK_SIZE);
	
	cache.blocks[set][way].valid = true;
	cache.blocks[set][way].dirty = false;
	cache.blocks[set][way].tag = tag;
	cache.blocks[set][way].counter = 65535;
}

unsigned char read_byte(unsigned int address) {
	unsigned int offset = get_offset(address);
	unsigned int set = find_set(address);
	unsigned int tag = ((address & TAG_MASK) >> OFFSET_BITS)>> INDEX_BITS;
	bool encontrado = false;
	unsigned char resultado;
	unsigned int way;

	for(unsigned i = 0; i < CACHE_WAYS; ++i){
		if(cache.blocks[set][i].valid && cache.blocks[set][i].tag == tag){
			encontrado = true;
		}
	}
	if(!encontrado){
		way = select_oldest(set);
		unsigned int blocknum = (address & MEMORY_INDEX_MASK) >> OFFSET_BITS;
		read_tocache(blocknum, way, set);
		cache.misses ++;
	}
	resultado = cache.blocks[set][i].data[offset]; 
	cache.accesses ++;

	return resultado;
}

void write_byte(unsigned int address, unsigned char value) {

	//Busco a ver si esta en cache
	//si no esta
		//selecciono mas viejo
		//veo si valid && dirty
			//copio a memoria
		//traigo bloque a cache
	//escribo cache
	//set dirty

	unsigned int offset = get_offset(address);
	unsigned int set = find_set(address);
	unsigned int way;
	unsigned int tag = ((address & TAG_MASK) >> OFFSET_BITS)>> INDEX_BITS;
	bool encontrado = false;

	for(unsigned i = 0; i < CACHE_WAYS; ++i){
		if(cache.blocks[set][i].valid && cache.blocks[set][i].tag == tag){
			encontrado = true;
			way = i;
		}
	}
	if(!encontrado){
		way = select_oldest(set);
		if(cache.blocks[set][way].valid && cache.blocks[set][way].dirty){
			unsigned int blocknum = (address & MEMORY_INDEX_MASK) >> OFFSET_BITS;
			write_tomem(blocknum, way, set);
		}
		read_tocache(blocknum, way, set);
		cache.misses ++;
	}
	cache.blocks[set][way].data[offset] = value;
	cache.blocks[set][way].dirty = true;
	cache.accesses ++;
}

float get_miss_rate() {
	return cache.misses/cache.accesses;
}
