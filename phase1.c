#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

/*
M2 -> 4 by 4 array
M1 -> 10 by 10 array, 7 by 7 with 3 padding each (filled with 0s)
Output -> 7 by 7 array

M2 loops through M1, multiply each value that overlaps with itself
(an identity matrix was used for testing) 
*/



//field declarations
int m1[10][10], m2[4][4], output[7][7];
int sum = 0; //temporary sum value to assign to output array values

void print7(int m[7][7]);

void printSqrMatrix (int size, int* matrix);

int main (int argc, char* argv[]) {
	srand(time(NULL)); //initialize srand

	//initializing m1, with random values
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
	
	/* CODE FOR RANDOMIZING M2
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m2[i][j] = rand() % 20 + 1; //fill the matrix with random numbers up to 20 
		}
	}
	COMMENTED OUT TO LATER USE IN PRESENTATION */ 
	
	//go through iterations and write on the output matrix
	for (int m = 0; m < 7; m++) { //output's height
		for (int n = 0; n < 7; n++) { //output's width
			for(int i = 0; i < 4; i++) { //looping through m1 and m2 in m2's sizes
				for(int j = 0; j < 4; j++) { 
					sum += m1[i+m][j+n] * m2 [i][j]; //multiply the overlapping m1 and m2 values and add it to the sum, increment m1's row and column values by n and m respectively to traverse over m1
				}
			}
			output[m][n] = sum; //write the sum on the corresponding slot on output
			sum = 0;
		}
	}
	
	//print the matrices

	printf("M1:\n");
	printSqrMatrix(10, m1[0]);

	printf("M2:\n");
	printSqrMatrix(4, m2[0]);
	
	printf("OUTPUT:\n");
	printSqrMatrix(7, output[0]);
}


void printSqrMatrix (int size, int* matrix) { //function to print matrices
	for (int i = 0; i < size; i++) { //columns
		if (i < 7) { //does not print the excess if column exceeds 7 (just for appearance, to remove padding from M1)
			for (int j = 0; j < size; j++) { //rows
				if (j < 7) { //does not print excess rows (again to remove padding)
					if (*(matrix + i*size + j) / 10 == 0) { //this section is just for appearance
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
