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

#define MAXC 32

void * visit_directory(void * arg);
void * compare_entries(void * arg);

struct t_read_arg{
    char dir_name[MAXC];
    int fd_w;
};

struct t_compare_arg{
    int fd_r;
};

typedef struct mutex_type_s{
    pthread_mutex_t mutex_pipe;
} mutex_pipe_t;

// Global semaphores
mutex_pipe_t * mp;

int num_active_threads;
int main(int argc, char* argv[]){
    int fd_pipe[2];
    int num_reading_threads;
    int i;
    pthread_t * reading_threads;
    pthread_t comparing_thread;
    struct t_read_arg * reading_threads_args;
    struct t_compare_arg  compare_thread_args;
    void * status;

    num_reading_threads = argc - 1;
    reading_threads = (pthread_t *)malloc(num_reading_threads * sizeof(pthread_t));
    reading_threads_args = (struct t_read_arg *)malloc(num_reading_threads * sizeof(struct t_read_arg));

    // Open pipe
    pipe(fd_pipe);

    // Init mutex pipe semaphore
    mp = (mutex_pipe_t *)malloc(sizeof(mutex_pipe_t));
    pthread_mutex_init(&mp->mutex_pipe, NULL);

    // Launch reading threads and comparing thread
    num_active_threads = num_reading_threads + 1;
    for(i=0;i<num_reading_threads;i++){
        strcpy(reading_threads_args[i].dir_name, argv[i+1]);
        reading_threads_args[i].fd_w = fd_pipe[1]; // writing thread
        pthread_create(&reading_threads[i], NULL, visit_directory, (void *)&reading_threads_args[i]);
    }
    compare_thread_args.fd_r = fd_pipe[0]; // reading thread
    pthread_create(&comparing_thread, NULL, compare_entries, (void *)&compare_thread_args);

    // Join threads
    for(i=0;i<num_reading_threads;i++){
        pthread_join(reading_threads[i], &status);
    }
    pthread_join(comparing_thread, &status);

    return 0;
}

void * compare_entries(void * arg){
    struct t_compare_arg * args;
    int fd_r, nb;
    char s[MAXC];

    args = (struct t_compare_arg *)arg;
    fd_r = args->fd_r;


    while(1){
        sleep(1);
        nb = read(fd_r, s, sizeof(s));
        printf("Read %d: %s\n",nb,s);
    }

    pthread_exit(NULL);
}

void * visit_directory(void * arg){
    struct t_read_arg * args;
    int fd_w;

    args = (struct t_read_arg *)arg;
    fd_w = args->fd_w;

    // Read from directory 1 element
    // TO DO

    // Write directory entries on pipe
    pthread_mutex_lock(&mp->mutex_pipe);
    write(fd_w, args->dir_name, sizeof(args->dir_name));
    pthread_mutex_unlock(&mp->mutex_pipe);

    pthread_exit(NULL);
}