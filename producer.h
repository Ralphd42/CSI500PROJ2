#ifndef __PROD_H__
#define __PROD_H__

#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>

/*thread function prototypes*/
void *readerThread( void *arg);
void *characterThread( void *arg);
void *toupperThread( void *arg);
void *writerThread( void *arg);

/*Threads   */
pthread_t ReaderThrd;
bool readstat;  //thread status

pthread_t ChrThrd;
bool charStat; // thread status

pthread_t UpprThrd;
bool upprStat; // thread status

pthread_t WriterThrd;
// no status needed
/*constants*/
#define QueueLen 10

/* data structures  */
/*Used to pass data to threads*/
typedef struct filedata{
    char toreplace;
    char * FileNameDesc;

} FileData;

/* QUEUES */
char charQueue   [QueueLen];  // first queue read thread writes here characterThread reads here
int cqp1,  cqp2;        // start and end 
char toUpperQueue[QueueLen];  // second quere charaterthread writes here toUpper thread will read from here
int tqp1,  tqp2;         // start end
char writerQueue [QueueLen];  // third queue to upper will write here and writer will read here
int wqp1,  wqp2;     //start end of writer queue    

/* helper functioins*/
void cleanQueues();





#endif