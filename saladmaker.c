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
    int low_bound, upp_bound,key,cooking_time;
    char ingredient;

    sem_t *onion_saladmaker;

	sem_t *pepper_saladmaker;

	sem_t *tomato_saladmaker;

    for(int i = 1; i < argc; i+=2 )
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
        else if(!(strcmp(argv[i], "-i")))
        {
            ingredient = *(argv[i+1]);
        }
    }
    srand(time(NULL));
    cooking_time = (rand() % (upp_bound - low_bound +1)) + low_bound;
    
    //int key = atoi(argv[1]);
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

    switch (ingredient)
    {
    case 'o':
        while(1)
        {
            sem_wait(onion_saladmaker);
            if((*nof_salads) != 0)
            {
                veggie_table[TOMATO]--;
                veggie_table[PEPPER]--;
                (*nof_salads)--;
                printf("ONION: start salad %d\n",*sum_of_salads);
            }
            else
            {
                *flag1 = 0;
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

            sem_wait(onion_saladmaker);
            printf("onion sleeped for %d\n", cooking_time);
            (*sum_of_salads)++;
            printf("onion: salads made %d\n", *sum_of_salads);
            sem_post(chef);
        }

        break;
    case 't':
        while(1)
        {
            sem_wait(tomato_saladmaker);
            if((*nof_salads) != 0)
            {
                veggie_table[ONION]--;
                veggie_table[PEPPER]--;
                (*nof_salads)--;
                printf("TOMATO: start salad %d\n", *sum_of_salads);
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
                return 0;
            }
            
            sem_post(chef);

            sleep(cooking_time);
            
            sem_wait(tomato_saladmaker);
            printf("tomato sleeped for %d\n", cooking_time);
            (*sum_of_salads)++;
            printf("tomato: salads made %d\n", *sum_of_salads);
            sem_post(chef);
        }

        break;
    case 'p':
        while(1)
        {
            sem_wait(pepper_saladmaker);
            if((*nof_salads) != 0)
            {
                veggie_table[TOMATO]--;
                veggie_table[ONION]--;
                (*nof_salads)--;
                printf("PEPPER: start salad %d\n", *sum_of_salads);
            }
            else
            {
                *flag3 = 0;
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
            printf("pepper sleeped for %d\n", cooking_time);
            (*sum_of_salads)++;
            printf("pepper: salads made %d\n", *sum_of_salads);
            sem_post(chef);
        }

        break;
    default:
        break;
    }
    
}