#include <stdio.h>
#include <time.h>
#include <sys/time.h> 
#include <string.h>
#include <stdlib.h>

void writing_data(struct timeval  now, struct tm* local, char* name, int pid, char* phrase);