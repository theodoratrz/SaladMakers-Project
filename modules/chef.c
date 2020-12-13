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
#include <errno.h>
#include <sys/time.h>  /* times() */
#include "writing_data.h"
#include "reading_data.h"

#define SEGMENTSIZE (6*sizeof(int) + 4*sizeof(sem_t))
#define SEGMENTPERM 0666

enum{TOMATO, PEPPER, ONION};

int main(int argc, char* argv[])
{
	int id,pid,retval,index, previous = -1;
	int* veggie_table, *flag1,*flag2,*flag3;
	int* nof_salads, *sum_of_salads, *onion_salads, *tomato_salads, *pepper_salads, *sm1_pid,*sm2_pid,*sm3_pid;
	int mantime,no_wait = 1,rest=0;
	
	struct timeval  now;
	struct tm* local;

	gettimeofday(&now, NULL);
	local = localtime(&now.tv_sec);

	key_t key = ftok(".", 2);
    sem_t *chef;
    sem_t *onion_saladmaker;
	sem_t *pepper_saladmaker;
	sem_t *tomato_saladmaker;

    id = shmget(key, SEGMENTSIZE, IPC_CREAT|SEGMENTPERM);
	if (id == -1) 
	{
		printf("%d\n", errno);
		perror("Creation error\n");
		exit(EXIT_FAILURE);
	}
	else printf("Allocated %d\n", id);

	printf("id: %d\n", id);
    /* Attach the segment. */
	chef = (sem_t*) shmat(id, (void*)0,0);
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

    /* Initialize the semaphore. */
	retval = sem_init(chef,1,1);
	if (retval != 0) {
		perror("Couldn't initialize.");
		exit(3);
	}

    /* Initialize the semaphore. */
	retval = sem_init(onion_saladmaker,1,0);
	if (retval != 0) {
		perror("Couldn't initialize.");
		exit(3);
	}

    /* Initialize the semaphore. */
	retval = sem_init(pepper_saladmaker,1,0);
	if (retval != 0) {
		perror("Couldn't initialize.");
		exit(3);
	}

    /* Initialize the semaphore. */
	retval = sem_init(tomato_saladmaker,1,0);
	if (retval != 0) {
		perror("Couldn't initialize.");
		exit(3);
	}

	srand(time(NULL));

	veggie_table[TOMATO] = 0;
	veggie_table[PEPPER] = 0;
	veggie_table[ONION] = 0;

	*onion_salads = 0;
	*tomato_salads = 0;
	*pepper_salads = 0;

	pid = getpid();

	if( !(strcmp(argv[1], "-n")) )
	{
		*nof_salads = atoi(argv[2]);
		mantime = atoi(argv[4]);
	}
	else if( !(strcmp(argv[1], "-m")) )
	{
		mantime = atoi(argv[2]);
		*nof_salads = atoi(argv[4]);
	}

	*sum_of_salads = 0;
	*flag1 = 1;
	*flag2 = 1;
	*flag3 = 1;
	
	while(1)
	{
		sem_wait(chef);
		if(!rest)
		{
			writing_data(now, local,"log_file.txt", "Chef", pid, "Man time for resting");
			sleep(mantime);
		}

		if((*nof_salads) != 0)
		{
			rest = 1;
			index = rand()%3;
			while(index == previous)
			{
				index = rand()%3;
			}
			previous = index;
			switch (index)
			{
			case 0:
				writing_data(now, local,"log_file.txt", "Chef", pid, "Selecting tomato and pepper");
				veggie_table[TOMATO] += 1;
				veggie_table[PEPPER] += 1;
				writing_data(now, local,"log_file.txt", "Chef", pid, "Notify Saladmaker1");
				sem_post(onion_saladmaker);
				break;
			case 1:
				writing_data(now, local, "log_file.txt","Chef", pid, "Selecting onion and pepper");
				veggie_table[PEPPER] += 1;
				veggie_table[ONION] += 1;
				writing_data(now, local, "log_file.txt","Chef", pid, "Notify Saladmaker3");
				sem_post(tomato_saladmaker);
				break;
			case 2:
				writing_data(now, local,"log_file.txt", "Chef", pid, "Selecting tomato and onion");
				veggie_table[TOMATO] += 1;
				veggie_table[ONION] += 1;
				writing_data(now, local,"log_file.txt", "Chef", pid, "Notify Saladmaker2");
				sem_post(pepper_saladmaker);
				break;
			default:
				break;
			}
		}
		else
		{
			break;
		}
		
	}

	while(1)
	{
		if(no_wait == 0)
		{
			sem_wait(chef);
		}
		else
		{
			no_wait = 0;
		}
		
		if((*flag1) != 0)
		{
			sem_post(onion_saladmaker);
		}
		else if((*flag2) != 0)
		{
			sem_post(tomato_saladmaker);
		}
		else if((*flag3) != 0)
		{
			sem_post(pepper_saladmaker);
		}
		else
		{
			break;
		}	
		
	}

	printf("Total #salads [ %d ]\n", *sum_of_salads);
	printf("salads of salad_maker1 [%d] : [ %d ]\n",*sm1_pid, *onion_salads);
	printf("salads of salad_maker2 [%d] : [ %d ]\n",*sm2_pid,*pepper_salads);
	printf("salads of salad_maker3 [%d] : [ %d ]\n",*sm3_pid,*tomato_salads);
	
	if (shmdt((void *)chef) == -1) 
	{   //shared memory detach
		perror("Failed to destroy shared memory segment");
		return 1;
	}

	if (shmctl(id, IPC_RMID, 0 ) <0) {
	perror("semctl");
	exit(1);
	}

	printf("Time intervals: (in increasing order)\n");
	reading_data();
	
}
