#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "error_handling.h"
#include "random_gen.h"

#define THINKING 0
#define HUNGRY 1
#define EATING 2

typedef struct {
	pthread_t thread;
	int state, id;	
} Philosopher;

typedef struct {
	Philosopher** philosophers;
	int size, currentId;
} Input;

void printPhilosophersState(Philosopher** philosophers, int size) {
	int i;
	char currentState;
	for (i = 0; i < size; i++) {
		if (philosophers[i]->state == 0)
			currentState = 'T';
		else if (philosophers[i]->state == 1) 
			currentState = 'H';
		else
			currentState = 'E';

		printf("%c ", currentState);
	}
	printf("\n");
}

int leftState(Philosopher** philosophers, int size, int currentId) {	
	return philosophers[(currentId + size - 1) % size]->state;	
}

int rightState(Philosopher** philosophers, int size, int currentId) {	
	return philosophers[(currentId + 1) % size]->state;	
}

void tryToTakeForks(Philosopher** philosophers, int size, int currentId) {
	philosophers[currentId]->state = HUNGRY;
	// TODO
}

void* philosopherDoingHisThing(void* arg) {
	Input* input = (Input*)arg;
	int sleepTime;
	while(1) {
		sleepTime = random_in_range(1, 10);
		//printf("I'm philosopher (%d) and I did something... and going to sleep now for (%d) seconds\n", philosopher->id, sleepTime);
		printPhilosophersState(input->philosophers, input->size);
		tryToTakeForks(input->philosophers, input->size, input->currentId);
		sleep(sleepTime);
	}
	pthread_exit(NULL);
}

void init(Philosopher **philosophers, int amount) {
	int i;
	Input* currentInput;
	for(i = 0; i < amount; i++) {
		philosophers[i] = (Philosopher*)malloc(sizeof(Philosopher));
		philosophers[i]->state = THINKING;
		philosophers[i]->id = i;
		currentInput = (Input*)malloc(sizeof(Input));
		currentInput->philosophers = philosophers;
		currentInput->size = amount;
		currentInput->currentId = i;
		pthread_create(&(philosophers[i]->thread), NULL, philosopherDoingHisThing, (void*)currentInput);
	}	
}

int main(int argc, char** argv) {
	int numPhilosophers = 0;
	Philosopher **philosophers;

	checkArgs(argc, argv, &numPhilosophers);
	philosophers = (Philosopher**)calloc(numPhilosophers, sizeof(Philosopher*));
	init(philosophers, numPhilosophers);
	
	pthread_exit(NULL);
}