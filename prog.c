#include     <stdio.h>
#include    <unistd.h>
#include  <sys/stat.h> 
#include    <stdlib.h>
#include  <sys/wait.h>
#include  "consumer.h"
#include "producer.h"
int main(int argc, char *argv[])
{
    char filename []="./input.txt";
    int status;
    int fd [2];
    int pipeStat =0;
    pid_t pid;
    pipeStat = pipe(fd);
    if( pipeStat<0)
    {
        perror("Error creating pipe");
        exit(EXIT_FAILURE);
    }
    pid = fork();
    if( pid>0)
    {
        // current set
        close (fd[0]);
        int fdesc =dup2(fd[1], STDOUT_FILENO);
        if(fdesc<0)
        {
            fprintf(stderr, "C1 Failed to DUP");
            exit(EXIT_FAILURE);
        }
        //write to stdout
        pid = wait(&status);




    }
    if(pid==0)
    {




        
    }
    if(pid<0)
    {
        
        exit(EXIT_FAILURE);
    }





}