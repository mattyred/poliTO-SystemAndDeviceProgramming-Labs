#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define COPYINCR (1024*1024*1024)

struct person_t{
  int ID;
  long int register_number;
  char surname[100];
  char name[100];
  int mark;
}
int main(int argc, char* argv[]){
  int fdin, fdout;
  void *src, *dst;
  size_t copysz;
  struct stat sbuf;
  off_t fsz;
  int i;

  fdin = open(argv[1], O_RDONLY);
  fstat(fdin, &sbuf);

  
  if((src = mmap(0,sbuf.st_size,PROT_READ,MAP_SHARED,fdin,0)) == MAP_FAILED){
     printf("error");
     exit(0);
  }

  sscanf(src, "%d %ld %s %s %d", 

  
  

  
  

  return 0;
}
