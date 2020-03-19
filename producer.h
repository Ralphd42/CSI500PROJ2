#ifndef __PROD_H__
#define __PROD_H__

#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include <stdbool.h>
void *readerThread( void *arg);
void *characterThread( void *arg);
void *toupperThread( void *arg);
void *writerThread( void *arg);
/*constants*/
#define QueueLen 10
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

bool running ;




#endif