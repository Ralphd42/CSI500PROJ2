#include "producer.h"

pthread_mutex_t mutcharQueue    = PTHREAD_MUTEX_INITIALIZER;  // mutex for character Queue
pthread_mutex_t muttoUpperQueue = PTHREAD_MUTEX_INITIALIZER;  // mutex for toUpperQueue
pthread_mutex_t mutwriterQueue = PTHREAD_MUTEX_INITIALIZER;  // mutex for writer Queue





/* reads full file

    writes to character queue
    maintains 
*/
void *readerThread( void *arg)
{
    // initialize status and queue pointers
    running =true;
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
        if( cqp1 ==cqp2)
        {
            // wrote as many characters as possible.  Give back to character thread
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
            pthread_mutex_unlock(&mutcharQueue);
            if( first)
            {
                // start character thread
                pthread_create(&ChrThrd,NULL,characterThread ,(void *) NULL );
            }
            first =false;
        }
    }
    while (running);
    fclose(fp);
    void * status;
    // join and wait for characterThread to finish
    pthread_join(ChrThrd, status);
   
    pthread_exit(NULL);
}

/*
    The character thread component will scan the line and replace each
	 blank character by the character supplied by the client. It will then pass the line to the toUpper thread through another queue of messages.
    takes a char * to indicate replace variable
    needed vars
    cqp1 =0;  
    cqp2 =0;

     tqp1,  tqp2;  

*/

void *characterThread( void *arg)
{
    bool first =true;
    char * p = (char *)arg;// replacement character
    cqp2 =0;
    bool hasdata =false;
    while (running || hasdata)
    {
        pthread_mutex_lock(&mutcharQueue  );
        if( (cqp2-1) !=cqp1)
        {
            // character is aavailable to read from char queue
            char nv =charQueue[cqp2];     
            if(charQueue[cqp2] == ' ' )
            {
                nv =*p;

            }
            
            charQueue[cqp2]='\0';
            pthread_mutex_lock(&muttoUpperQueue);
            if( tqp1>=QueueLen)
            {
                tqp1=0;
            }
            toUpperQueue[tqp1] =nv;
            
            if( first)
            {
                // kick off upper on first
                pthread_create(&UpprThrd,NULL,toupperThread ,(void *) NULL );
                
            }
            first =false;
            
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
    while(hasdata || running)
    {
        pthread_mutex_lock(&muttoUpperQueue);
        if( (tqp2-1) !=tqp1)
        {
            char nv =charQueue[tqp2];     
            nv = toupper(nv);
            charQueue[tqp2]='\0';
            pthread_mutex_lock(&mutwriterQueue);
            if(wqp1>=QueueLen)
            {
                wqp1 =0;
            }
            writerQueue[wqp1] =nv;
            if( first)
            {
                // kick off upper on first
                char * outfile ="./ThOut.txt";
                pthread_create(&WriterThrd,NULL,toupperThread ,(void *) outfile );
                
            }
            first =false;
            
            pthread_mutex_unlock(&mutwriterQueue);
            tqp2++;
            
            if( tqp2>=QueueLen)
            {
                tqp2=0;
            }
            pthread_mutex_unlock(&muttoUpperQueue);
        }
        else
        {
            pthread_mutex_unlock(&muttoUpperQueue);
            hasdata=false;
        }
        


    }
    void * status;
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
    while(hasdata || running)
    {
        pthread_mutex_lock(&mutwriterQueue);
        if( (wqp2-1) !=wqp1)
        {   
            towrite = writerQueue[wqp2];
            writerQueue[wqp2]='\0';
            wqp2++;
            if( wqp2>=QueueLen)
            {
                wqp2=0;
            }
            pthread_mutex_unlock(&mutwriterQueue);// release before the IO call
            fputc(towrite, fp);
            hasdata=true;
        }else
        {
            pthread_mutex_unlock(&mutwriterQueue);
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
        //pthread_join(charTh,&status);

    }

    pthread_exit(NULL);
}