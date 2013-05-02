#include <stdio.h>
#include <stdlib.h>
#include "error_handling.h"

void checkArgs(int argSize, char** argv, int* numPhilosophers) {
	if (argSize != 2) {
		printf("Você deve informar a quantidade de filósofos.\n");
		exit(EXIT_FAILURE);
	}

	sscanf(argv[1], "%d", numPhilosophers);
	if ((*numPhilosophers) < 1) {
		printf("Deve haver mais do que 1 filósofo.\n");
		exit(EXIT_FAILURE);
	}
}