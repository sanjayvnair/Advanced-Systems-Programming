#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h> 
#include <pthread.h> 
#include "sem.h"

int num_children = 1;
int iterations = 3;
pid_t pids[1000];
int i,j;
int n = 10;
int pid;
int count = 0;
char phil_args1[5];
char phil_args2[5];
int main(int argc, char *argv[]){
	int fd_state,fd_count;
    struct stat mystat;
    char *map_state;
	semaphore_t *semap_mutex;
	semaphore_t *semap[atoi(argv[1])];
	int pid_parent = getpid();
	int processes = atoi(argv[1]);
	char string[] = "./semaphore";
    char filename[50];
    printf("pid ==== %d\n", getpid() );
    system("rm -rf sema* state.txt");
    
    //Creating a state file
    fd_state = open("state.txt", O_RDWR | O_CREAT | O_EXCL, 0666);
    
       
    if (fd_state == -1){
        perror("Open  error in state file");
        exit(1);
    }

    

    char temp[processes];
	int i;
	for( i = 0; i < processes; i++){
		temp[i] = 'T';
	}
	write(fd_state, temp, processes);

    if (fstat(fd_state,&mystat) < 0){
        perror("fstat");
        close(fd_state);
        exit(1);
    }
    
    map_state = mmap(0,mystat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_state, 0);
    
    
    if (map_state == MAP_FAILED){
        perror("mmap");
        close(fd_state);
        exit(1);
    }

    //Create initial state as thinking
    
    for( i = 0 ; i < processes ; i++){
    	map_state[i] = 'T';
    }

    

   	semap_mutex = semaphore_create(string,1,99);
   	
	for(i=0 ; i < processes; i++){
        sprintf(filename,"%s%d",string,i);
        semap[i] = semaphore_create(filename,0,i + 1);
    }

    //Creating the processes(philosophers)

	for (i = 0; i < processes; ++i) {
		sprintf(phil_args1,"%d",atoi(argv[2]));
		sprintf(phil_args2,"%d",i);
		if ((pids[i] = fork()) < 0) {
    	perror("fork");
    	abort();
  		}else if (pids[i] == 0) {
  			printf("PROCESS NO ====== %d  CREATED  \n", i + 1 );
  			sleep(1);	
  			if(execlp("./phil","philosopher",&phil_args1,&phil_args2,argv[1],NULL) == -1){
  				printf("Error\n");
  				exit(1);
  			}

  		}
  		count++;
	}

	//Wait for all child processes to end

	if (getpid() == pid_parent){
		for (i = 0; i < processes; ++i) {
			waitpid(-1, NULL, 0);
		}
		munmap((void *) map_state, sizeof(map_state));
		close (fd_state);
		printf("\n\n--------------------------------DINNER COMPLETE------------------------------\n\n\n");
		exit(0);	
	}


	
	
	
}