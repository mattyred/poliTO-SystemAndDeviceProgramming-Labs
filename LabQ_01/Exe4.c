#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main() {
	int n,h,i,j;
	pid_t pid;
	printf("Insert the value n: ");
	scanf("%d", &n);
	printf("\nInsert the value of h: ");
	scanf("%d", &h);
	
	for(i=0; i<h; i++){ // depth of the tree
		
		for(j=0; j<n; j++){
			pid = fork();
			if(pid==0) // if i'm the child exit from loop
				break;	
		}
		
		if(pid){ // after having generated the n chilldren, terminate
			return(1);
		}
			
	}
	printf("LEAF=%d, PARENT=%d\n",getpid(),getppid());
}


