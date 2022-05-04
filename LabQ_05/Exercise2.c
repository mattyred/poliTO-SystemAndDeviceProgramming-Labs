#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include <time.h>
#include <semaphore.h>


void print_array(int * v, int dim);
void * sort_file(void * arg);
void bubblesort(int * v, int n);
void swap(int *a, int *b);
void sort_array();
void sorted_merge(int * a1, int dim1, int * a2, int dim2, int * a3);
struct t_args{
    int fd;
    int * array;
    int n;
};

int main(int argc, char* argv[]){
    
    int * input_fd;
    void * status;
    int i,j,k;
    int final_array_dim = 0;
    int * a1 = NULL;
    int dim1 = 0;
    int * a2;
    int dim2;
    int * a3;

    struct t_args * arg;
    pthread_t * threads;
    int num_input_files = argc-2;

    input_fd = (int *)malloc(num_input_files * sizeof(int));
    arg = (struct t_args *)malloc(num_input_files * sizeof(struct t_args));
    threads = (pthread_t *)malloc(num_input_files * sizeof(pthread_t));
    // Read input parameters
    for(i=1, j=0;i<=argc-2;i++, j++){
        arg[j].fd = open(argv[i], O_RDONLY);
        pthread_create(&threads[j], NULL, sort_file, (void*)&arg[j]);
    }

    for(i=0;i<num_input_files;i++){
        pthread_join(threads[i], &status);
        // After array i termination
        dim2 = arg[i].n;
        a3 = (int *)malloc((dim2+dim1) * sizeof(int));
        sorted_merge(a1,dim1,arg[i].array,dim2,a3);
        dim1 = dim2 + dim1;
        a1 = a3;
    }
    
    
    
    return 0;
    
}

void sorted_merge(int * a1, int dim1, int * a2, int dim2, int * a3){
    int i = 0, j = 0, k = 0;
    while(1){
        if(i < dim1 && j < dim2){
            if(a1[i] <= a2[j]){
                a3[k++] = a1[i++];
            }else if(a1[i] > a2[j]){
                a3[k++] = a2[j++];
            }
            continue;
        
        }else if(i >= dim1 || j >= dim2){
            if(i >= dim1){
                a3[k++] = a2[j++];
            }else if(j >= dim2){
                a3[k++] = a1[i++];
            }
        }else if(i >= dim1 && j >= dim2){
            printf("here");// TODO: non entra qui
            break;
        } 
    }
    return;
}
void * sort_file(void * arg){
    struct t_args * t_arg;
    int fd;
    int n, dim, nbyte;
    int i = 0;

    t_arg = (struct t_args *)arg;
    fd = t_arg->fd;

    // Read the content of the file
    while((nbyte = read(fd, &n, sizeof(int))) > 0){
        // Read number of elements and malloc the array
        if(i==0){
            t_arg->n = n;
            t_arg->array = (int *)malloc(n * sizeof(int));
        }else{// Fill the array staring from 0
            t_arg->array[i-1] = n;
        }
        i++;
    }
    
    // Sort the file
    bubblesort(t_arg->array, t_arg->n);
    pthread_exit(NULL);
}


void print_array(int * v, int dim){
    int i;
    for(i=0;i<dim;i++){
        printf("%d ",v[i]);
    }
    printf("\n");
}

void bubblesort(int * v, int n){
    int i,j;

    for(i=0;i<n-1;i++){
        for(j=0;j<n-i-1;j++){
            if(v[j] > v[j+1]){
                swap(&v[j],&v[j+1]);
            }
        }
    }
}

void swap(int *a, int*b){
    int t;
    t = *a;
    *a = *b;
    *b = t;
}

