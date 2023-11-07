#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

//code for summing numbers from an array by collecting sums in two separate threads and summing those together
//also returning a value via join thread

int primes[10] = {2,3,5,7,11,13,17,19,23,29};

int sum;

void* pickPrime(void* arg) {
	int index = *(int*)arg; //index is set to a dereferenced arg, which is the i value in the first main for loop
	int sum = 0;
	
	printf ("The sum of ");
	
	for (int i = 0; i < 5; i++) { //looping through the array, sum 5 numbers in each thread
		printf ("%d ", primes[index + i]); //index is 0 so we need to add i
		sum += primes[index + i];
	}
	
	printf("= %d\n", sum);
	
	*(int*)arg = sum; //reallocating the sum to this pointer (it also works with malloc so that we can free it in the main function instead of here)
	return arg; //return the sum value inside arg
}

int main (int argc, char* argv[]) {
	pthread_t threads[2];
	for (int i = 0; i < 2; i++) {
		int* a = malloc(sizeof(int)); //allocate memory for each i value to prevent threads messing up and taking the same value 
		*a = i * 5; //to split the array in the middle, thread1 -> 0, thread2 -> 5 
		
		if (pthread_create(&threads[i], NULL, &pickPrime, a) != 0) {
			perror("Error: Failed to create thread");
			return 1;
		}
	}
	
	int totalSum = 0;
	
	for (int i = 0; i < 2; i++) {
		void* result; //this will hold the arg value returned from the function
		if (pthread_join(threads[i], &result) != 0) {
			perror("Error: Thread failed to finish executing");
			return 2;
		}
		totalSum += *(int*)result; //typecast before dereferencing, result is void*
		free(result); //free the memory allocated with malloc
	}
	
		
	printf("Result: %d\n", totalSum);

	return 0;
}
