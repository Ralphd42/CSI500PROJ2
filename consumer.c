#include "consumer.h"
/*
    this needs to do the following
    1 get item 1 at a time from file
    2 redirect std output to a pipe 
    3 fork  
    4 set child to
*/
void processFile(char * fname)
{
    FILE* fp = fopen(fname, "rb");
    //ssize_t	getline (char **, size_t *, FILE *);
    ssize_t read =
    while ((read = getline(&line, &len, fp)) != -1) 
    {



    }
}