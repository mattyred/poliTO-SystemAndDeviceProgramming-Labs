#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <pthread.h>

#define MAX 50

struct person_t{
  int ID;
  long int mat_number;
  char name[MAX];
  char surname[MAX];
  int mark;
};
struct thread_args{
  void* file;
  size_t size;
};

void * thread_function1(void * args){
  struct thread_args *targ;
  int i,j,k,start;
  struct person_t p;
  long int target_field;
  //char * line = (char *)sizeof((int)targ->size * sizeof(char)); doesn't work
  char line[MAX];
  char c, next;
  char* src;
  targ = (struct thread_args *)args;
  src = (char *)targ->file;
  k=0;
  start=0;
  for(j=0;j<targ->size;j++){
      c = src[j];
      if(c=='\n'){ // new line
	line[k]='\0';
	sscanf(line,"%d %ld %s %s %d", &p.ID,&p.mat_number,p.name,p.surname,&p.mark);
	target_field = p.mat_number + 1;
	printf("%ld\n",target_field);
	sprintf(src+start,"%d %ld %s %s %d", p.ID, target_field, p.name, p.surname, p.mark);
	k=0;
	start = j+1;
      }else{
	line[k++] = c;
      }
  }

   printf("Content of the file[MOD BY T1]: \n");
   for(i=0;i<targ->size;i++){
     printf("%c",*(src+i));
   }
}

void * thread_function2(void * args){
  struct thread_args *targ;
  int i;

  targ = (struct thread_args *)args;

  

}

int main(int argc, char* argv[]){
  int fdin, fdout;
  void *src_void, *dst;
  size_t copysz;
  struct stat sbuf;
  off_t fsz;
  int i;
  pthread_t tids[2];
  struct thread_args targ;
  void * status;

  /* Memory mapping the file */
  fdin = open(argv[1], O_RDWR);
  fstat(fdin, &sbuf);
  if((src_void = mmap(0,sbuf.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fdin,0)) == MAP_FAILED){
     printf("error");
     exit(0);
  }
  char* src_char = (char *)src_void; // cast to char pointer (array of char)
  printf("Content of the file: \n");
  for(i=0;i<sbuf.st_size;i++){
    printf("%c",*(src_char+i));
  }

  /* Create the threads */
  targ.file = src_void;
  targ.size = sbuf.st_size;
  pthread_create(&tids[0], NULL, thread_function1, (void *)&targ);
  pthread_create(&tids[1], NULL, thread_function2, (void *)&targ);

  pthread_join(tids[0], &status);
  pthread_join(tids[1], &status);
  

  return 0;
}
