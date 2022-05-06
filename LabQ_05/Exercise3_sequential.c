#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

void random_fill_array(float * a, int dim);
void print_array(float * a, int dim);
int main(int argc, char* argv[]){
    int n;
    float *v1, *v2;
    float **mat;
    int i,j;

    n = atoi(argv[1]);

    // Initialize the seed of random
    srand(time(NULL));

    // Allocate the arrays v1,v2 and matrix mat
    v1 = (float *)malloc(n * sizeof(float));
    v2 = (float *)malloc(n * sizeof(float));
    mat = (float **)malloc(n * sizeof(float*));
    for(i=0;i<n;i++)
        mat[i] = (float *)malloc(n * sizeof(float));
    
    // Fill arrays and matrix with random numbers
    random_fill_array(v1, n);
    print_array(v1,n);
    random_fill_array(v2, n);
    print_array(v2,n);
    // random_fill_matrix(mat);


}

void random_fill_array(float * a, int dim){
    int i;
    for(i=0;i<dim;i++){
        a[i] = -0.5 + ((float)rand()/(float)RAND_MAX);
    }
}

void print_array(float * a, int dim){
    int i;
    for(i=0;i<dim;i++){
        printf("%f ",a[i]);
    }
    printf("\n");
}