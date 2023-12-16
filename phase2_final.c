#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

/*
M1 -> 10 by 10 array, 7 by 7 with 3 padding each (filled with 0's)
M2 -> 4 by 4 array
Output -> 7 by 7 array (filled with -1's)

M2 loops through M1 in multithreaded iterations, multiplying each value that overlaps with itself
(an identity matrix was used for testing) 
*/


//field declarations
int m1[10][10], m2[4][4], output[7][7]; //defining matrices
pthread_t threads[49]; //a total of 49 values will be filled in the output array, so we make 49 threads


//functions
void printSqrMatrix (int size, int* matrix) { //function to print matrices

	for (int i = 0; i < size; i++) { //this loop is for the columns
		if (i < 7) { //does not print excess columns (to remove padding of 0's from M1)
			for (int j = 0; j < size; j++) { //this loop is for the rows
				if (j < 7) { //does not print excess rows
					if (*(matrix + i*size + j) / 10 == 0 && *(matrix + i*size + j) >= 0) {
						printf("[ %d] ", *(matrix + i*size + j)); //put space for 1 digit nums for neat display
					} 
					
					else {
						printf("[%d] ", *(matrix + i*size + j));
					}
				}
			}
			printf("\n");
		}
	}
	printf("\n");
}

void threadWork (int height, int width)  { //function to calculate and store the sums in the output matrix and print it

	int sum = 0; 
	for(int i = 0; i < 4; i++) { //looping through m1 and m2 in m2's sizes
		for(int j = 0; j < 4; j++) { 
			sum += m1[i+height][j+width] * m2 [i][j]; //multiply the overlapping m1 and m2 values and add it to the sum, increment m1's row and column values by width and height respectively to traverse over m1
			}
	}
	
	output[height][width] = sum; //write the sum on the corresponding slot on output
	printSqrMatrix(7, output[0]); //print the output matrix
}

void* fillMatrix (void* arg) { //the function the threads will use to fill the output matrix

	int coord = *(int*)arg; //dereferencing the value inserted in function for each thread
	int height = coord/7; //array indexes of the columns
	int width = coord%7; //array indexes of the rows
	
	printf("Thread %d result:\n", coord+1);
	threadWork(height, width);
	
	return arg; //return the value inside arg to be freed later
}


//main function
int main (int argc, char* argv[]) {

	srand(time(NULL)); //initialize srand
	

	//initializing m1 with random values
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 7; j++) {
			m1[i][j] = rand() % 20 + 1; //fill the matrix with random numbers up to 20 
		}
	}
	
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
	
	/* CODE FOR INITIALIZING M1 AS AN ARRAY OF 1'S
	for (int i = 0; i < 7; i++) { 	
		for (int j = 0; j < 7; j++) {
			m1[i][j] = 1; //fill the matrix with 1
		}
	}
	
	/* CODE FOR RANDOMIZING M2
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m2[i][j] = rand() % 20 + 1; //fill the matrix with random numbers up to 20 
		}
	}
	COMMENTED OUT FOR LATER USE IN PRESENTATION */ 
	
	
	//create the threads 
	for (int t = 0; t < 49; t++) {
		int* coord = malloc(sizeof(int)); //dynamically allocating the coordinate value for each thread to prevent race condition
		*coord = t; //assigning the index value to the dynamically allocated variable
		
		if (pthread_create(&threads[t], NULL, &fillMatrix, coord) != 0) { //we pass the fillMatrix function and the coord pointer for each thread
			perror("Failed to create thread");
			return 1;
		}
		printf("Created thread %d \n", t+1);
	}
	
	
	//join the threads
	for (int t = 0; t < 49; t++) {
		void* coord; //this will hold the arg value returned from each thread
		
		if (pthread_join(threads[t], &coord) != 0) { 
			perror("Failed to join thread");
			return 2;
		}
		printf("Joined thread %d \n\n", t+1);
		
		free(coord); //free the allocated memory
	}
	
	
	//print the matrices
	printf("M1:\n");
	printSqrMatrix(10, m1[0]);

	printf("M2:\n");
	printSqrMatrix(4, m2[0]);
	
	printf("OUTPUT:\n");
	printSqrMatrix(7, output[0]);
}
