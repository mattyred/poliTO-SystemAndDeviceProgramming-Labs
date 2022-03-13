#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char* argv[]){
	
	int n, *vet, i;
	n = atoi(argv[1]);
	vet = (int * )malloc(n * sizeof(int));
	
	for(i=0; i<n; i++){
		if(fork()){ // parent
			vet[i] = 0;
		}else{ // child
			vet[i] = 1;
		}
	}
	
	printf("vet = [");
	for(i=0; i<n; i++){
		printf("%d ",vet[i]);
	}
	printf("]\n");
	
	
}
