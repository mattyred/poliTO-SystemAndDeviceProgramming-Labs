#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>


#define MAXC 31

void visit(DIR* dp1, DIR* dp2, char* path_dir1, char* path_dir2){
	struct dirent *dirp1, *dirp2;
	char path_dir1_obj[MAXC], obj[MAXC], path_dir2_obj[MAXC];
	struct stat buf;
	int fd;

	// open the directory dir_1
	if((dp1 = opendir(path_dir1)) == NULL){
		printf("Error in opening %s\n", path_dir1);
		exit(1);
	}

			
	// visit each element
	while((dirp1 = readdir(dp1)) != NULL){
		sprintf(obj,"%s",dirp1->d_name); // current object inside dp1
		
		if((strcmp(obj,".") != 0) && (strcmp(obj, "..") != 0)){ // if the current object is . or .. skip
			// build the full name: path_dir1/object for visiting dir_1
			sprintf(path_dir1_obj,"%s/%s",path_dir1,dirp1->d_name);
			
			// build the full name for dir_2
			sprintf(path_dir2_obj,"%s/%s",path_dir2,dirp1->d_name);
			
			// obtain information about path_dir1/object in dir_1
			lstat(path_dir1_obj, &buf);
			
			if(S_ISDIR(buf.st_mode) != 0){ // path_dir1/object is a directory		
					
				printf("Current name D: '%s'\n", obj);
				
				// create the directory inside the path of dir_2
				mkdir(path_dir2_obj, 0777); // create the dir with full privileges
				
				// visit the directory 'path_dir1/object' recursively
				visit(dp1, dp2, path_dir1_obj, path_dir2_obj);
				
			}else{ // it is a file, continue iterating but inside path_dir1/
				printf("Current name F: '%s'\n", obj);
				
				// create the same file inside dir_2
				fd = open(path_dir2_obj, O_RDWR | O_CREAT | O_TRUNC, 0777);
				
				// appen two lines at the beginning
				off_t size_of_file = buf.st_size;

				write(fd, obj, sizeof(obj));
				//write(fd, " ", sizeof(char *));
				write(fd, &size_of_file, sizeof(off_t));
			}
		}else{ // directory . or ..
			continue;	
		}
	}
	
	return;
}


int main(int argc, char* argv[]){
	
	char dir_1[MAXC], dir_2[MAXC];
	strcpy(dir_1, argv[1]);
	strcpy(dir_2, argv[2]);
	
	struct stat buf;
	DIR *dp1,*dp2;
	
	lstat(dir_1, &buf); // get information about dir_1
	if(S_ISDIR(buf.st_mode) == 0){
		printf("Error, %s is not a directory!", argv[1]);
		exit(1);
	}
	
	// open dir_1
	if((dp1 = opendir(dir_1)) == NULL){
		printf("Error in opening(main) %s", argv[1]);
		exit(1);
	}
	
	// open dir_2
	if((dp2 = opendir(dir_2)) == NULL){
		printf("Error in opening(main) %s", argv[2]);
		exit(1);
	}
	
	printf("Current name D: '%s'\n", dir_1);
	visit(dp1, dp2, dir_1, dir_2);
	return 0;
}
