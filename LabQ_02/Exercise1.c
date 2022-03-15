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
	int fd_binary_file, nR, nW, i=0;
	char buf[BUFFSIZE];
	FILE *fp_binary_file, *fp_out_text_file, *fp_file1;

	fp_file1 = fopen(argv[1], "r"); // Open file1 as a text file -> fp_file1
	
	fd_binary_file = open (argv[2], 
							O_WRONLY | O_CREAT | O_TRUNC, 
							S_IRUSR | S_IWUSR);// Open file2 as a binary file -> fp_binary_file
	struct person_t p;
	while (fscanf(fp_file1, "%d %ld %s %s %d", &p.ID, 
									&p.register_number,
									p.surname,
									p.name,
									&p.mark) != EOF){
		// printf("%d %ld %s %s %d\n",p.ID,p.register_number,p.surname,p.name,p.mark);							
		nW = write (fd_binary_file, &p, sizeof(struct person_t));								
	}
	
	close(fd_binary_file);
	fd_binary_file = open (argv[2], O_RDONLY); // Open file2 as a binary file -> fd_binary_file

	fp_out_text_file = fopen(argv[3], "r"); // Open file3 as a text file  -> fp_out_text_file
	printf("Content of the binary file: \n");

	while ( (nR = read (fd_binary_file, buf, BUFFSIZE)) > 0 ) {
		printf("%c",buf[i++]);
	}
	

	return 0;
		
} 
