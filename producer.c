#include "producer.h"

pthread_mutex_t mutcharQueue    = PTHREAD_MUTEX_INITIALIZER;  // mutex for character Queue
pthread_mutex_t muttoUpperQueue = PTHREAD_MUTEX_INITIALIZER;// mutex for toUpperQueue
pthread_mutex_t MmutwriterQueue = PTHREAD_MUTEX_INITIALIZER;






void *readerThread_old( void *arg)
{
     
    printf("\n1\n");

    FileData * fd = (FileData *) arg;
    printf( "\ndesc %s\n", fd->FileNameDesc);
    FILE* fp = fopen(fd->FileNameDesc, "rb");
    int chr =getc(fp);;
    while (chr !=EOF)
    {
        pthread_mutex_lock(&mutcharQueue);



        pthread_mutex_unlock(&mutcharQueue);
        chr =getc(fp);    
    }

}








/*
    1 take a file and a character as input
    void * arg will hold a pointer to FileData
    read file line by line
*/
void *readerThread_old( void *arg)
{
     
    printf("\n1\n");

    FileData * fd = (FileData *) arg;
    printf( "\ndesc %s\n", fd->FileNameDesc);
    FILE* fp = fopen(fd->FileNameDesc, "rb");
    //ssize_t	getline (char **, size_t *, FILE *);
    ssize_t read ;
    size_t len;
    char * line;
    while ((read = getline(&line, &len, fp)) != -1) 
    {
        pthread_mutex_lock(&mutcharQueue);
        strncat(charQueue,line, 10 );
        pthread_t charTh; 
        pthread_create(&charTh,NULL,characterThread ,(void *) NULL );





        pthread_mutex_unlock(&mutcharQueue);
        void * status;
        pthread_join(charTh,&status);

    }

    pthread_exit(NULL);
}

void *characterThread( void *arg)
{
    pthread_mutex_lock(&mutcharQueue);


    printf("\n%s\n", charQueue  );
    pthread_mutex_unlock(&mutcharQueue);
    pthread_exit(NULL);
}

void *toupperThread( void *arg)
{
    pthread_exit(NULL);
}

void *writerThread( void *arg)
{
    pthread_exit(NULL);
}