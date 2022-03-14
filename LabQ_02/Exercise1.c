#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXC 31
#define BUFFSIZE 1000
struct person_t{
	int ID;
	long int register_number;
	char surname[MAXC];
	char name[MAXC];
	int mark;
};

int main(int argc, char* argv[]){
	int fd_file1, fd_file2, nR, nW, i=0;
	char buf[BUFFSIZE];
	FILE *fp_file1, *fp_file3;


	/* Open file with fopen() */
	fp_file1 = fopen(argv[1], "r");
	
	/* Read file with fscanf() and print on second file(binary)*/
	fd_file2 = open (argv[2], 
					 O_WRONLY | O_CREAT | O_TRUNC, 
					 S_IRUSR | S_IWUSR);
	struct person_t p;
	while (fscanf(fp_file1, "%d %ld %s %s %d", &p.ID, 
									&p.register_number,
									p.surname,
									p.name,
									&p.mark) != EOF){							
		nW = write (fd_file2, &p, sizeof(p));								
	}
	close(fd_file2);
	/* Read the written binary file */
	fd_file1 = open (argv[2], O_RDONLY);
	if(fd_file1 == -1){
		fprintf(stdout,"Error opening file\n");
		exit(1);
	}
	
	fp_file3 = fopen(argv[3], "r");
	printf("Content of the binary file: \n");
	while ( (nR = read (fd_file1, buf, BUFFSIZE)) > 0 ) {
		
	}
	
	close(fd_file1);
	return 0;
		
} 
