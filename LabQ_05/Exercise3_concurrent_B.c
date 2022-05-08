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
#include <semaphore.h>

/*
* Idea: there is only one function, the same for each thread. Only the last thread (the one
* that is able to unlock the barrier) will execute the code inside the barrier computing
* the result res and unlocking all the other threads to let them finish. In order to parallelize
* the work each thread computes concurrently the value of v[i] but since we don't know in which
* order the threads compute the result we also store in array v_index the index of the element.
*/
void random_fill_matrix(float** mat, int nrow, int ncol);
void print_matrix(float** mat, int nrow, int ncol);
void random_fill_array(float * a, int dim);
void print_array(float * a, int dim);
void * dot_product(void * arg);
float take_element_i(int j, float * v, float *v_index, int n);
struct t_args{
    float * v1;
    float * v2;
    float ** mat;
    int n;
    int mat_col_index;
    float v_i;
    float num_threads;
};
typedef struct barrier_s{
    sem_t sem1, sem2;
    pthread_mutex_t mutex;
    int count;
} barrier_t;
barrier_t * bar;
float * v;
float * v_index;
int k = 0;
pthread_mutex_t mutex_v;
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

    // Allocate arrays and matrix
    v1 = (float *)malloc(n * sizeof(float));
    v2 = (float *)malloc(n * sizeof(float));
    v = (float *)malloc(n * sizeof(float));
    v_index = (float *)malloc(n * sizeof(float));
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

    // Init the barrier
    bar = (barrier_t *)malloc(sizeof(barrier_t));
    sem_init(&bar->sem1,0,0);
    sem_init(&bar->sem2,0,0);
    pthread_mutex_init(&bar->mutex, NULL);
    bar->count=0;

    //Init mutex semaphore to write inside v
    pthread_mutex_init(&mutex_v, NULL);

    // Run n threads
    for(i=0;i<n;i++){
        arg[i].v1 = v1;
        arg[i].v2 = v2;
        arg[i].mat = mat; 
        arg[i].n = n;
        arg[i].mat_col_index = i;
        arg[i].num_threads = n;
        pthread_create(&threads[i], NULL, (void*)dot_product, (void *)&arg[i]);
    }
    
    // Wait for termination: NECESSARY
    for(i=0;i<n;i++){
        pthread_join(threads[i], NULL);
    }
    return 0;
}

void * dot_product(void * arg){
    struct t_args * args;
    float * v1;
    float * v2;
    float ** mat;
    int n;
    int mat_col_index;
    float v_i = 0;
    int i,j;
    float res;

    // Retrieve struct attributes
    args = (struct t_args *)arg;
    v1 = args->v1;
    v2 = args->v2;
    n = args->n;
    mat = args->mat;
    mat_col_index = args->mat_col_index;

    // Compute v = v1.T * mat
    for(i=0;i<n;i++){
        v_i += v1[i] * mat[mat_col_index][i];
    }

    // Write the result v_i inside the array v and the index of v_i inside v_index
    pthread_mutex_lock(&mutex_v);
    v[k] = v_i;
    v_index[k++] = mat_col_index;
    pthread_mutex_unlock(&mutex_v);

    // BARRIER
    pthread_mutex_lock(&bar->mutex);
    bar->count++;
    if(bar->count == args->num_threads){ // i'm the last thread
        // Compute res
        for(i=0;i<n;i++){
            res += take_element_i(i,v,v_index,n) * v2[i];
        }
        printf("Res = %.6f\n", res);
        for(j=0;j<args->num_threads;j++){
            sem_post(&bar->sem1); // unlock other threads to let them do pthread_exit
        }
    }
    pthread_mutex_unlock(&bar->mutex);
    sem_wait(&bar->sem1); // ---barrier---
    pthread_exit(NULL);
}

float take_element_i(int j, float * v, float *v_index, int n){
    int i;
    for(i=0;i<n;i++){
        if(v_index[i] == j)
            return v[i];
    }
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