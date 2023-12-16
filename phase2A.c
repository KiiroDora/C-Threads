#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

/*
M2 -> 4 by 4 array
M1 -> 10 by 10 array, 7 by 7 with 3 padding each (filled with 0's)
Output -> 7 by 7 array (filled with -1's)

M2 loops through M1 in a multithreaded manner, multiplying each value that overlaps with itself
(an identity matrix was used for testing) 
*/


//field declarations
int m1[10][10], m2[4][4], output[7][7];
int sum = 0; //temporary sum value to assign to output array values

pthread_t threads[49]; //a total of 49 values will be filled in the output array
pthread_mutex_t mutex; //defining mutex
pthread_cond_t cond; //defining condition variable

int m = 0; 
int n = 0; //m and n are global variables for the output's width and height respectively

void* fillMatrix (void* arg); 

void printSqrMatrix (int size, int* matrix);


//main function
int main (int argc, char* argv[]) {
	srand(time(NULL)); //initialize srand
	pthread_mutex_init(&mutex, NULL); //initialize mutex
	pthread_cond_init(&cond, NULL); //initialize condition

	//initializing m1, with random values
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 7; j++) {
			m1[i][j] = rand() % 20 + 1; //fill the matrix with random numbers up to 20 
		}
	}

	/* CODE FOR INITIALIZING M1 AS AN ARRAY OF 1'S
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 7; j++) {
			m1[i][j] = 1; //fill the matrix with 1
		}
	}
	COMMENTED OUT FOR POSSIBLE USE IN PRESENTATION */
	
	//initialize m2 as an identity matrix
	for (int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			if (i == j) {
				m2[i][j] = 1;
			} else {
				m2[i][j] = 0;
			}
		}
	}
	
	//initialize output as a matrix filled with -1's
	for (int i = 0; i < 7; i++) {
		for(int j = 0; j < 7; j++) {
			output[i][j] = -1;
		}
	}
	
	/* CODE FOR RANDOMIZING M2
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m2[i][j] = rand() % 20 + 1; //fill the matrix with random numbers up to 20 
		}
	}
	COMMENTED OUT TO LATER USE IN PRESENTATION */ 
	
	//create the threads 
	for (int t = 0; t < 49; t++) {
		if (pthread_create(&threads[t], NULL, &fillMatrix, NULL) != 0) { //we pass the fillMatrix function while creating each thread
			perror("Failed to create thread");
			return 1;
		}
		printf("Created thread %d \n", t+1);
	}
	
	
	//join the threads
	for (int t = 0; t < 49; t++) {
		if (pthread_join(threads[t], NULL) != 0) { 
			perror("Failed to join thread");
			return 2;
		}
		printf("Joined thread %d \n\n", t+1);
	}
	
	pthread_mutex_destroy(&mutex); //destroy mutex
	pthread_cond_destroy(&cond); //destroy condition
	
	//print the matrices

	printf("M1:\n");
	printSqrMatrix(10, m1[0]);

	printf("M2:\n");
	printSqrMatrix(4, m2[0]);
	
	printf("OUTPUT:\n");
	printSqrMatrix(7, output[0]);
}

void* fillMatrix (void* arg) { //the function the threads will use to fill the output matrix
	pthread_mutex_lock(&mutex); //lock the other threads
	while (output[m][n] != -1) { //if the m and n values have not been updated yet
		pthread_cond_wait(&cond, &mutex); //wait for a signal
	}

	for(int i = 0; i < 4; i++) { //looping through m1 and m2 in m2's sizes
		for(int j = 0; j < 4; j++) { 
			sum += m1[i+m][j+n] * m2 [i][j]; //multiply the overlapping m1 and m2 values and add it to the sum, increment m1's row and column values by n and m respectively to traverse over m1
			}
	}
		
	output[m][n] = sum; //write the sum on the corresponding slot on output
	printSqrMatrix(7, output[0]); //print the output matrix
	sum = 0; //reset the sum value to use on the next thread
	
	n++; //increment n
	if (n >= 7) {  //increment m and set n to 0 if n has become larger than 7 (to mimic a for loop)
		n = 0;
		m++;
	}
	
	pthread_cond_signal(&cond); //send a signal to a waiting thread
	pthread_mutex_unlock(&mutex); //unlock all other threads
}

void printSqrMatrix (int size, int* matrix) { //function to print matrices
	for (int i = 0; i < size; i++) { //columns
		if (i < 7) { //does not print the excess if column exceeds 7 (just for appearance, to remove padding from M1)
			for (int j = 0; j < size; j++) { //rows
				if (j < 7) { //does not print excess rows (again to remove padding)
					if (*(matrix + i*size + j) / 10 == 0 && *(matrix + i*size + j) >= 0) { //this section is just for appearance
						printf("[ %d] ", *(matrix + i*size + j)); //put space for 1 digit nums for neat display
					} else {
						printf("[%d] ", *(matrix + i*size + j));
					}
				}
			}
			printf("\n");
		}
		
	}
	
	printf("\n");
}
