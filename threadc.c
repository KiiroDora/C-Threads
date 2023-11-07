#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

//code for passing arguments to threads
//each thread prints one unique prime number from the array

int primes[10] = {2,3,5,7,11,13,17,19,23,29};

int sum;

void* pickPrime(void* arg) { //the address primes+i gets stored in arg
	int index = *(int*)arg; //set index to a dereferenced arg (so the value in the array)
	printf("%d ", index);
}

int main (int argc, char* argv[]) {
	pthread_t threads[10];
	for (int i = 0; i < 10; i++) {
		//if we use &i instead of primes+i the threads will use the wrong index
		//another solution to this is using malloc for i
		if (pthread_create(&threads[i], NULL, &pickPrime, primes+i) != 0) {
			perror("Error: Failed to create thread");
			return 1;
		}
	}
	
	for (int i = 0; i < 10; i++) {
		if (pthread_join(threads[i], NULL) != 0) {
			perror("Error: Thread failed to finish executing");
			return 2;
		}
	}

	return 0;
}
