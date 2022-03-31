#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <pthread.h>

#define MAX 100

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

void increment_field_line(char * file,int offset,int lastindex,int field,char * line){
  struct person_t p;
  int target_field;
  sscanf(line,"%d %ld %s %s %d", &p.ID,&p.mat_number,p.name,p.surname,&p.mark);

  if(field==1){ // first thread
    target_field = p.mat_number+1;
    sprintf(line,"%d %ld %s %s %d",p.ID,target_field,p.name,p.surname,p.mark);
  }else{ // second thread
    target_field = p.mark+1;
    sprintf(line,"%d %ld %s %s %d",p.ID,p.mat_number,p.name,p.surname,target_field);
  }
  sprintf(file+offset,"%s", line);
  *(file+offset+lastindex)='\n';
}
void * thread_function1(void * args){
  struct thread_args *targ;
  int i,j,k,start;
  struct person_t p;
  long int target_field;
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
        increment_field_line(src,start,k,1,line);
	k=0;
	start = j+1;
      }else{
	line[k++] = c;
      }
  }
}

void * thread_function2(void * args){
  struct thread_args *targ;
  int j,i,start,length,target_field,k;
  char c;
  char * src;
  char line[MAX];
  struct person_t p;
  targ = (struct thread_args *)args;
  src = (char *)targ->file;
 
  for(j=targ->size-2;j>=0;j--){
    c = src[j];
    if(c=='\n' || j==0){ // Go back till i find \n or beggining of file
      start = (c=='\n')?j+1:j;

      // Copy the content of the line inside a string
      for(i=0,k=start;;k++){
	if(src[k]=='\n' || src[k]==EOF){ // go on till find \n or the file is finished
	  line[i]='\0';
	  break;
	}
	line[i++] = src[k];
      }

      // Modify the content of the line 
      increment_field_line(src,start,i,2,line);
    }

  }
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

  /* Create the threads */
  targ.file = src_void;
  targ.size = sbuf.st_size;
  pthread_create(&tids[0], NULL, thread_function1, (void *)&targ);
  pthread_create(&tids[1], NULL, thread_function2, (void *)&targ);

  pthread_join(tids[0], &status);
  pthread_join(tids[1], &status);
  

  return 0;
}
