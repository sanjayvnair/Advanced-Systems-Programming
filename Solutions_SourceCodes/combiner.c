/*
 ============================================================================
 Name        : Combiner.c
 Author      : Sanjay Nair
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

int main(int argc , char *argv[]) {
  pid_t pid;
  int pipefd[2];
  int ret;
  off_t size;

  ret = pipe(pipefd);

  if(ret == -1) {
    perror("piper error \n");
    exit(1);
  }

  pid = fork();
  // Executing the child process
  if (pid == 0){
    
    dup2(pipefd[1],STDOUT_FILENO);
    close(pipefd[1]);                           //closing unwanted connection
    execlp("./mapper","mapper",argv[1],NULL);   // Executing the mapper program 
    }

  // Exeuting the parent process
  else if(pid > 0 ){

      // Parent process wait till the first child execution
      wait();
      close(pipefd[1]);                         //Unwanted connection
      
      //Parent forking to create second child
      pid = fork();
      if (pid == 0)
      { 
        close(pipefd[1]);
        dup2(pipefd[0],STDIN_FILENO);
        close(pipefd[0]);
        execlp("./reducer","Reducer",NULL);     // Executing the reducer program 
        

      }else if (pid > 0)
      {
        exit(0);                                //Terminate the parent
      }else{
         //Terminate with error
         perror("Fork error\n");  
         exit(1);
      }
    
  }
  else{

    //Terminate with error
    perror("Fork error\n");
    exit(1);
  }
  return 0;
}






