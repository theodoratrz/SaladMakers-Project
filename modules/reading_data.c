#include <stdio.h>
#include <time.h>
#include <sys/time.h> 
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void reading_data()
{
    FILE* file;
    int nof_processes=0,flag=0, chef=0, sm1=0,sm2=0,sm3=0;
    char *start_time,*start_name,*times, *pid, *name, *phrase;
    char ch;
    times = malloc(15*sizeof(char));
    start_time = malloc(15*sizeof(char));
    pid = malloc(6*sizeof(char));
    name = malloc(13*sizeof(char));
    start_name = malloc(13*sizeof(char));
    phrase = malloc(30*sizeof(char));

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

        //fscanf(file, "%[^\n]", c);
        fscanf(file, "%[^]]", times);
        //printf("%s", times);
        fscanf(file, "%c",&ch);
        //printf("%c",ch);
        fscanf(file, "%c",&ch);
        //printf("%c",ch);

        fscanf(file, "%[^]]", pid);
        //printf("%s", pid);
        fscanf(file, "%c",&ch);
        //printf("%c",ch);
        fscanf(file, "%c",&ch);
        //printf("%c",ch);

        fscanf(file, "%[^]]", name);
        //printf("%s", name);
        fscanf(file, "%c",&ch);
        //printf("%c",ch);
        fscanf(file, "%c",&ch);
        //printf("%c",ch);

        fscanf(file, "%[^]]", phrase);
        //printf("%s\n", phrase);
        fscanf(file, "%c",&ch);
       // printf("%c",ch);
        fscanf(file, "%c",&ch);
        //printf("%c",ch);

        if(!strcmp(phrase, "[Start making salad"))
        {
            if(nof_processes == 0)
            {
                flag = 1;
                strcpy(start_time, times);
                strcpy(start_name, name);
                if( (!strcmp(name, "[Saladmaker1")) && (sm1 == 0))
                {
                    sm1 = 1;
                }
                else if((!strcmp(name, "[Saladmaker2")) && (sm2 == 0))
                {
                    sm2 = 1;
                }
                else if((!strcmp(name, "[Saladmaker3")) && (sm3 == 0))
                {
                    sm3 = 1;
                }
                nof_processes++;
                //printf("#process: %s\n", name);
            }
            else
            {
                if( (!strcmp(name, "[Saladmaker1")) && (sm1 == 0))
                {
                    sm1 = 1;
                    nof_processes++;
                }
                else if((!strcmp(name, "[Saladmaker2")) && (sm2 == 0))
                {
                    sm2 = 1;
                    nof_processes++;
                }
                else if((!strcmp(name, "[Saladmaker3")) && (sm3 == 0))
                {
                    sm3 = 1;
                    nof_processes++;
                }
                
                //printf("!process: %s\n", name);
            }
            
            
        }
        else if(!strcmp(phrase,"[End making salad"))
        {
            if(!(strcmp(name, start_name)))
            {
                flag = 0;
                chef = 0;
                sm1=0;
                sm2=0;
                sm3=0;
                printf("[ start: %s], end: %s] ] -> %d processes work concurrently\n",start_time, times, nof_processes);
                nof_processes = 0;
            }
        }
        else
        {
            if(flag)
            {
                if( (!strcmp(name, "[Saladmaker1")) && (sm1 == 0))
                {
                    sm1 = 1;
                    nof_processes++;
                    //printf("@process: %s\n", name);
                }
                else if((!strcmp(name, "[Saladmaker2")) && (sm2 == 0))
                {
                    sm2 = 1;
                    nof_processes++;
                    //printf("$process: %s\n", name);
                }
                else if((!strcmp(name, "[Saladmaker3")) && (sm3 == 0))
                {
                    sm3 = 1;
                    nof_processes++;
                    //printf("^process: %s\n", name);
                }
                else if((!strcmp(name, "[Chef")) && (chef == 0))
                {
                    chef = 1;
                    nof_processes++;
                    //printf("&process: %s\n", name);
                }
            }
        }
        
    }
    

	fclose(file);
    free(name);
    free(start_name);
    free(start_time);
    free(phrase);
    free(pid);
    free(times);
}