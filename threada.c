#include <stdio.h>
#include <pthread.h>

void* computation(void* add);

int main() {
	pthread_t thread1, thread2;
	
	long value1 = 1;
	long value2 = 2;

	//creating threads (takes: pointer to thread, NULL, the function to be executed, the value passed into the function)
	if (pthread_create(&thread1, NULL, computation, (void*) &value1) != 0) {
		return 1;
	}
	
	if (pthread_create(&thread2, NULL, computation, (void*) &value2) != 0) {
		return 2;
	}
	
	//joining threads (takes: thread, NULL(the value to return from function, if any))
	if (pthread_join(thread1, NULL) != 0) {
		return 3;
	}
	
	if (pthread_join(thread2, NULL) != 0) {
		return 4;
	}
	
	return 0;
}

void* computation(void* add) {
	long sum = 0;
	long* add_num = (long*) (add);
	
	for (long i = 0; i < 1000000000; i++) {
		sum += *add_num;
	}
	printf("Add: %ld\n", *add_num);
	return NULL;
}
