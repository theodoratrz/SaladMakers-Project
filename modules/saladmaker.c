#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/time.h>
#include "writing_data.h"

enum{TOMATO, PEPPER, ONION};

int main(int argc, char* argv[])
{
    sem_t *chef;

	int* veggie_table;
	int* nof_salads,*onion_salads, *tomato_salads, *pepper_salads,*sm1_pid, *sm2_pid, *sm3_pid;
    int low_bound, upp_bound,key,cooking_time, pid;
    char ingredient;
    struct timeval  now;
	struct tm* local;

	gettimeofday(&now, NULL);
	local = localtime(&now.tv_sec);

    sem_t *onion_saladmaker;

	sem_t *pepper_saladmaker;

	sem_t *tomato_saladmaker;

    for(int i = 1; i < argc; i+=2 )             // flags may be given in any order
    {
        if(!(strcmp(argv[i], "-t1")))
        {
            low_bound = atoi(argv[i+1]);
        }
        else if(!(strcmp(argv[i], "-t2")))
        {
            upp_bound = atoi(argv[i+1]);
        }
        else if(!(strcmp(argv[i], "-s")))
        {
            key = atoi(argv[i+1]);
        }
        else if(!(strcmp(argv[i], "-i")))       // the user calls the same excecutable of saladmaker
        {                                       // and gives as argument which saladmaker is
            ingredient = *(argv[i+1]);          // by giving the standard ingredient of each saladmaker
        }
    }
    srand(time(NULL));
    // compute the random cooking time for each saladmaker, depending on low and upper bounds
    cooking_time = (rand() % (upp_bound - low_bound +1)) + low_bound;
    
    int *flag1,*flag2,*flag3,*sum_of_salads;

    // Attach the segment
    chef = (sem_t*) shmat(key, (void*)0,0);
	if ( chef == (void *) -1) { perror("Attachment."); exit(2);}
	onion_saladmaker = chef+1;
	pepper_saladmaker = chef+2;
	tomato_saladmaker = chef+3;

	veggie_table = (int *)(tomato_saladmaker+3);

	nof_salads = veggie_table+3;
    flag1 = nof_salads+1;
	flag2 = flag1+1;
	flag3 = flag2+1;
    sum_of_salads = flag3+1;
    onion_salads = sum_of_salads+1;
	tomato_salads = onion_salads+1;
	pepper_salads = tomato_salads+1;
    sm1_pid = pepper_salads+1;
	sm2_pid = sm1_pid+1;
	sm3_pid = sm2_pid+1;

    *onion_salads = 0;
	*tomato_salads = 0;
	*pepper_salads = 0;
    pid = getpid();                             // each saladmaker's pid

    switch (ingredient)
    {
    case 'o':                                   // first saladmaker
        while(1)
        {
            sem_wait(onion_saladmaker);         // saladmaker 1 wants to enter its critical section
            *sm1_pid = pid;                     // chef wants to know the pid of each saladmaker
            writing_data(now, local, "log_file.txt","Saladmaker1", pid, "Waiting for ingredients");
            writing_data(now, local, "log_file_sm1.txt","Saladmaker1", pid, "Waiting for ingredients");
            if((*nof_salads) != 0)              // if there are salads to be made
            {
                writing_data(now, local,"log_file.txt", "Saladmaker1", pid, "Get ingredients");
                writing_data(now, local,"log_file_sm1.txt", "Saladmaker1", pid, "Get ingredients");
                veggie_table[TOMATO]--;         // saladmaker takes the ingredients from the table
                veggie_table[PEPPER]--;
                (*nof_salads)--;                // update the number of salads left
                writing_data(now, local, "log_file.txt","Saladmaker1", pid, "Start making salad");
                writing_data(now, local, "log_file_sm1.txt","Saladmaker1", pid, "Start making salad");
            }
            else
            {                                   // if all salads have been made
                *flag1 = 0;                     // set flag to 1
                writing_data(now, local,"log_file.txt", "Saladmaker1", pid, "finished");
                writing_data(now, local,"log_file_sm1.txt", "Saladmaker1", pid, "finished");
                sem_post(chef);                 // post chef's semaphore
                if (shmdt((void *)chef) == -1)  // detach the segment
                {  /* shared memory detach */
                    perror("Failed to destroy shared memory segment");
                    return 1;
                }

                return 0;
            }
            sem_post(chef);

            sleep(cooking_time);

            sem_wait(onion_saladmaker);
            (*onion_salads)++;
            (*sum_of_salads)++;
            writing_data(now, local,"log_file.txt", "Saladmaker1", pid, "End making salad");
            writing_data(now, local,"log_file_sm1.txt", "Saladmaker1", pid, "End making salad");
            sem_post(chef);
        }

        break;
    case 't':
        while(1)
        {
            sem_wait(tomato_saladmaker);        // saladmaker 3 wants to enter its critical section
            *sm3_pid = pid;
            writing_data(now, local, "log_file.txt","Saladmaker3", pid, "Waiting for ingredients");
             writing_data(now, local, "log_file_sm3.txt","Saladmaker3", pid, "Waiting for ingredients");
            if((*nof_salads) != 0)
            {
                writing_data(now, local,"log_file.txt", "Saladmaker3", pid, "Get ingredients");
                writing_data(now, local,"log_file_sm3.txt", "Saladmaker3", pid, "Get ingredients");
                veggie_table[ONION]--;
                veggie_table[PEPPER]--;
                (*nof_salads)--;
                writing_data(now, local,"log_file.txt", "Saladmaker3", pid, "Start making salad");
                writing_data(now, local,"log_file_sm3.txt", "Saladmaker3", pid, "Start making salad");
            }
            else
            {
                *flag2 = 0;
                writing_data(now, local, "log_file.txt","Saladmaker3", pid, "finished");
                writing_data(now, local, "log_file_sm3.txt","Saladmaker3", pid, "finished");
                sem_post(chef);
                if (shmdt((void *)chef) == -1) 
                {  /* shared memory detach */
                    perror("Failed to destroy shared memory segment");
                    return 1;
                }
                return 0;
            }
            sem_post(chef);
            
            sleep(cooking_time);

            sem_wait(tomato_saladmaker);
            (*tomato_salads)++;
            (*sum_of_salads)++;
            writing_data(now, local, "log_file.txt","Saladmaker3", pid, "End making salad");
            writing_data(now, local, "log_file_sm3.txt","Saladmaker3", pid, "End making salad");
            sem_post(chef);
        }

        break;
    case 'p':
        while(1)
        {
            sem_wait(pepper_saladmaker);        // saladmaker 2 wants to enter its critical section
            *sm2_pid = pid;
            writing_data(now, local, "log_file.txt","Saladmaker2", pid, "Waiting for ingredients");
            writing_data(now, local, "log_file_sm2.txt","Saladmaker2", pid, "Waiting for ingredients");
            if((*nof_salads) != 0)
            {
                writing_data(now, local,"log_file.txt", "Saladmaker2", pid, "Get ingredients");
                writing_data(now, local,"log_file_sm2.txt", "Saladmaker2", pid, "Get ingredients");
                veggie_table[TOMATO]--;
                veggie_table[ONION]--;
                (*nof_salads)--;
                writing_data(now, local,"log_file.txt", "Saladmaker2", pid, "Start making salad");
                writing_data(now, local,"log_file_sm2.txt", "Saladmaker2", pid, "Start making salad");
            }
            else
            {
                *flag3 = 0;
                writing_data(now, local,"log_file.txt", "Saladmaker2", pid, "finished");
                writing_data(now, local,"log_file_sm2.txt", "Saladmaker2", pid, "finished");
                sem_post(chef);
                if (shmdt((void *)chef) == -1) 
                {  /* shared memory detach */
                    perror("Failed to destroy shared memory segment");
                    return 1;
                }
                return 0;
            }
            sem_post(chef);

            sleep(cooking_time);
            
            sem_wait(pepper_saladmaker);
            (*pepper_salads)++;
            (*sum_of_salads)++;
            writing_data(now, local,"log_file.txt", "Saladmaker2", pid, "End making salad");
            writing_data(now, local,"log_file_sm2.txt", "Saladmaker2", pid, "End making salad");
            sem_post(chef);
        }

        break;
    default:
        break;
    }
}
