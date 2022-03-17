#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXC 31
#define BYTELENGTH 47

struct person_t{
	int id;
	long int  reg_num;
	char surname[MAXC];
	char name[MAXC];
	int mark;
};
int main(int argc, char* argv[]){
	
	char option;
	int n;
	
	int fd;
	int nR, nW;
	struct person_t p;
	
	
	fd = open(argv[1], O_RDWR, S_IRUSR|S_IWUSR);

	
	while(1){
		printf("Insert an option[R,W,E]: ");
		scanf("%c",&option);
		if(option == 'R'){
			printf("Insert a value n: ");
			scanf("%d", &n);

			// Compute the offset
			off_t offset = (n-1) * sizeof(struct person_t);

			// Read from the file the student with ID==n
			lseek(fd, offset, SEEK_SET);
			nR = read(fd, &p, sizeof(struct person_t));
			printf("Row with index %d: ", n);
			printf("%d %ld %s %s %d\n",p.id, p.reg_num, p.surname, p.name, p.mark); 
			
		}else if(option == 'W'){
			printf("Insert a value n: ");
			scanf("%d", &n);
			printf("Insert the data of a person: ");
			scanf("%d %ld %s %s %d", &p.id, &p.reg_num, p.surname, p.name, &p.mark);
			//printf("You have inserted: %d %ld %s %s %d", p.id, p.reg_num, p.surname, p.name, p.mark);
			
			// Move the curson on the line with index n
			off_t offset = (n-1) * sizeof(struct person_t);
			lseek(fd, offset, SEEK_SET);
			
			// Write the new record
			nW = write(fd, &p, sizeof(struct person_t));
			
		}else if(option == 'E'){
			
				
			close(fd);	
			exit(1);
		}
		
	}
	
}
