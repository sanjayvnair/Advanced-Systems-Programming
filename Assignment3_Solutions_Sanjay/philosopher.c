#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sem.h"

#define READY 'R'
#define THINKING 'T'
#define HUNGRY 'H'
#define EATING 'E'
#define LEFT (ph_num + (num - 1))%num
#define RIGHT (ph_num + 1)%num  


semaphore_t *semap_mutex;
semaphore_t *semap[1000];
  
void philosopher_dining(int num);
void barrier(int num);
void take_fork(int);
void put_fork(int);
void test(int);
  
char *map_state;
int num_iterations;
int num;

  
int main(int argc, char *argv[])
{   
    
    num_iterations = atoi(argv[1]);
    int i;
    int id;
    int fd_state;
    struct stat mystat;
    id = atoi(argv[2]);
    num = atoi(argv[3]);
    fd_state = open ("state.txt", O_RDWR);
    
    if (fd_state == -1){
        perror("Open");
        exit(1);
    }

    if (fstat(fd_state,&mystat) < 0){
        perror("fstat");
        close(fd_state);
        exit(1);
    }

    //Mapping the state of the philsopher to a file state.txt

    map_state = mmap(0,mystat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_state, 0);

    if (map_state == MAP_FAILED){
        perror("mmap");
        close(fd_state);
        exit(1);
    }


    //File name definition

    char string[] = "./semaphore";
    char filename[50];
    
    semap_mutex = semaphore_open(string);

    //Creation of files for the semaphores---- one for each fork

    for (i = 0 ; i < num; i++){
        sprintf(filename,"%s%d",string,i);
        semap[i] = semaphore_open(filename);
   }
    
    printf("Philosopher no %d waiting at the barrier\n", id + 1 );
    barrier(id);

    if (id == 0){
        printf("BARRIER END\n\n");    
    }
    
    
    philosopher_dining (id);

    //Memory unmap after use

    semaphore_close(semap_mutex);
    semaphore_close(semap[id]);

    exit(0);
}
  
void philosopher_dining(int ph_num)
{   int iterations = 1;


    //Iterating the number of time a philospher can dine

    while( iterations <= num_iterations)
    {   
        
        int i = ph_num;
        sleep(1);
        take_fork(i);
        sleep(0);
        put_fork(i);
        if (iterations == num_iterations){
            printf("\n==================Philosopher %d has completed its %d iterations=======================\n\n", (i + 1) , iterations);
            break;
        }
        iterations++;

    }
 
}
  
void take_fork(int ph_num)
{
 
    semaphore_wait(semap_mutex);

    // Changes the state of the philosopher to hungry
    
    map_state[ph_num] = HUNGRY;
    printf("Philosopher %d -------------- HUNGRY\n\n",ph_num+1);

    //Checks if allowed to grab the fork
    test(ph_num);
    
    semaphore_post(semap_mutex);
    semaphore_wait(semap[ph_num]);
    sleep(1);
    
    
}


//Function to check if the philosopher is allowed to eat 

void test(int ph_num)
{

    if (map_state[ph_num] == HUNGRY && map_state[LEFT] != EATING && map_state[RIGHT] != EATING)
    {   
        //If allowed to grab the fork philosopher goes ahead and grabs them
        //Changes the state of the philosopher to eating

        map_state[ph_num] = EATING;
        sleep(2);
        printf("Philosopher %d grabs forks %d and %d\n",ph_num+1,LEFT+1,ph_num+1);
        printf("Philosopher %d -------------- EATING\n\n",ph_num+1);
 
        semaphore_post(semap[ph_num]);

    }
    
    
}
  
//Function to release the forks after eating and notifying adjacent philosophers about the release

void put_fork(int ph_num)
{
    semaphore_wait(semap_mutex);

    //Changes state of eating philosopher to thinking

    map_state[ph_num] = THINKING;
    printf("Philosopher %d releases forks %d and %d\n",ph_num+1,LEFT+1,ph_num+1);
    printf("Philosopher %d -------------- THINKING\n\n",ph_num+1);

    //Allowing the philosopher on the right and left to grab the fork

    test(LEFT);
    test(RIGHT);

    semaphore_post(semap_mutex);

}

void barrier(int id){
    sleep(4);
}
