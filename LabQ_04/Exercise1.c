#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include <time.h>
#include <semaphore.h>

void * sum_with_gap(void * arg);
int * random_array(int dim);
void print_array(int * v, int dim);
struct t_args{
    int * v;
    int num_threads;
    int n;
    int index;
};

/*
* I use the barrier implementation of slides, not the posix one
*/
typedef struct barrier_s{
    sem_t sem1, sem2;
    pthread_mutex_t mutex;
    int count;
} barrier_t;

barrier_t * bar;
int active_threads;
int main(int argc, char* argv[]){
    int i, j;
    int n = atoi(argv[1]);
    int * v;
    int dim = (int)pow(2,n);
    int num_threads = dim-1; // 2^n - 1 threads
    pthread_t *threads = (pthread_t *)malloc(num_threads*sizeof(pthread_t)); 
    struct t_args * arg = (struct t_args *)malloc(num_threads*sizeof(struct t_args));
    void * status;

    v = random_array(dim);
    printf("Original array: ");
    print_array(v, dim);
    
    // Allocate barrier
    bar = (barrier_t *)malloc(sizeof(barrier_t));
    sem_init(&bar->sem1,0,0);
    sem_init(&bar->sem2,0,0);
    pthread_mutex_init(&bar->mutex, NULL);
    bar->count=0;

    active_threads = num_threads; // at the beginning
    for(i=0; i<num_threads; i++){
        arg[i].num_threads = num_threads;
        arg[i].index = i+1; // i have one thread for each element from 1 to 2^n-2
        arg[i].n = n;
        arg[i].v = v;
        if(pthread_create(&threads[i], NULL, sum_with_gap, (void *)&arg[i]) != 0)
            printf("error in pthread_create"); 
    }
    // Print the array computed
    printf("%d ",v[0]);
    for(i=0; i<num_threads; i++){
        pthread_join(threads[i], (void *)status);
        // Element computed by that thread
        printf("%d ", v[arg[i].index]);
    }
    
}

void * sum_with_gap(void * arg){
    int i, j, index;
    int * v;
    int num_threads, n;
    int a, b, gap;
    struct t_args * args;
    int sum;

    args = (struct t_args *)arg;
    v = args->v;
    num_threads = args->num_threads;
    n = args->n;
    index = args->index;
    
    for(i=0;i<n;i++){ // n steps

        // At step i starting from 1 => gap = 2^(i-1) 
        gap = (int) pow(2,i); // i starts from 0 so i, not i-1

        if(index-gap >= 0)
            a = v[index-gap];
        else 
            a=0;

        b = v[index];
        sum = a + b;

        // BARRIER 1
        pthread_mutex_lock(&bar->mutex);
        bar->count++;
        if(bar->count == active_threads){
            for(j=0;j<active_threads;j++){
                sem_post(&bar->sem1);
            }
        }
        pthread_mutex_unlock(&bar->mutex);
        sem_wait(&bar->sem1);

        // I'm here when all the elements of step i have been computed
        // Only now i can write on the array
        v[index] = sum;

        // Check if i'm the thread with index i, terminate
        if(index == i){
            pthread_mutex_lock(&bar->mutex);
            bar->count--;
            pthread_mutex_unlock(&bar->mutex);

            active_threads--;
            pthread_exit(NULL);
        }

        // BARRIER 2 for avoiding problem of fast thread looping 
        pthread_mutex_lock(&bar->mutex);
        bar->count--;
        if(bar->count == 0){
            for(j=0;j<active_threads;j++){
                sem_post(&bar->sem2);
            }
        }
        pthread_mutex_unlock(&bar->mutex);
        sem_wait(&bar->sem2);

    }
    
    pthread_exit(NULL);
}

int * random_array(int dim){
    int i;
    int * v = (int *)malloc(dim * sizeof(int));
    srand(time(NULL));
    for(i=0;i<dim;i++){
        v[i] = (int)(rand() % 9) + 1;
    } 
    return v;
}

void print_array(int * v, int dim){
    int i;
    for(i=0;i<dim;i++){
        printf("%d ",v[i]);
    }
    printf("\n");
}