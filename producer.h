#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
void *readerThread( void *arg);
void *characterThread( void *arg);
void *toupperThread( void *arg);
void *writerThread( void *arg);

typedef struct filedata{
    char toreplace;
    char * FileNameDesc;

} FileData;

/* QUEUES */
 char charQueue   [10];  // first queue read thread writes here characterThread reads here
 char toUpperQueue[10];  // second quere charaterthread writes here toUpper thread will read from here
 char writerQueue [10];  // third queue to upper will write here and writer will read here

/*MUtexes*/
pthread_mutex_t mutcharQueue    = PTHREAD_MUTEX_INITIALIZER;  // mutex for character Queue
pthread_mutex_t muttoUpperQueue = PTHREAD_MUTEX_INITIALIZER;// mutex for toUpperQueue
pthread_mutex_t MmutwriterQueue = PTHREAD_MUTEX_INITIALIZER;


