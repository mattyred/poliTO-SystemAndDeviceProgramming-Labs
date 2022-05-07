#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <pthread.h>

void random_fill_matrix(float** mat, int nrow, int ncol);
void print_matrix(float** mat, int nrow, int ncol);
void random_fill_array(float * a, int dim);
void print_array(float * a, int dim);
void * dot_product(void * arg);
struct t_args{
    float * v1;
    float ** mat;
    int n;
    int mat_col_index;
    float v_i;
};
int main(int argc, char* argv[]){
    int n;
    float *v1, *v2;
    float **mat;
    float res=0;
    int i;
    pthread_t *threads; 
    struct t_args * arg;

    n = atoi(argv[1]);

    // Initialize the seed of random
    srand(time(NULL));

    // Allocate the arrays v1,v2, matrix mat, threads array
    v1 = (float *)malloc(n * sizeof(float));
    v2 = (float *)malloc(n * sizeof(float));
    mat = (float **)malloc(n * sizeof(float*));
    for(i=0;i<n;i++)
        mat[i] = (float *)malloc(n * sizeof(float));
    threads = (pthread_t *)malloc(n*sizeof(pthread_t));
    arg = (struct t_args *)malloc(n*sizeof(struct t_args));
    
    // Fill arrays and matrix with random numbers
    random_fill_array(v1, n);
    printf("v1: "); print_array(v1,n);
    random_fill_array(v2, n);
    printf("v2: "); print_array(v2,n);
    random_fill_matrix(mat, n, n);
    printf("mat: \n"); print_matrix(mat, n, n);

    // Run n threads
    for(i=0;i<n;i++){
        arg[i].v1 = v1;
        arg[i].mat = mat; 
        arg[i].n = n;
        arg[i].mat_col_index = i;
        pthread_create(&threads[i], NULL, (void*)dot_product, (void *)&arg[i]);
    }

    // Wait all the threads
    for(i=0;i<n;i++){
        pthread_join(threads[i], NULL); // passing (void*)status gives segfault

    }

    // Compute res
    for(i=0;i<n;i++){
        res+=arg[i].v_i * v2[i];
    }

    printf("Res = %.6f\n", res);
    return 0;
}

void * dot_product(void * arg){
    struct t_args * args;
    float * v1;
    float ** mat;
    int n;
    int mat_col_index;
    float v_i = 0;
    int i;

    // Retrieve struct attributes
    args = (struct t_args *)arg;
    v1 = args->v1;
    n = args->n;
    mat = args->mat;
    mat_col_index = args->mat_col_index;

    // Compute v = v1.T * mat
    for(i=0;i<n;i++){
        v_i += v1[i] * mat[mat_col_index][i];
    }
    args->v_i = v_i;
    // printf("Thread %ld computes %.4f\n",pthread_self(), v_i);
    pthread_exit(NULL);
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