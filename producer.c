#include "producer.h"




/*
    1 take a file and a character as input
    void * arg will hold a pointer to FileData
    read file line by line
*/
void *readerThread( void *arg)
{
     


    FileData * fd = (FileData *) arg;
    FILE* fp = fopen(fd->FileNameDesc, "rb");
    //ssize_t	getline (char **, size_t *, FILE *);
    ssize_t read ;
    size_t len;
    char * line;
    while ((read = getline(&line, &len, fp)) != -1) 
    {
        pthread_mutex_lock(&mutcharQueue);
        

        pthread_mutex_unlock(&mutcharQueue);

    }

    pthread_exit(NULL);
}

void *characterThread( void *arg)
{
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