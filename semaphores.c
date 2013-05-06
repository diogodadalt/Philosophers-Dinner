#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include "error_handling.h"
#include "random_gen.h"

#define TRUE 1
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

sem_t mutex, eating_mutex;

void printPhilosophersState(Input* input) {
	int i;
	char currentState;
	for (i = 0; i < input->size; i++) {
		if (input->philosophers[i]->state == THINKING)
			currentState = 'T';
		else if (input->philosophers[i]->state == HUNGRY) 
			currentState = 'H';
		else
			currentState = 'E';

		printf("%c ", currentState);
	}
	printf("\n", input->currentId);	
}

void sleepRand() {
	int sleepTime = random_in_range(1, 10) * 100000;
	usleep(sleepTime);
}

int changeState(Input* input, int newState) {
	int currentState = input->philosophers[input->currentId]->state;

	if (currentState != newState) {		
		if (currentState == THINKING && newState == HUNGRY) {			
			sem_wait(&mutex);
			input->philosophers[input->currentId]->state = newState;
			printPhilosophersState(input);
			sem_post(&mutex);
		}	else if (currentState == HUNGRY && newState == EATING) {			
			sem_wait(&eating_mutex);
			sem_wait(&mutex);

			if (input->philosophers[left(input)]->state != EATING && input->philosophers[right(input)]->state != EATING) {
				input->philosophers[input->currentId]->state = newState;			
				printPhilosophersState(input);
			} else
				sem_post(&eating_mutex);

			sem_post(&mutex);
		} else if (currentState == EATING && newState == THINKING) {			
			sem_wait(&mutex);
			input->philosophers[input->currentId]->state = newState;
			printPhilosophersState(input);
			sem_post(&mutex);
			sem_post(&eating_mutex);			
		}
	}	
}

int left(Input* input) {	
	return (input->currentId + input->size - 1) % input->size;	
}

int right(Input* input) {	
	return (input->currentId + 1) % input->size;	
}

void* philosopherDoingHisThing(void* arg) {
	Input* input = (Input*)arg;
	int sleepTime;
	while(TRUE) {
		sleepRand();
		changeState(input, HUNGRY);
		changeState(input, EATING);
		sleepRand();
		changeState(input, THINKING);
	}
}

void waitAll(Philosopher **philosophers, int amount) {
	int i;
	for(i = 0; i < amount; i++) {
		pthread_join(philosophers[i]->thread, NULL);
	}
}

void startAll(Philosopher **philosophers, int amount) {
	int i;
	Input* currentInput;
	for(i = 0; i < amount; i++) {
		currentInput = (Input*)malloc(sizeof(Input));
		currentInput->philosophers = philosophers;
		currentInput->size = amount;
		currentInput->currentId = i;
		pthread_create(&(philosophers[i]->thread), NULL, philosopherDoingHisThing, (void*)currentInput);
	}
}

void init(Philosopher **philosophers, int amount) {
	int i;	
	for(i = 0; i < amount; i++) {
		philosophers[i] = (Philosopher*)malloc(sizeof(Philosopher));
		philosophers[i]->state = THINKING;
		philosophers[i]->id = i;
	}
}

int main(int argc, char** argv) {
	int numPhilosophers = 0;
	Philosopher **philosophers;
	Input* input = (Input*)malloc(sizeof(Input));

	checkArgs(argc, argv, &numPhilosophers);
	philosophers = (Philosopher**)calloc(numPhilosophers, sizeof(Philosopher*));	
	sem_init(&mutex, 0, 1);
	sem_init(&eating_mutex, 0, (numPhilosophers / 2));
	init(philosophers, numPhilosophers);
	input->philosophers = philosophers;
	input->size = numPhilosophers;
	input->currentId = -1;
	printPhilosophersState(input);
	free(input);
	startAll(philosophers, numPhilosophers);
	waitAll(philosophers, numPhilosophers);
	
	pthread_exit(NULL);
}