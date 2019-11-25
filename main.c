#include "cache.h"
#include <stdio.h>

int main(int argc, char const *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "%s\n", "Uso: ./tp <nombre_archivo>");
	}
	return 0;
}