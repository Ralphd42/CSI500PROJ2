#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void *readerThread( void *arg);
void *characterThread( void *arg);
void *toupperThread( void *arg);
void *writerThread( void *arg);

/* QUEUES */

