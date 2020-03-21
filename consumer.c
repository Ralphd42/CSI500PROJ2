#include "consumer.h"
#include "producer.h"



/*
    this needs to do the following
    1 get item 1 at a time from file
    2 redirect std output to a pipe 
    3 fork  
    4 set child to
*/
void processFile(char * fname)
{
    printf("\nfname %s\n",fname);
    FILE* fp = fopen(fname, "rb");
    //ssize_t	getline (char **, size_t *, FILE *);
    ssize_t read ;
    size_t len;
    char * line;
    while ((read = getline(&line, &len, fp)) != -1) 
    {
        //fork pipe write to input
        int pipeStat =0;
        int fd[2];
        pipeStat = pipe(fd);
        if( pipeStat<0)
        {
            perror("Error creating pipe");
            exit(EXIT_FAILURE);
        }
        pid_t pid;
        pid = fork();
        if(pid<0)
        {
            perror("Error creating FORK");
            exit(EXIT_FAILURE);
        }
        if( pid>0)
        {
            printf("\nPIPE\n");
            pipeouput(fd);
            int status;
            printf("./input.txt\n\n");
            pid = wait(&status);



        }
        if( pid==0)
        {
            ReadPipe( fd);
            
            FileData Fd;
            char input[20];
            scanf("%s",input);
            Fd.toreplace ='c';
            Fd.FileNameDesc =input;
            running=true;
            pthread_create(&ReaderThrd,NULL,readerThread ,(void *) &Fd );

            
            void * status;
            pthread_join(ReaderThrd,status  );
             
            exit(EXIT_SUCCESS);
        }
    }
}

void pipeouput (int fd[2])
{
    close (fd[0]);
    int fdesc =dup2(fd[1], STDOUT_FILENO);
    if(fdesc<0)
    {
        perror("pipeouput");
        fprintf(stderr, "pipeouput %d", fdesc);
        exit(EXIT_FAILURE);
    }
}

void ReadPipe( int fd[2])
{
    close(fd[1]);
    int d =dup2(fd[0], STDIN_FILENO);
    if( d<0)
    {
        perror("Failed to read Pipe");
        fprintf(stderr,"Failed to dup = %d\n\n",d);
        exit(1);
    }
    printf("Open for Read\n");
}
