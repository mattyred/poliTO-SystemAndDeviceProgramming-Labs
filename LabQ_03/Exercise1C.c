#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/select.h>

#define STR_SIZE 20
#define STR_NUM 4
#define WAIT_TIME_1 1
#define WAIT_TIME_2 1
/*
Multiplexing
*/

int main(int argc, char *argv[])
{
    int fd1[2];
    int fd2[2];
    pid_t child[2];
    time_t t;
    int i, j, nbytes, length;
    char str[STR_SIZE + 1];
    char random_char;
    fd_set read_set, write_set;
    srand((unsigned)time(&t));
    /*Pipe creation for C1*/
    pipe(fd1);

    /*Create first child*/
    if ((child[0] = fork()) == 0)
    {
        /*Child 1: waits WAIT_TIME_1*/
        close(fd1[0]);
        i = STR_NUM;
        while (i > 0)
        {
            sleep(WAIT_TIME_1);
            length = (int)(1 + rand() % STR_SIZE); // string with 0...STR_SIZE characters
            for (j = 0; j < length; j++)
            {
                random_char = (char)(97 + (rand() % 25)); // to generate a number between 97-122
                str[j] = random_char;
            }
            str[length] = '\0';
            /* Create the write set */

            nbytes = write(fd1[1], str, sizeof(str));
            printf("Written %d bytes by C1: %s\n", nbytes, str);
            i--;
        }
    }
    else
    {
        /*Parent*/

        /*Pipe creation for C2*/
        pipe(fd2);

        /*Create second child*/
        if ((child[1] = fork()) == 0)
        {
            /*Child 2*/

            close(fd2[0]);
            i = STR_NUM;
            while (i > 0)
            {
                sleep(WAIT_TIME_2);
                length = (int)rand() % STR_SIZE;
                for (j = 0; j < length; j++)
                {
                    random_char = (char)(97 + (rand() % 25)); // to generate a number between 97-122
                    str[j] = random_char;
                }
                str[length] = '\0';

                /* Writing section */
                nbytes = write(fd2[1], str, sizeof(str));

                printf("Written %d bytes by C2: %s\n", nbytes, str);
                i--;
            }
        }
        else
        {
            /*Parent after creation of C1 and C2*/
            
            /* Prepare the read set */
            int maxfd = ((fd1[0] > fd2[0]) ? fd1[0]:fd2[0]) + 1;
            int i1=0, i2=0;

            /*Read strings from C1 and C2*/
            while(i1<STR_NUM || i2<STR_NUM)
            {   
                FD_ZERO(&read_set);
                FD_SET(fd1[0], &read_set);
                FD_SET(fd2[0], &read_set);
                select(maxfd, &read_set, NULL, NULL, NULL);

                if(FD_ISSET(fd1[0], &read_set)){ // Able to read from child 1
                    nbytes = read(fd1[0], str, sizeof(str));

                    for (j = 0; j < STR_SIZE; j++)
                        str[j] = toupper(str[j]);

                    printf("[FROM C1] Received %d bytes  string: %s \n", nbytes, str);
                    i1++;
                }else if(FD_ISSET(fd2[0], &read_set)){ // Able to read from child 2
                    nbytes = read(fd2[0], str, sizeof(str));

                    for (j = 0; j < STR_SIZE; j++)
                        str[j] = toupper(str[j]);
                        
                    printf("[FROM C2] Received %d bytes  string: %s \n", nbytes, str);
                    i2++;
                }
            }
        }
    }

    return 0;
}
