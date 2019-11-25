#include "cache.h"
#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void readfile(const char* filename);

int main(int argc, char const *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "%s\n", "Uso: ./tp <nombre_archivo>");
		return 1;
	}
	init();
	readfile(argv[1]);
	return 0;
}

static void readfile(const char* filename) {
	FILE* fp;
	fp = fopen(filename, "r");
	if (!fp) {
		fprintf(stderr, "%s\n", "No existe el archivo ingresado.");
		exit(1);
	}

	char* line = NULL;
    size_t len = 0;

	while(getline(&line, &len, fp) != -1) {
	    if (strncmp(line, "FLUSH", 5) == 0) {
	    	printf("%s\n", "Flushing.");
            init();

        } else if (strncmp(line, "R", 1) == 0) {
        	printf("%s\n", "Reading byte.");

        } else if (strncmp(line, "W", 1) == 0) {
        	printf("%s\n", "Writing byte.");

        } else if (strncmp(line, "MR", 2) == 0) {
        	printf("%s\n", "Showing miss rate.");
        	printf("Miss rate: %f\n", get_miss_rate());

        } else {
        	fprintf(stderr, "%s\n", "Undefined cmd, exiting program.");
        	exit(1);
        }
	}

	if (line)
		free(line);
	fclose(fp);
    
}