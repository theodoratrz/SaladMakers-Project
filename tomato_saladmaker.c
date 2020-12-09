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

enum{TOMATO, PEPPER, ONION};

int main(int argc, char* argv[])
{
    sem_t *chef;

	int* veggie_table;
	int* nof_salads;

    sem_t *onion_saladmaker;

	sem_t *pepper_saladmaker;

	sem_t *tomato_saladmaker;

    int key = atoi(argv[1]);
    int *flag1,*flag2,*flag3,*sum_of_salads;

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

    while(1)
    {
        sem_wait(tomato_saladmaker);
        if((*nof_salads) != 0)
        {
            veggie_table[ONION]--;
            veggie_table[PEPPER]--;
            (*nof_salads)--;
            (*sum_of_salads)++;
            printf("tomato: salad left %d\n", *nof_salads);
        }
        else
        {
            *flag2 = 0;
            sem_post(chef);
            if (shmdt((void *)chef) == -1) 
            {  /* shared memory detach */
                perror("Failed to destroy shared memory segment");
                return 1;
            }
            break;
        }
        
        sem_post(chef);
    }

}