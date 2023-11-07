#include <stdio.h>
#include <pthread.h>

//studying threads (race condition, mutex, looping through arrays to create threads...)

int notifs = 0;
pthread_mutex_t mutex; //defining mutex

void* routine() {
	for (int i = 0; i < 10000000; i++) {
		pthread_mutex_lock(&mutex); //lock other threads
		notifs++;
		pthread_mutex_unlock(&mutex); //unlock the threads
	}
}

int main (int argc, char* argv[]) {
	pthread_t threads[8];
	pthread_mutex_init(&mutex, NULL); //initialize mutex (mutex prevents threads running into each other while incrementing stuff etc.)
	int i;
	
	//looping through array of threads to create and them more easily
	for (i = 0; i < 8; i++) {
		if (pthread_create(&threads[i], NULL, routine, NULL) != 0) {
			perror("Failed to create thread");
			return 1;
		}
		printf("Created thread %d \n", i+1);
	}
	
	//looping through array to join threads
	for (i = 0; i < 8; i++) {
		if (pthread_join(threads[i], NULL) != 0) {
			perror("Thread failed to finish execution");
			return 2;
		}
		printf("Thread %d has finished execution \n", i+1);
	}
	
	pthread_mutex_destroy(&mutex); //destroy mutex once it's done its job
	printf("Number incremented by: %d\n", notifs);
	
	return 0;
}
