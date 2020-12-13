
#include "reading_data.h"

void reading_data()
{
    FILE* file;
    int nof_processes=0,flag=0, sm1=0,sm2=0,sm3=0;
    char *start_time,*start_name,*times, *pid, *name, *phrase;
    char ch;
    times = malloc(16*sizeof(char));
    start_time = malloc(16*sizeof(char));
    pid = malloc(7*sizeof(char));
    name = malloc(14*sizeof(char));
    start_name = malloc(14*sizeof(char));
    phrase = malloc(31*sizeof(char));

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

        fscanf(file, "%[^]]", times);
        fscanf(file, "%c",&ch);
        fscanf(file, "%c",&ch);

        fscanf(file, "%[^]]", pid);
        fscanf(file, "%c",&ch);
        fscanf(file, "%c",&ch);

        fscanf(file, "%[^]]", name);
        fscanf(file, "%c",&ch);
        fscanf(file, "%c",&ch);

        fscanf(file, "%[^]]", phrase);
        fscanf(file, "%c",&ch);
        fscanf(file, "%c",&ch);

        if(!strcmp(phrase, "[Start making salad"))
        {
            if(nof_processes == 0)
            {
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
            }
            else
            {
                if( (!strcmp(name, "[Saladmaker1")) && (sm1 == 0))
                {
                    if(!flag)
                    {
                        strcpy(start_time, times);
                        flag = 1;
                    }
                    sm1 = 1;
                    nof_processes++;
                }
                else if((!strcmp(name, "[Saladmaker2")) && (sm2 == 0))
                {
                    if(!flag)
                    {
                        strcpy(start_time, times);
                        flag = 1;
                    }
                    sm2 = 1;
                    nof_processes++;
                }
                else if((!strcmp(name, "[Saladmaker3")) && (sm3 == 0))
                {
                    if(!flag)
                    {
                        strcpy(start_time, times);
                        flag = 1;
                    }
                    sm3 = 1;
                    nof_processes++;
                }
                
            }
            
            
        }
        else if(!strcmp(phrase,"[End making salad"))
        {
            if(!(strcmp(name, start_name)))
            {
                flag = 0;
                sm1=0;
                sm2=0;
                sm3=0;
                printf("[ start: %s], end: %s] ] -> %d processes work concurrently\n",start_time, times, nof_processes);
                nof_processes = 0;
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