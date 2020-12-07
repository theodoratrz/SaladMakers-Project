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

#define SEGMENTSIZE (5*sizeof(int) + 4*sizeof(sem_t))
#define SEGMENTPERM 0666


int main(int argc, char* argv[])
{
	int id,retval;
	key_t key = ftok(".", 2);
    sem_t *chef;

	int* veggie_table, *flag;
	int* nof_salads;

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
	flag = nof_salads+1;
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

	*nof_salads = 5;

	while(1)
	{
		sem_wait(chef);

		if((*nof_salads) != 0)
		{
			*flag = 1;
			veggie_table[0] = 1;	// tomato
			veggie_table[1] = 1;	// pepper
			veggie_table[2] = 0;	// onion
			printf("chef: salad left %d\n",*nof_salads);
		}
		else
		{
			if((*flag) == 0)
			{
				break;
			}	
		}
		sem_post(onion_saladmaker);
	}
		if (shmdt((void *)chef) == -1) 
		{   //shared memory detach
			perror("Failed to destroy shared memory segment");
			return 1;
		}

	if (shmctl(id, IPC_RMID, 0 ) <0) {
	perror("semctl");
	exit(1);
	}
}