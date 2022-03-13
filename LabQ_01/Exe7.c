#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <pthread.h>

struct info{
	long int * tid_tree; // array to store all the TID along the path
	int depth; // when reaches n-1 stop creating threads
	int n; // input value
	
};

void *thread_function(void * par){
	
	struct info * arg_struct;
	arg_struct = (struct info *) par;
	pthread_t childrenThreads[2];
	struct info info_thread[2];
	int i,j;
	void * status;
	
	if(arg_struct->depth == arg_struct->n - 1){
		// print the tree of threads...
		for(i=0; i<arg_struct->n; i++){
			printf("%ld ", arg_struct->tid_tree[i]);
		}
		printf("\n");
		
	}else{
		
		// Copy the struct received into another one and launch the threads
		for(i=0; i<2; i++){
			info_thread[i].n = arg_struct->n;
			info_thread[i].depth = arg_struct->depth+1;
			info_thread[i].tid_tree = (long int *)malloc((1<<arg_struct->n) * sizeof(long int));
			for(j=0; j<=arg_struct->depth; j++)
				info_thread[i].tid_tree[j] = arg_struct -> tid_tree[j];
				
			info_thread[i].tid_tree[j] = (long int) pthread_self();
			pthread_create(&childrenThreads[i], NULL, thread_function, (void *)&info_thread[i]);
		}
		
		// Wait for them to terminate
		pthread_join(childrenThreads[0], &status);
		pthread_join(childrenThreads[1], &status);
	}
	
	
	pthread_exit(NULL);
}

int main(int argc, char* argv[]){
	
	int n = atoi(argv[1]);
	void * status;
	pthread_t childrenThreads[2];
	struct info info_thread[2];
	int i;
	// Initialize the struct and create the threads
	for(i=0; i<2; i++){
		info_thread[i].depth = 0;
		info_thread[i].n = n;
		info_thread[i].tid_tree = (long int *)malloc((1<<n) * sizeof(long int)); // initialize the array to store the threads tree
		info_thread[i].tid_tree[0] = (long int) pthread_self();
			
		pthread_create(&childrenThreads[i], NULL, thread_function, (void *)&info_thread[i]);
	}
	
	// Wait for them to terminate
	pthread_join(childrenThreads[0], &status);
	pthread_join(childrenThreads[1], &status);
}
