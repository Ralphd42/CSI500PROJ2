#include "producer.h"

pthread_mutex_t mutcharQueue    = PTHREAD_MUTEX_INITIALIZER;  // mutex for character Queue
pthread_mutex_t muttoUpperQueue = PTHREAD_MUTEX_INITIALIZER;  // mutex for toUpperQueue
pthread_mutex_t MmutwriterQueue = PTHREAD_MUTEX_INITIALIZER;  // mutex for writer Queue





/* reads full file

    writes to character queue
    maintains 
*/
void *readerThread( void *arg)
{
    cqp1 =0;  
    cqp2 =0;
    printf("\n1\n");
    bool first =true;
    FileData * fd = (FileData *) arg;
    printf( "\ndesc %s\n", fd->FileNameDesc);
    FILE* fp = fopen(fd->FileNameDesc, "rb");
    int chr; 
    do 
    {
        //printf("\n@\n");
        pthread_mutex_lock(&mutcharQueue);
        if( cqp1 >= QueueLen )
        {
            cqp1=0;
        }
        if( cqp1 ==cqp2 && !first)
        {
           // printf("\n*\n");
            pthread_mutex_unlock(&mutcharQueue);
             usleep(1);
        }else
        {
            chr =getc(fp);
            if( chr!=EOF)
            {
                //printf("\n!\n");
                charQueue[cqp1++]=chr;
            }else
            {
                //printf("\n<>\n");
                running=false;
            }
            //printf("\n-\n");
            pthread_mutex_unlock(&mutcharQueue);
            usleep(1);
            first =false;
        }
        
        //pthread_t charTh; 
        //pthread_create(&charTh,NULL,characterThread ,(void *) NULL );
        
            
    }
    while (running);
    printf("\nDONE\n");
     pthread_exit(NULL);
}

/*
    The character thread component will scan the line and replace each
	 blank character by the character supplied by the client. It will then pass the line to the toUpper thread through another queue of messages.

    needed vars
    cqp1 =0;  
    cqp2 =0;

    int tqp1,  tqp2;  

*/

void *characterThread( void *arg)
{
    bool first;
    char * p = (char *)arg;
    cqp2 =0;
    bool hasdata =false;
    while (running || hasdata)
    {
        pthread_mutex_lock(&mutcharQueue  );
        if( (cqp2-1) !=cqp1)
        {
            char nv =charQueue[cqp2];     
            if(charQueue[cqp2] ==' '   )
            {
                nv =*p;

            }
            //printf("%C",charQueue[cqp2]);
            charQueue[cqp2]='\0';
            pthread_mutex_lock(&muttoUpperQueue);
            if( tqp1>=QueueLen)
            {
                tqp1=0;
            }
            toUpperQueue[tqp1] =nv;
            pthread_mutex_unlock(&muttoUpperQueue);
            cqp2++;
            if( cqp2>=QueueLen)
            {
                cqp2=0;
            }
            pthread_mutex_unlock(&mutcharQueue);
        }else
        {
            pthread_mutex_unlock(&mutcharQueue);
            hasdata=false;
        }
        
    }
    pthread_exit(NULL);
}

void *toupperThread( void *arg)
{
     bool hasdata=true;
    while(hasdata || running)
    {
      
    
    pthread_mutex_lock(&muttoUpperQueue);
        pthread_mutex_lock(&writerQueue);

         pthread_mutex_unlock(&writerQueue);
    pthread_mutex_unlock(&muttoUpperQueue);


    }

    pthread_exit(NULL);
}



/*
    writes output to file
    Takes string as input 
    reads from writerQueue 
    writes to file

    char writerQueue [QueueLen];  // third queue to upper will write here and writer will read here
int wqp1,  wqp2;
*/
void *writerThread( void *arg)
{
    char * fname = (char*) arg;
    bool hasdata=true;
    FILE* fp = fopen(fname, "rb");
    char towrite;
    while(hasdata || running)
    {
        pthread_mutex_lock(&writerQueue);
        if( (wqp2-1) !=wqp1)
        {   
            towrite = writerQueue[wqp2];
            writerQueue[wqp2]='\0';
            wqp2++;
            if( wqp2>=QueueLen)
            {
                wqp2=0;
            }
            pthread_mutex_unlock(&writerQueue);// release before the IO call
            fputc(towrite, fp);
            hasdata=true;
        }else
        {
            pthread_mutex_unlock(&writerQueue);
            hasdata=false;
        }
        
        
    }
    // close the file
    fclose(fp);
    pthread_exit(NULL);
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