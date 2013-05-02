#include <stdio.h>
#include <stdlib.h>
#include "error_handling.h"

int main(int argc, char** argv) {
	int numPhilosophers = 0;

	checkArgs(argc, argv, &numPhilosophers);

	return 0;
}