#include <stdio.h>
#include <time.h>
#include <sys/time.h> 
#include <string.h>
#include <stdlib.h>

// a function that writes the data to the file with name file_name
void writing_data(struct timeval  now, struct tm* local, char* file_name, char* name, int pid, char* phrase);