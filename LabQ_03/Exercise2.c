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
  char mark;
};
struct thread_args{
  char* file;
  size_t size;
};

void * thread_function1(void * args){
  struct thread_args *targ;
  int i;
  struct person_t p;
  long int target_field;
  //char * line = (char *)sizeof((int)targ->size * sizeof(char)); doesn't work
  char line[MAX];
  char c, next;

  targ = (struct thread_args *)args;

  for(i=0;i<targ->size;i++){ // i have to add an external for to iterate over lines
    c = targ->file[i];
    if(c=='\n'){ // new line
      line[i]='\0';
      sscanf(line,"%d %ld %s %s %d", &p.ID, &p.mat_num, p.name, p.surname, &p.mark);
      p.mat_num++;
      // Idea: convert the modified struct into a string and the copy all the characters
      // inside the file array. 
      
    }else{
      line[i] = c;
    }
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
  fdin = open(argv[1], O_RDONLY);
  fstat(fdin, &sbuf);
  if((src_void = mmap(0,sbuf.st_size,PROT_READ,MAP_SHARED,fdin,0)) == MAP_FAILED){
     printf("error");
     exit(0);
  }
  char* src_char = (char *)src_void; // cast to char pointer (array of char)
  printf("Content of the file: \n");
  for(i=0;i<sbuf.st_size;i++){
    printf("%c",*(src_char+i));
  }

  /* Create the threads */
  targ.file = src_char;
  targ.size = sbuf.st_size;
  pthread_create(&tids[0], NULL, thread_function1, (void *)&targ);
  pthread_create(&tids[1], NULL, thread_function2, (void *)&targ);

  pthread_join(tids[0], &status);
  pthread_join(tids[1], &status);
  

  return 0;
}
