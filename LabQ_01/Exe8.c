#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <pthread.h>
#define N 5

// A : [r,x]
// B : [x,c]
   
void mat_mul (int **A, int **B, int r, int x, int c, int **C){
		pthread_t matrix_thread[r*c];
		int i,j;
		for(i=0; i<r; i++){
			for(j=0; j<c; j++){
				printf("%d ", A[i][j]);
			}
			printf("\n");
		}
	
}

int main(){
	
	int r = 3, x = 3, c = 3;
	int i,j,k=1;
	
	/* Initilize matrix A */
	int **A = (int **)malloc(r * sizeof(int *));
	for(i=0; i<r; i++){
		A[i] = (int *)malloc(c * sizeof(int));
	}
	
	/* Initialize matrix B */
	int **B = (int **)malloc(x * sizeof(int *));
	for(i=0; i<x; i++){
		B[i] = (int *)malloc(c * sizeof(int));
	}
	
	/* Initialize matrix C */
	int **C = (int **)malloc(r * sizeof(int *));
	for(i=0; i<r; i++){
		C[i] = (int *)malloc(c * sizeof(int));
	}
	
	for(i=0; i<r; i++){
		for(j=0; j<c; j++){
			A[i][j] = k;
			B[i][j] = k++; // supposing that B has the same dimension of A
		}
	}

	mat_mul(A,B,r,x,c,C);
	/*
	 * Create one thread for each element to be computed. Each thread is in charge
	 * of multiplying a row by a colum element by element and summing them
	 */
	
	
}
