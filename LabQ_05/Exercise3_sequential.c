#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

float compute_res(float * v1, float * v2, float ** mat, int n);
void random_fill_matrix(float** mat, int nrow, int ncol);
void print_matrix(float** mat, int nrow, int ncol);
void random_fill_array(float * a, int dim);
void print_array(float * a, int dim);
int main(int argc, char* argv[]){
    int n;
    float *v1, *v2;
    float **mat;
    float res;
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
    printf("v1: "); print_array(v1,n);
    random_fill_array(v2, n);
    printf("v2: "); print_array(v2,n);
    random_fill_matrix(mat, n, n);
    printf("mat: \n"); print_matrix(mat, n, n);

    // Compute res
    res = compute_res(v1,v2,mat,n);
    printf("res = %.6f\n", res);


}

float compute_res(float * v1, float * v2, float ** mat, int n){
    int i,j;
    float v_i;
    float res = 0;
    float * v = (float*)malloc(n*sizeof(float));
    // Compute v = v1.T * mat
    for(i=0;i<n;i++){
        v_i = 0;
        for(j=0;j<n;j++){
            v_i += v1[j] * mat[j][i];
        }
        v[i] = v_i;
    }

    // Compute res = v * v2
    for(i=0;i<n;i++){
        res += v[i] * v2[i];
    }
    free(v);
    return res;
}


void random_fill_array(float * a, int dim){
    int i;
    for(i=0;i<dim;i++){
        a[i] = -0.5 + ((float)rand()/(float)RAND_MAX);
    }
}

void random_fill_matrix(float** mat, int nrow, int ncol){
    int i,j;
    for(i=0;i<nrow;i++){
        for(j=0;j<ncol;j++){
            mat[i][j] = -0.5 + ((float)rand()/(float)RAND_MAX);
        }
    }
}

void print_array(float * a, int dim){
    int i;
    for(i=0;i<dim;i++){
        printf("%.4f ",a[i]);
    }
    printf("\n");
}

void print_matrix(float** mat, int nrow, int ncol){
    int i,j;
    for(i=0;i<nrow;i++){
        for(j=0;j<ncol;j++){
            printf("%.4f ", mat[i][j]);
        }
        printf("\n");
    }
    
}