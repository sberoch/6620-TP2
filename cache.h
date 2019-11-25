#ifndef CACHE_H
#define CACHE_H

#define MAIN_MEMORY_SIZE 65536

unsigned char main_memory[MAIN_MEMORY_SIZE];

void init();
unsigned int get_offset (unsigned int address);
unsigned int find_set(unsigned int address);
unsigned int select_oldest(unsigned int setnum);
void read_tocache(unsigned int blocknum, unsigned int way, unsigned int set);
unsigned char read_byte(unsigned int address);
void write_byte(unsigned int address, unsigned char value);
float get_miss_rate();

#endif // CACHE_H
