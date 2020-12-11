#include <stdio.h>
#include <time.h>
#include <sys/time.h> 
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main()
{
    FILE* file;
    int data=0;
    char* c;
    char ch;
    c = malloc(70*sizeof(char));
    file = fopen("log_file.txt", "r");
	if(file == NULL)
    {
        perror("Opening file");
        exit(2);
    }

    while(1)
    {
        if(feof(file))
        {
            break;
        }

        fscanf(file, "%[^\n]", c);
        
    }
    

	fclose(file);
    free(c);
}