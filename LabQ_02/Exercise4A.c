#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <pthread.h>

#define MAXC 31
struct t_arg{
	char dir_name[MAXC];
};

void visit(char* path, pthread_t tid){
	
	struct dirent *dirp;
	struct stat buf;
	char path_obj[MAXC];
	DIR* dp;
	
	// open the directory
	if((dp = opendir(path)) == NULL){
		printf("Error in opening %s", path);
		pthread_exit(0);
	}
	
	while((dirp = readdir(dp))){
		
		if((strcmp(dirp->d_name,".") != 0) && (strcmp(dirp->d_name, "..") != 0)){ // if it is not . or ..
			
			// build the name of the path: e.g. dir_1/file1.txt [path_obj]
			sprintf(path_obj,"%s/%s",path,dirp->d_name); // note: it is important not to overwrite 'path'
			
			// obtain information about the path_obj
			lstat(path_obj, &buf);
			
			if(S_ISDIR(buf.st_mode) != 0){ // path_obj is a directory
				// print out information
				printf("Thread %ld is visiting folder %s\n",tid, path_obj);
				
				// visit recursively the directory
				visit(path_obj, tid);
			}else{ // it is a file, continue visiting but go up of one level [path]
				printf("Thread %ld is visiting file %s\n",tid, path_obj);
			}			
		}else{
			continue;
		}
		
	}
}

void * thread_function(void * arg){
	struct t_arg *data;
	DIR* dp;
	char path[MAXC];
	
	data = (struct t_arg *)arg;

	// visit each element of the directory recursively
	printf("Thread %ld is visiting folder %s\n",pthread_self(), data->dir_name);
	visit(data->dir_name, pthread_self());
}

int main(int argc, char* argv[]){
	
	int i,j;
	pthread_t *arr_threads = (pthread_t *)malloc((argc-1)*sizeof(pthread_t));
	struct t_arg *threads_args = (struct t_arg *)malloc((argc-1) * sizeof(struct t_arg));
	void * status;
	
	// create and launch one thread for each command line argument
	for(i=1,j=i-1; i<argc; i++,j++){
		strcpy(threads_args[j].dir_name, argv[i]);
		printf("Creating thread #%d\n", i);
		pthread_create(&arr_threads[j], NULL, thread_function, (void *)&threads_args[j]);
	}
	
	// join the threads
	for(j=0; j<argc-1; j++){
		pthread_join(arr_threads[j], &status);
	}
	
	return 0;
}
