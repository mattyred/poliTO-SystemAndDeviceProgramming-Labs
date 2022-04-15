#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char* argv[]){
	
	int fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0777); // output
    FILE * fp = fopen(argv[1], "r"); // input
	int nR;
	int n, i;
    int * v;

    fscanf(fp, "%d", &n);
    v = (int *)malloc((n+1) * sizeof(int));
    v[0] = n;
    for(i=1;i<n+1;i++){
        fscanf(fp,"%d",&v[i]);
    }
    write(fd, v, (n+1)*sizeof(int));

}
