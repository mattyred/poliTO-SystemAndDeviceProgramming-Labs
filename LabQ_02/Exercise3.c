#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>


#define MAXC 31

void explore_object(char* full_name){
	struct stat buf;
	struct dirent *dirp;
	char current_dir_name[MAXC];
	DIR* dp;
	
	// Get information about the current element
	lstat(full_name, &buf);

	// Check if it is a file or a directory
	if(S_ISREG(buf.st_mode) != 0){ // it is a file, return
		printf("%s : It is a file\n",full_name);
		return;
	}else if(S_ISDIR(buf.st_mode) != 0){ // it is a directory, recur
		printf("%s : It is a directory\n", full_name);

		// Open the directory
		if((dp = opendir(full_name)) == NULL){
			printf("Error in opening %s\n", full_name);
			exit(1);
		}
			
		// visit each element and if it is a directory visit it recursively
		while((dirp = readdir(dp)) != NULL){
			sprintf(full_name,"%s/%s",full_name,dirp->d_name);
			sprintf(current_dir_name,"%s",dirp->d_name);
			
			if((strcmp(current_dir_name,".") != 0) && (strcmp(current_dir_name, "..") != 0)){ // it is a normal directory
				// printf("Current dir name: '%s' - %d\n", current_dir_name, strcmp(current_dir_name,".."));
				explore_object(full_name);
			}
		}	
		return;
	}
}

int main(int argc, char* argv[]){
	
	char dir_1[MAXC], dir_2[MAXC];
	char current_dir_name[MAXC];
	strcpy(dir_1, argv[1]);
	strcpy(dir_2, argv[2]);
	
	// Copy the tree rooted in dir_1 inside the tree rooted in dir_2
	// For each file copied it has to be put on top:
	// name of the file ; size of the file
	
	struct stat buf;
	DIR* dp;
	char full_name[100];
	struct dirent *dirp;
	
	lstat(dir_1, &buf); // get information about the first folder
	if(S_ISDIR(buf.st_mode) == 0){
		printf("Error, %s is not a directory!", argv[1]);
		exit(1);
	}
	if((dp = opendir(dir_1)) == NULL){
		printf("Error in opening %s", argv[1]);
		exit(1);
	}
	
	int i = 0;
	
	while((dirp = readdir(dp)) != NULL){
		sprintf(full_name,"%s/%s",dir_1,dirp->d_name);
		sprintf(current_dir_name, "%s", dirp->d_name);
		if((strcmp(current_dir_name,".") != 0) && (strcmp(current_dir_name, "..") != 0)){ // it is a normal directory or a file
			// printf("Current dir name: '%s' - %d\n", current_dir_name, strcmp(current_dir_name,".."));
			explore_object(full_name); // this function can be called either on a file or on a directory
		}
	}
	return 0;
}
