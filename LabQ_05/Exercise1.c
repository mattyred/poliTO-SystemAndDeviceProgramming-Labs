#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <ctype.h>

#define SHM_SIZE 1024
void P1(char * data, int * fd_pipe, pid_t pid_P2);
void P2(char * data, int * fd_pipe, pid_t pid_P1);
char * shared_memory_init();
void shared_memory_detach(char* data);
int random_n();
char * random_text(int n);
void display_capital_text(char process, char * data, int n);
int main(int argc, char* argv[]){
    int fd_pipe[2];
    char * data;
    pid_t pid;

    // Initialize shared memory pointer
    data = shared_memory_init();

    // Initialized pipe
    pipe(fd_pipe);

    // Set random numbers seed
    srand(time(NULL));

    // Fork
    pid = fork();
    if(pid==0){
        P2(data, fd_pipe, getppid());
    }else{
        P1(data, fd_pipe, pid);
    }


    /* PROCESS READING AND WRITING
    if(argc==2){
        printf("Writing to segment: \"%s\"\n", argv[1]);
        strncpy(data, argv[1], SHM_SIZE);
    }else{
        printf("segment contains: \"%s\"\n", data);
    } */
    

    // Detach shared memory sement
    shared_memory_detach(data);
    return 0;

}

char * shared_memory_init(){
    key_t key;
    int shmid;
    char * data;

    // Create the key for using shared memory
    if((key = ftok("hello.txt", 5)) == -1){
        perror("ftok");
        exit(1);
    }

    // Obtain a shared mamemory segment
    if((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1){
        perror("shmget");
        exit(1);
    }

    // Store in 'data' the address of the shared memory segment
    data = shmat(shmid, NULL, 0);
    if(data == (char*)(-1)){
        perror("shmat");
        exit(1);
    }
    return data;
}
void shared_memory_detach(char * data){
    if(shmdt(data) == -1){
        perror("shmdt");
        exit(1);
    }
}

void P2(char * data, int * fd_pipe, pid_t pid_P1){
    int n;
    char * capital_text = (char*)malloc((n+1)*sizeof(char));
    char * text;
    int i;
    char letter;
    
    while(1){
        // Wait until P1 wakes up me
        read(fd_pipe[0], &n, sizeof(int));
        
        // Convert the text into capital letters
        display_capital_text('2', text, n);

        // Random text generation and awaking of P1
        n = random_n();
        if(n == 0){
            printf("I'm P2 and n=0 => terminate\n");
            kill(pid_P1, SIGINT);
            exit(1);
        }
        text = random_text(n);
        printf("I'm P2 and the random text is: [%s]\n", text);
        strncpy(data, data, SHM_SIZE);
        write(fd_pipe[1], &n, sizeof(int));
    }

}

void P1(char * data, int * fd_pipe, pid_t pid_P2){
    int n;
    char * text;

    while(1){
        // Generate random number n
        n = random_n();

        // Check the value of n
        if(n == 0){
            printf("I'm P1 and n=0 => terminate\n");
            kill(pid_P2, SIGINT);
            exit(1);
        }

        // Random text generation
        text = random_text(n);
        printf("I'm P1 and the random text is: [%s]\n", text);

        // Write the text on the shared memory
        strncpy(data, text, SHM_SIZE);

        // Awake P2 using the pipe
        write(fd_pipe[1], &n, sizeof(int));

        // Awaiting of P1 and capital text displaying
        read(fd_pipe[0], &n, sizeof(int));
            
        // Convert the text into capital letters
        display_capital_text('1', data, n);
    }
}

int random_n(){
    
    int n = rand() % 6; // between 0 and 5
    return n;
}

char * random_text(int n){
    int num_valid_letters = 'z'-'a'+1+2;
    char * valid_letters = (char *)malloc(num_valid_letters * sizeof(char));
    char * text = (char *)malloc((n+1)*sizeof(char));
    int i;
    int index;
    char j;

    for(i=0,j='a';i<num_valid_letters-2;i++){
        valid_letters[i] = j++;
    }
    valid_letters[num_valid_letters - 2] = '\n';
    valid_letters[num_valid_letters - 1] = ' ';

    for(i=0;i<n;i++){
        index = rand() % num_valid_letters;
        text[i] = valid_letters[index];
    }
    text[n] = '\0';
    return text;
}

void display_capital_text(char process, char * data, int n){
    char letter;
    int i;

    printf("I'm P%c and the text(capital) is: [", process);
    for(i=0;i<n;i++){
        letter = data[i];
        if(letter != '\n' && letter != ' '){
            letter = toupper(letter);
        }
        printf("%c",letter);
    }
    printf("]\n");
}