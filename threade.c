#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

//trylock code

pthread_mutex_t mutex; //defining mutex

void* routine(void* arg) {
	if (pthread_mutex_trylock(&mutex) == 0) { //using trylock instead of lock
		printf("Locked!\n");
		sleep(1);
		pthread_mutex_unlock(&mutex);
	}
	
	else {
		printf("Didn't get locked\n");
	}
}

int main (int argc, char* argv[]) {
	pthread_t threads[4];
	pthread_mutex_init(&mutex, NULL); //initialize mutex
	
	for (int i = 0; i < 4; i++) {
		if (pthread_create(&threads[i], NULL, &routine, NULL) != 0) {
			perror("Error: Couldn't create thread");
			return 1;
		}
	}
	
	for (int i = 0; i < 4; i++) {
		if (pthread_join(threads[i], NULL) != 0) {
			perror("Error: Couldn't join thread");
			return 2;
		}
	}
		
	pthread_mutex_destroy(&mutex);	
	
	return 0;
}
