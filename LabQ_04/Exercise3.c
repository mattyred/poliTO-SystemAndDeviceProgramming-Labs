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
#include <dirent.h>

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

typedef struct mutex_sem_s{
    pthread_mutex_t mutex_sem;
} mutex_sem_t;

typedef struct barrier_s{
    sem_t sem1, sem2;
    pthread_mutex_t mutex;
    int count;
} barrier_t;

barrier_t * bar;


// Global semaphores
mutex_sem_t * mpipe;
barrier_t * bar;
sem_t sem_compare;

int num_active_reading_threads;
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
    mpipe = (mutex_sem_t *)malloc(sizeof(mutex_sem_t));
    pthread_mutex_init(&mpipe->mutex_sem, NULL);

    // Init semaphore to unlock last thread
    sem_init(&sem_compare,0,0);

    // Init barriers
    bar = (barrier_t *)malloc(sizeof(barrier_t));
    sem_init(&bar->sem1,0,0);
    sem_init(&bar->sem2,0,0);
    pthread_mutex_init(&bar->mutex, NULL);
    bar->count=0;

    // Launch reading threads and comparing thread
    num_active_reading_threads = num_reading_threads;
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
    int fd_r, nb, j;
    char s[MAXC];

    args = (struct t_compare_arg *)arg;
    fd_r = args->fd_r;
    while(1){
        sem_wait(&sem_compare); // unlocked by the last reading thread
        for(j=0;j<num_active_reading_threads;j++){ // for each active reading thread
            nb = read(fd_r, s, sizeof(s));
            printf("Read %d: %s\n",nb,s);
        }
        // Final part of barrier 1: unlock all reading threads
        for(j=0;j<num_active_reading_threads;j++){
            sem_post(&bar->sem1);
        }
    }
    
    pthread_exit(NULL);
}

void * visit_directory(void * arg){
    struct t_read_arg * args;
    int fd_w, j;
    struct stat buf;
	DIR *dp;
    struct dirent *dirp;
    char dir_name[MAXC];

    args = (struct t_read_arg *)arg;
    fd_w = args->fd_w;
    strcpy(dir_name, args->dir_name);

    // Obtain information about the directory
    lstat(dir_name, &buf);

    if(S_ISDIR(buf.st_mode) == 0){
		printf("Error, %s is not a directory!", dir_name);
		pthread_exit((void*)1);
	}

    // Open the directory
    if((dp = opendir(dir_name)) == NULL){
		printf("Error in opening %s", dir_name);
		pthread_exit((void*)1);
	}

    // Visit the directory
    while((dirp = readdir(dp)) != NULL){
        if(strcmp(dirp->d_name,".")!=0 && strcmp(dirp->d_name, "..")){
            printf("Element %s from %s\n", dirp->d_name, dir_name);
            // Write directory entries on pipe alone
            pthread_mutex_lock(&mpipe->mutex_sem);
            write(fd_w, dirp->d_name, sizeof(args->dir_name));
            pthread_mutex_unlock(&mpipe->mutex_sem);

            // BARRIER 1
            pthread_mutex_lock(&bar->mutex);
            bar->count++;
            if(bar->count == num_active_reading_threads){ // i'm the last thread
                sem_post(&sem_compare); // unlock comparing thread
            }
            pthread_mutex_unlock(&bar->mutex);
            sem_wait(&bar->sem1); // ---barrier---

            // BARRIER 2
            pthread_mutex_lock(&bar->mutex);
            bar->count--;
            if(bar->count == 0){
                for(j=0;j<num_active_reading_threads;j++){
                    sem_post(&bar->sem2);
                }
            }
            pthread_mutex_unlock(&bar->mutex);
            sem_wait(&bar->sem2);
        }
    }
    pthread_exit(NULL);
}