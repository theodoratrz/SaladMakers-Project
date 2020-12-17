# Λειτουργικά Συστήματα(Κ-22)  
## Τμήμα Περιττών
## Οκτώβριος2020
## Τροιζή Θεοδώρα
### sdi1800197

  
  
**Compilation:** First we run cef with the command: *./chef -n numOfSlds -m mantime*  
Chef prints the id of the shared segment.  
Then we run the saladmakers with the command: *./saladmaker -t1 lb -t2 ub -i ingredient -s shmid*  
**-i ingredient:**  
"o" for onion == first saladmaker  
"p" for pepper == second saladmaker   
"t" for tomato == third saladmaker

(example: assume we want to run saladmaker1: ./saladmaker -t1 1 -t2 3 **-i o** -s 12345)


For this project, we used 4 semaphores, one for each process. Chef's semaphore is set to 1 because we want this process to be the very first to enter its critical section. If chef must increase the number of the ingredients from 0 to 1 (meaning: let the ingredients on the table) before a saladmaker tries to decrease it (meaning: fetch the ingredients from the table). So, we initialize the saladmakers' semaphores to 1.

**Shared variables:** Apart from semaphores, the processes must share other variables, too. Chef must know saladmakers' pid(sm1_pid, sm2_pid, sm3_pid), how many salads each saladmaker makes(onion_salads refers to the first saladmaker, pepper_salad refers to the second saladmaker, tomato_salad refers to the third saladmaker), how many salads are made at the end(sumofsalads), when a saladmaker is finished/terminates(flag1, flag2, flag3), how many salads are left(nofsalads). There is also an array that represnts the table, where the chef lets the ingredients.

**Writing data:** We make a log file for each of the saladmakers and a shared log file for all the processes. At the class we have learned that only one writer at a time can modify a file. That's why whenever a process wants to write something to the shared log file, it writes it while it is in its critical section. 

1. Process Chef: Chef creates the shared memory. Attaches all the semaphores and data that is using. Chef,also, initializes all the semaphores and variables. While there salads left, chef chooses 2 ingredients randomly, notify the saladmakers and write to the shared log file all its actions. Chef must sleep for some time. Because we wanted this message: "Man time for resting", written in the shared log file, we added the sleep in chef's critical section. But, we set a flag, in order to know if this is the very first time that chef enters its critical section. If it is the first we don't want it to sleep. When all salads are finished, chef must wait for the saladmakers to terminate first, so to dettach the shared memory. Chef created the shared memory and must as well delete it. When all saladmakers are finished, chef produces its output , dettaches and deletes the shared memory. At the end, chef calls a function, that reads the shared log file and prints the time spaces when many processes were running concurrently.

2. Process Saladmaker: We have 1 excetubale for all saladmakers, but we use an extra flag "-i o/p/t" to determine which saladmaker is each time("o" for onion == first saladmaker, "p" for pepper == second saladmaker, "t" for tomato == third saladmaker) . Each saladmaker must attach the shared memory, in order to have access to the shared variables. Depending on the low and upper bound(arguments) saladmaker picks a "cooking time space", randomly. While there salads left, saladmaker enters its first critical section, picks its ingredient and decreases the counter of "salads left". When "cooking time" ends, saladmaker enters again its critical section, to increase the counters and to write to the shared file. This is because the "cooking time" cannot be in the critical section, so we make 2 critical sections, one before sleep and one after. When there are no salads left, saladmakers makes its flag=0 , dettaches the shared memory and terminates. 

3. Reading Data: We read the shared log file top to bottom. We check from when a saladmaker starts making a salad, till it finishes it, if other saladmakers have started making their salads. If so, we assume that in this time space more than 1 process were running concurrently. 