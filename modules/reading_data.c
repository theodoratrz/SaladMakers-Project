#include "reading_data.h"


void reading_data()
{
    FILE* file;
    int nof_processes=0,flag=0, sm1=0,sm2=0,sm3=0;                   // flags: nof_processes!=0 if another saladmaker has already started making a salad
    // sm1/2/3 !=0 if they already 
    char *start_time,*start_name,*times, *pid, *name, *phrase;
    char ch;
    times = malloc(16*sizeof(char));                                // allocate memory for the data(+1 because strcpy adds \0 at the end)
    start_time = malloc(16*sizeof(char));
    pid = malloc(7*sizeof(char));
    name = malloc(14*sizeof(char));
    start_name = malloc(14*sizeof(char));
    phrase = malloc(31*sizeof(char));

    file = fopen("log_file.txt", "r");                              // open file for reading
	if(file == NULL)
    {
        perror("Opening file");
        exit(2);
    }

    while(1)
    {
        if(feof(file))                                          // stop when find file eof
        {
            break;
        }

        fscanf(file, "%[^]]", times);                           // read till "]"
        fscanf(file, "%c",&ch);                                 // read "]"
        fscanf(file, "%c",&ch);                                 // read " "

        fscanf(file, "%[^]]", pid);                             // read till "]"
        fscanf(file, "%c",&ch);                                 // read "]"
        fscanf(file, "%c",&ch);                                 // read " "

        fscanf(file, "%[^]]", name);                            // read till "]"
        fscanf(file, "%c",&ch);                                 // read "]"
        fscanf(file, "%c",&ch);                                 // read " "

        fscanf(file, "%[^]]", phrase);                          // read till "]"
        fscanf(file, "%c",&ch);                                 // read "]"
        fscanf(file, "%c",&ch);                                 // read "\n"

        if(!strcmp(phrase, "[Start making salad"))              // if you find this phrase
        {
            if(nof_processes == 0)                              // first process that makes salad
            {
                strcpy(start_time, times);                      // keep time and name
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
                nof_processes++;                                // increase running processes
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
        else if(!strcmp(phrase,"[End making salad"))        // if the first saladmaker(the first found) ends
        {
            if(!(strcmp(name, start_name)))                 // print the list of timelines
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
    free(name);                                             // free all memory
    free(start_name);
    free(start_time);
    free(phrase);
    free(pid);
    free(times);
}