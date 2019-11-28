#include "cache.h"
#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int readfile(const char* filename);
static int read(char* line);
static int write(char* line);

int main(int argc, char const *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "%s\n", "Uso: ./tp <nombre_archivo>");
		return 1;
	}
	init();
	int ret = readfile(argv[1]);
	return ret;
}

static int readfile(const char* filename) {
	FILE* fp;
	fp = fopen(filename, "r");
	if (!fp) {
		fprintf(stderr, "%s\n", "No existe el archivo ingresado.");
		exit(1);
	}

	char* line = NULL;
    size_t len = 0;

	while(getline(&line, &len, fp) != -1) {
        printf("Command: %s", line);
	    if (strncmp(line, "FLUSH", 5) == 0) {
	    	printf("%s\n", "Flushing.");
            init();

        } else if (strncmp(line, "R", 1) == 0) {
        	int ret = read(line);
        	if (ret == 1) return ret;

        } else if (strncmp(line, "W", 1) == 0) {
        	int ret = write(line);
        	if (ret == 1) return ret;

        } else if (strncmp(line, "MR", 2) == 0) { 
        	printf("Miss rate: %f\n", get_miss_rate());

        } else {
        	fprintf(stderr, "%s\n", "Undefined cmd, exiting program.");
        	return 1;
        }
        printf("\n");
	}

	if (line)
		free(line);
	fclose(fp);
	return 0;
}

static int read(char* line) {
	unsigned int addr;
    int ret = sscanf(line, "R %u", &addr);
    if (ret != 1) {
    	fprintf(stderr, "%s\n", "Wrong argument count.");
    	return 1;
    } else if (addr >= MAIN_MEMORY_SIZE) {
    	fprintf(stderr, "%s\n", "err: addr > main_mem_size");
    	return 1;
    }
    unsigned char result = read_byte(addr);
    printf("Read value: %u\n", result);
	return 0; 
}

static int write(char* line) {
	unsigned int addr, val_tmp;
    int ret = sscanf(line, "W %u, %u", &addr, &val_tmp);
    if (ret != 2) {
        fprintf(stderr, "%s\n", "Wrong argument count.");
        return 1;
    } else if (addr >= MAIN_MEMORY_SIZE) {
        fprintf(stderr, "%s\n", "err: addr > main_mem_size");
        return 1;
    }
    unsigned char val = val_tmp;
    write_byte(addr, val);
	return 0;
}

