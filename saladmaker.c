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


enum{TOMATO, PEPPER, ONION};

int main(int argc, char* argv[])
{
    FILE* file;
    sem_t *chef;

	int* veggie_table;
	int* nof_salads,*onion_salads, *tomato_salads, *pepper_salads;
    int low_bound, upp_bound,key,cooking_time;
    char ingredient;
    struct timeval  now;
	struct tm* local;

	gettimeofday(&now, NULL);
	local = localtime(&now.tv_sec);

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
    onion_salads = sum_of_salads+1;
	tomato_salads = onion_salads+1;
	pepper_salads = tomato_salads+1;

    *onion_salads = 0;
	*tomato_salads = 0;
	*pepper_salads = 0;

    switch (ingredient)
    {
    case 'o':
        while(1)
        {
            sem_wait(onion_saladmaker);
            file = fopen("log_file.txt", "a+");
            if(file == NULL)
            {
                perror("Opening file");
                exit(2);
            }
            fprintf(file, "%s", "Onion is waiting for the ingredients\n");
            if((*nof_salads) != 0)
            {
                fprintf(file, "%s", "Onion took the ingredients\n");
                veggie_table[TOMATO]--;
                veggie_table[PEPPER]--;
                (*nof_salads)--;
                fprintf(file, "%s", "Onion starts cooking salad\n");
            }
            else
            {
                *flag1 = 0;
                sem_post(chef);
                fprintf(file, "%s", "Onion is finished\n");
                if (shmdt((void *)chef) == -1) 
                {  /* shared memory detach */
                    perror("Failed to destroy shared memory segment");
                    return 1;
                }
                return 0;
            }
            fclose(file);
            sem_post(chef);

            sleep(cooking_time);

            sem_wait(onion_saladmaker);
            file = fopen("log_file.txt", "a+");
            if(file == NULL)
            {
                perror("Opening file");
                exit(2);
            }
            fprintf(file, "%s", "Onion is cooking\n");
            (*onion_salads)++;
            (*sum_of_salads)++;
            fprintf(file, "%s", "Onion finished the salad\n");
            fclose(file);
            sem_post(chef);
        }

        break;
    case 't':
        while(1)
        {
            sem_wait(tomato_saladmaker);
            file = fopen("log_file.txt", "a+");
            if(file == NULL)
            {
                perror("Opening file");
                exit(2);
            }
            fprintf(file, "%s", "Tomato is waiting for the ingredients\n");
            if((*nof_salads) != 0)
            {
                fprintf(file, "%s", "Pepper took the ingredients\n");
                veggie_table[ONION]--;
                veggie_table[PEPPER]--;
                (*nof_salads)--;
                fprintf(file, "%s", "Tomato starts cooking salad\n");
            }
            else
            {
                *flag2 = 0;
                sem_post(chef);
                fprintf(file, "%s", "Tomato is finished\n");
                if (shmdt((void *)chef) == -1) 
                {  /* shared memory detach */
                    perror("Failed to destroy shared memory segment");
                    return 1;
                }
                return 0;
            }
            fclose(file);
            sem_post(chef);

            sleep(cooking_time);
            
            sem_wait(tomato_saladmaker);
            file = fopen("log_file.txt", "a+");
            if(file == NULL)
            {
                perror("Opening file");
                exit(2);
            }
            fprintf(file, "%s", "Tomato is cooking\n");
            (*tomato_salads)++;
            (*sum_of_salads)++;
            fprintf(file, "%s", "Tomato finished the salad\n");
            fclose(file);
            sem_post(chef);
        }

        break;
    case 'p':
        while(1)
        {
            sem_wait(pepper_saladmaker);
            file = fopen("log_file.txt", "a+");
            if(file == NULL)
            {
                perror("Opening file");
                exit(2);
            }
            fprintf(file, "%s", "Pepper is waiting for the ingredients\n");
            if((*nof_salads) != 0)
            {
                fprintf(file, "%s", "Pepper took the ingredients\n");
                veggie_table[TOMATO]--;
                veggie_table[ONION]--;
                (*nof_salads)--;
                fprintf(file, "%s", "Pepper starts cooking salad\n");
            }
            else
            {
                *flag3 = 0;
                sem_post(chef);
                fprintf(file, "%s", "Pepper is finished\n");
                if (shmdt((void *)chef) == -1) 
                {  /* shared memory detach */
                    perror("Failed to destroy shared memory segment");
                    return 1;
                }
                return 0;
            }
            fclose(file);
            sem_post(chef);

            sleep(cooking_time);
            
            sem_wait(pepper_saladmaker);
            file = fopen("log_file.txt", "a+");
            if(file == NULL)
            {
                perror("Opening file");
                exit(2);
            }
            fprintf(file, "%s", "Pepper is cooking\n");
            (*pepper_salads)++;
            (*sum_of_salads)++;
            fprintf(file, "%s", "Pepper finished the salad\n");
            fclose(file);
            sem_post(chef);
        }

        break;
    default:
        break;
    }
}
