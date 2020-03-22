#include "producer.h"

pthread_mutex_t mutcharQueue    = PTHREAD_MUTEX_INITIALIZER;  // mutex for character Queue
pthread_mutex_t muttoUpperQueue = PTHREAD_MUTEX_INITIALIZER;  // mutex for toUpperQueue
pthread_mutex_t mutwriterQueue = PTHREAD_MUTEX_INITIALIZER;  // mutex for writer Queue





/* 
    readerThread
    FileData as inout paramter
    reads full file
    writes to character queue (charQueue)
     
*/
void *readerThread( void *arg)
{   
    readstat =true;
    cqp1 =0;  
    cqp2 =0;
    bool first =true;
    FileData * fd = (FileData *) arg;
    FILE* fp = fopen(fd->FileNameDesc, "rb");
    int chr; 
    do 
    {
        pthread_mutex_lock(&mutcharQueue);
        if( cqp1 >= QueueLen )
        {
            cqp1=0;
        }
        if( charQueue[cqp1] !='\0')
        {
            pthread_mutex_unlock(&mutcharQueue);
            sleep(1);
             
        }else
        {
            chr =getc(fp);
            if( chr!=EOF)
            {
                charQueue[cqp1++]=chr;
            }else
            {
                readstat=false;
            }
            
            if( first)
            {
                pthread_create(&ChrThrd,NULL,characterThread ,(void *) & fd->toreplace );
            }
            pthread_mutex_unlock(&mutcharQueue);
            sleep(1);
            first =false;
        }
    }
    while (readstat);
    
    readstat=false;
    fclose(fp);
    void * status;
    // join and wait for characterThread to finish
    pthread_join(ChrThrd, status);
    pthread_exit(NULL);
}

/*
    characterThread

    reads from charQueue
    replaces spaces with space replacement character
    writes new character to toUpperQueue  
*/
void *characterThread( void *arg)
{
    charStat=true;
    bool first =true;
    char * p = (char *)arg;// replacement character
    cqp2 =0;
    bool hasdata =true;
    while (readstat || hasdata)
    {
        hasdata =true;
        if(charQueue[cqp2] != '\0'  && toUpperQueue[tqp1]=='\0' )
        {
            pthread_mutex_lock(&mutcharQueue  );
            char nv =charQueue[cqp2];
            if(charQueue[cqp2] == ' ' )
            {
                nv =*p;
            }
            charQueue[cqp2]='\0';
            pthread_mutex_lock(&muttoUpperQueue); // wait until upperqueue is available
            if( tqp1>=QueueLen)
            {
                tqp1=0;
            }
            toUpperQueue[tqp1++] =nv;
            if( first)
            {
                // kick off upper on first
                pthread_create(&UpprThrd,NULL,toupperThread ,(void *) NULL );
                sleep(1);
                first =false;    
            }
            pthread_mutex_unlock(&muttoUpperQueue);
            cqp2++;
            if( cqp2>=QueueLen)
            {
                cqp2=0;
            }
            pthread_mutex_unlock(&mutcharQueue);
            sleep(1);
        }else
        {
            sleep(1);
            hasdata=false;
        }
    }
    
    charStat =false;
    void * status;
    pthread_join(UpprThrd, status);
    pthread_exit(NULL);
}

/*
    this thread will read from the 
    toUpperQueue 
    it will write characters to the writer queue.
    
*/
void *toupperThread( void *arg)
{
    bool hasdata=true;
    bool first =true;
    upprStat =true;
    wqp1 =0;
    while(hasdata || charStat)
    {
        pthread_mutex_lock(&muttoUpperQueue);
       // if( (tqp2-1) !=tqp1)
        if(toUpperQueue[tqp2]!='\0'    &&  writerQueue[wqp1]=='\0')
        {
            char nv =toUpperQueue[tqp2];     
            nv = toupper(nv);
            toUpperQueue[tqp2]='\0';
            printf( "%c",nv );
            fflush(stdout);
            if(true){
            pthread_mutex_lock(&mutwriterQueue);
            writerQueue[wqp1++] =nv;
            if(wqp1>=QueueLen)
            {
                wqp1 =0;
            }
            
            if( first)
            {
                // kick off upper on first
                char * outfile ="./ThOut.txt";
                pthread_create(&WriterThrd,NULL,writerThread ,(void *) outfile );
                first =false;
                sleep(1);
                
            }
            
            
            pthread_mutex_unlock(&mutwriterQueue);
            sleep(1);
            }   
            tqp2++;
            
            if( tqp2>=QueueLen)
            {
                tqp2=0;
            }
            pthread_mutex_unlock(&muttoUpperQueue);
            sleep(1);
        }
        else
        {
            pthread_mutex_unlock(&muttoUpperQueue);
            sleep(1);
            hasdata=false;
        }
        


    }
    void * status;
    upprStat=false;
    pthread_join(WriterThrd, status);
    pthread_exit(NULL);
}



/*
    writes output to file
    Takes string as input 
    reads from writerQueue 
    writes to file
*/
void *writerThread( void *arg)
{
    char * fname = (char*) arg;
    bool hasdata=true;
    FILE* fp = fopen(fname, "w");
    char towrite;
    wqp2 =0;
    while(hasdata || upprStat)
    {
        
        pthread_mutex_lock(&mutwriterQueue);
       // if( (wqp2-1) !=wqp1)
        if(writerQueue[wqp2]!='\0' )
        {   
            printf(" -w has data -");
            towrite = writerQueue[wqp2];
            writerQueue[wqp2]='\0';
            wqp2++;
            if( wqp2>=QueueLen)
            {
                wqp2=0;
            }
            pthread_mutex_unlock(&mutwriterQueue);// release before the IO call
            sleep(1);
            fputc(towrite, fp);
            printf("W %c", towrite);
            hasdata=true;
        }else
        {
            pthread_mutex_unlock(&mutwriterQueue);
            sleep(1);
            hasdata=false;
        }
        
        
    }
    // close the file
    fclose(fp);
    printf("END WRITER");
    pthread_exit(NULL);

}




/*
    cleanQueues()
    empties character queues
*/
void cleanQueues()
{

    for (int i=0;i< QueueLen ;i++)
    {
        charQueue[i] ='\0';
        toUpperQueue[i] ='\0';
        writerQueue[i] ='\0';
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
        //pthread_join(charTh,&status);

    }

    pthread_exit(NULL);
}