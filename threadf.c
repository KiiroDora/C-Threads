#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

//code for condition variables

pthread_mutex_t mutexFuel; //field for mutex
pthread_cond_t condFuel; //field for condition

int fuel = 0; //field for fuel variable

void* fuel_filling(void* arg) {
	for (int i = 0; i < 5; i++) {
		pthread_mutex_lock(&mutexFuel);
		fuel += 15;
		pthread_mutex_unlock(&mutexFuel);
		printf("Fuel-filling hehe   Fuel: %d \n", fuel);
		pthread_cond_signal(&condFuel); //give out signal when fuel is filled
		sleep(1);
	}
}

void* car(void* arg) {
	pthread_mutex_lock(&mutexFuel);
	while (fuel < 40) { //important to use while loop here to control the condition
		printf("And just let him starve?? \n");
		pthread_cond_wait(&condFuel, &mutexFuel); 
		//^call for mutex unlock and wait for signal on condFuel, then lock again
	}

	fuel -= 40;
	printf("I'm so thirsty      Fuel: %d \n", fuel);
	
	pthread_mutex_unlock(&mutexFuel);
}

int main (int argc, char* argv[]) {
	pthread_t threads[2]; //array of threads
	pthread_mutex_init(&mutexFuel, NULL); //initialize mutex
	pthread_cond_init(&condFuel, NULL); //initialize condition
	
	for (int i = 0; i < 2; i++) { //looping through array to create threads
		if (i == 1) { //to ensure the car thread gets created first
			if(pthread_create(&threads[i], NULL, &fuel_filling, NULL) != 0) {
				perror("Failed to create thread");
				return 1;
			}
		}
		
		else {
			if(pthread_create(&threads[i], NULL, &car, NULL) != 0) {
				perror("Failed to create thread");
				return 2;
			}
		}
	}
	
	for (int i = 0; i < 2; i++) { //loop through the array to join threads
		if(pthread_join(threads[i], NULL) != 0) {
			perror("Failed to join thread");
			return 3;
		}
	}
	
	pthread_mutex_destroy(&mutexFuel); //destroy mutex
	pthread_cond_destroy(&condFuel); //destroy condition
	
	return 0;
}
