
#ifndef CACHE_H
#define CACHE_H

#define _GNU_SOURCE 

#include <stdint.h>
#include <stdbool.h>

/*
*	CACHE
*	-Cache Size: 16kB
*	-Block size: 64B
*	-8WSA => 32 sets.
*	-FIFO
*	-WB/WA
*	-Adresses: 16 bits.
*
*	-Offset: 6 bits (BS = 64B).
*	-Index: 5 bits (32 sets).
*	-Tag: 5 bits.
*
*	-Metadata: Valid, Dirty, Tag, FIFO Counter.
*/

#define MAIN_MEMORY_SIZE 65536
#define CACHE_SIZE 16384
#define CACHE_WAYS 8
#define CACHE_SETS 32
#define CACHE_BLOCK_SIZE 64

#define OFFSET_BITS 6
#define INDEX_BITS 5
#define TAG_BITS 5

#define OFFSET_MASK 0x003f
#define INDEX_MASK 0x07c0
#define TAG_MASK 0xf800
#define MEMORY_INDEX_MASK 0xf7c0

#define MAX_COUNTER_VALUE 65535

typedef struct {
	unsigned char main_mem[MAIN_MEMORY_SIZE];
} main_mem_t;

typedef struct {
	bool valid;
	bool dirty;
	unsigned char tag;
	uint16_t counter;
	unsigned char data[CACHE_BLOCK_SIZE];
} block_t;

typedef struct {
	block_t blocks[CACHE_SETS][CACHE_WAYS];
	uint32_t accesses;
	uint32_t misses;
	uint16_t globalCounter;
} cache_t;

void init();
unsigned int get_offset (unsigned int address);
unsigned int find_set(unsigned int address);
unsigned int select_oldest(unsigned int setnum);
void read_tocache(unsigned int blocknum, unsigned int way, unsigned int set);
unsigned char read_byte(unsigned int address);
void write_byte(unsigned int address, unsigned char value);
void write_tomem (unsigned int blocknum, unsigned int way, unsigned int set);
float get_miss_rate();

#endif // CACHE_H
