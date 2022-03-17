#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXC 31

/*
 * Given a binary file passed in argv[1] it
 * prints out its content as a textual file
 * by interpreting its lines as struct person_t
 * */
struct person_t{
	int id;
	long int  reg_num;
	char surname[MAXC];
	char name[MAXC];
	int mark;
};

int main(int argc, char* argv[]){
	
	int fd = open(argv[1], O_RDONLY);
	int nR;
	struct person_t p;
	while ( (nR = read (fd, &p, sizeof(struct person_t)) > 0) ) {
		fprintf(stdout,"%d %ld %s %s %d\n",p.id, p.reg_num, p.surname, p.name, p.mark);
	}

	
	
}
