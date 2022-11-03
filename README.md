## Shared Memory Project

In this project, we implement independent programms, which run simultaneously as a part of the Operational Systems course.

**Summary**

![image](https://user-images.githubusercontent.com/61420354/199811994-98800ae4-4db8-40b0-aa40-0f5b413288ef.png)

- Each saladmaker usees the ingredient that has access to and waits for the other ingredients to arrive at the shared table.
-  When a saladmaker has every ingredient, starts to prepare the salad. This process takes a while, and when the salad is ready, it is delivered at the available waiter. 
-  The chef each time chooses 2 of the ingredients randomly. This combination of vegetables can supply a specific saladmaker at a time. The chef "informs" the saladmaker to take the ingredients that he is need of. For example if the chef chooses tomato and onion he will inform the saladmaker with the peppers.

**Goals/Tasks**
- Use semaphors to achieve a successful cooperation between the independent processes.
- Use a shared memory segment as that the processes can access variables and data sturctures in a right way.

**Compilation:** First we run cef with the command: `./chef -n numOfSlds -m mantime`
- Chef prints the id of the shared segment.  
- Then we run the saladmakers with the command: `./saladmaker -t1 lb -t2 ub -i ingredient -s shmid`  

  `ingredient` can be one of the following:
  - `o` for onion
  - `p` for pepper
  - `t` for tomato

  for example: assume we want to run saladmaker1: ```./saladmaker -t1 1 -t2 3 -i o -s 12345```
