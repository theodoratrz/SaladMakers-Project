## Shared Memory Project

In this project, we implement independent programms, which run simultaneously as a part of the Operational Systems course.

**Summary**
![image](https://user-images.githubusercontent.com/61420354/199811994-98800ae4-4db8-40b0-aa40-0f5b413288ef.png)


**Goals/Tasks**
- Use semaphors to achieve a successful cooperation between the independent processes.
- Use a shared memory segment as that the processes can access variables and data sturctures in a right way.

**Compilation:** First we run cef with the command: `./chef -n numOfSlds -m mantime`
- Chef prints the id of the shared segment.  
- Then we run the saladmakers with the command: `./saladmaker -t1 lb -t2 ub -i ingredient -s shmid`  

  `ingredient` can be one of the following:
  - `o` for onion. The first saladmaker is responsi
  - `p` for pepper == second saladmaker   
  - `t` for tomato == third saladmaker

  (example: assume we want to run saladmaker1: `./saladmaker -t1 1 -t2 3 **-i o** -s 12345`)
