#include "producer.h"

pthread_mutex_t mutcharQueue    = PTHREAD_MUTEX_INITIALIZER;  // mutex for character Queue
pthread_mutex_t muttoUpperQueue = PTHREAD_MUTEX_INITIALIZER;// mutex for toUpperQueue
pthread_mutex_t MmutwriterQueue = PTHREAD_MUTEX_INITIALIZER;





/* reads full file

    writes to character queue
    maintains 
*/
void *readerThread( void *arg)
{
    cqp1 =0;  
    cqp2 =0;
    printf("\n1\n");
    bool first =false;
    FileData * fd = (FileData *) arg;
    printf( "\ndesc %s\n", fd->FileNameDesc);
    FILE* fp = fopen(fd->FileNameDesc, "rb");
    int chr; 
    do 
    {
        pthread_mutex_lock(&mutcharQueue);
        if( cqp1 >= QueueLen )
        {
            cqp1=0;
        }
        if( cqp1 ==cqp2 && !first)
        {
            pthread_mutex_unlock(&mutcharQueue);
        }else
        {
            chr =getc(fp);
            if( chr!=EOF)
            {
                charQueue[cqp1++]=chr;
            }else
            {
                running=false;
            }
        }
        first =false;
        //pthread_t charTh; 
        //pthread_create(&charTh,NULL,characterThread ,(void *) NULL );
        pthread_mutex_unlock(&mutcharQueue);
            
    }
    while (running);
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

/*
    The character thread component will scan the line and replace each
	 blank character by the character supplied by the client. It will then pass the line to the toUpper thread through another queue of messages.

    needed vars
    cqp1 =0;  
    cqp2 =0;



*/

void *characterThread( void *arg)
{
    cqp2 =0;
    bool hasdata =false;
    while (running || hasdata)
    {
        pthread_mutex_lock(&mutcharQueue  );
        if( cqp2!=cqp1)
        {
            printf("%c",charQueue[cqp2]);    
            cqp2++;
            if( cqp2>=QueueLen)
            {
                cqp2=0;
            }


        }else
        {
            hasdata=false;
        }
        pthread_mutex_unlock(&mutcharQueue);





    }



    
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