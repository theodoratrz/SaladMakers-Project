#include "writing_data.h"

void writing_data(struct timeval  now, struct tm* local, char* file_name, char* name, int pid, char* phrase)
{
    FILE* file;
    file = fopen(file_name, "a+");
    if(file == NULL)
    {
        perror("Opening file");
        exit(2);
    }

    gettimeofday(&now, NULL);
	local = localtime(&now.tv_sec);

    fprintf(file, "%s", "[");
    fprintf(file,"%02d:", local->tm_hour);
    fprintf(file, "%02d:", local->tm_min);
    fprintf(file, "%02d:", local->tm_sec);
    fprintf(file, "%03ld",now.tv_usec / 1000);
    fprintf(file, "%s", "] ");

    fprintf(file, "%s", "[");
    fprintf(file, "%d", pid);
    fprintf(file, "%s", "] ");

    fprintf(file, "%s", "[");
    fprintf(file, "%s", name);
    fprintf(file, "%s", "] ");

    fprintf(file, "%s", "[");
    fprintf(file, "%s", phrase);
    fprintf(file, "%s", "]\n");

    fclose(file);
}