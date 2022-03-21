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

/*
 * Locks the region of the file descriptor fd 
 * Use cmd: F_SETLK
 * |	|
 * |	|
 * |	|
 * |    | <- whence + offset(start) = staring point of the lock
 * 
 * To unlock a region we can call lock_region passing F_UNLK to cmd
 */
int lock_region(int fd, int cmd, int type, off_t offset, int whence, off_t len){
	
		struct flock lock;
		
		lock.l_type = type; // shared read lock: F_RDLCK, exclusive write: F_WRCLK, unlock: F:UNLCK
		lock.l_start = offset; // relative to whence
		lock.l_whence = whence; // SEEK_SET, SEEK_CUR, SEEK_END
		lock.l_len = len; // in bytes, 0 means lock to EOF
		
		return(fcntl(fd, cmd, &lock));
}

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

			// Seek the row with ID==n
			lseek(fd, offset, SEEK_SET);
			
			// Lock the line before reading
			if(lock_region(fd, F_SETLK, F_RDLCK, offset, SEEK_SET, sizeof(struct person_t)) == -1){ // Go to SEEK_SET + offset
																									// and locks sizeof(struct person_t) bytes
				printf("Error on locking");
			}
			  
			nR = read(fd, &p, sizeof(struct person_t));
			
			// Unlock the read line
			if(lock_region(fd, F_SETLK, F_UNLCK, offset, SEEK_SET, sizeof(struct person_t)) == -1){
				printf("Error on unlocking");
			}
			
			// Print out the results
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
