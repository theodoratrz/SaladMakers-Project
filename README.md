## Shared Memory Project

In this project, we implement independent programms, which run simultaneously. 

**Goals/Tasks**
- Use semaphors to achieve a successful cooperation between the independent processes.
- Use a shared memory segment as that the processes can access variables and data sturctures in a right way.

**Compilation:** First we run cef with the command: *./chef -n numOfSlds -m mantime*  
- Chef prints the id of the shared segment.  
- Then we run the saladmakers with the command: *./saladmaker -t1 lb -t2 ub -i ingredient -s shmid*  

**-i ingredient:**  
"o" for onion == first saladmaker  
"p" for pepper == second saladmaker   
"t" for tomato == third saladmaker

(example: assume we want to run saladmaker1: ./saladmaker -t1 1 -t2 3 **-i o** -s 12345)
