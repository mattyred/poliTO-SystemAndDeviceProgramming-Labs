#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <signal.h>

void handler(int signo);
int n1=0; // number of SIGUSR1 in a row
int n2=0;
int previous = 0;
int main(){
	
	// Instantiate the signal handlers
	signal(SIGUSR1, handler);
	signal(SIGUSR2, handler);
	
	while(1); // wait for signals
}

void handler(int signo){
	
	switch(signo){
		case SIGUSR1:
			printf("## SIGUSR1 received ##\n");
			n1++;
			if(previous == 1 && n1 != 3)
				printf("error\n");
			else if(previous == 2){
				printf("success\n");
				n1 = 0;
			}else if(n1 == 3){
				exit(1);
			}	
			previous = 1;	
		break;
		case SIGUSR2:
			printf("## SIGUSR2 received ##\n");
			n2++;
			if(previous == 2 && n2 != 3)
				printf("error\n");
			else if(previous == 1){
				printf("success\n");
				n2 = 0;
			}else if(n2 == 3){
				exit(1);
			}	
			previous = 2;
		break;
	}
	
	
	
}
