#ifndef __CONSUME_H__
#define __CONSUME_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<wait.h>
#include <sys/wait.h>
void processFile(char * fname);
void pipeouput (int fd[2]);
void ReadPipe( int fd[2]);
#endif