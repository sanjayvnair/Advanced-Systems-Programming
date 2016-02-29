#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "sem.h"


typedef struct semaphore {
    int id;
    pthread_mutex_t lock;
    pthread_cond_t nonzero;
    int count;
} semaphore_t;

              semaphore_t *
              semaphore_create(char *semaphore_name, int init, int ID)
              {
              int fd;
                  semaphore_t *semap;
                  pthread_mutexattr_t psharedm;
                  pthread_condattr_t psharedc;


                  fd = open(semaphore_name, O_RDWR | O_CREAT, 0666);
                  if (fd < 0)
                      return (NULL);
                  (void) ftruncate(fd, sizeof(semaphore_t));
                  (void) pthread_mutexattr_init(&psharedm);
                  (void) pthread_mutexattr_setpshared(&psharedm,
                      PTHREAD_PROCESS_SHARED);
                  (void) pthread_condattr_init(&psharedc);
                  (void) pthread_condattr_setpshared(&psharedc,
                      PTHREAD_PROCESS_SHARED);
                  semap = (semaphore_t *) mmap(NULL, sizeof(semaphore_t),
                          PROT_READ | PROT_WRITE, MAP_SHARED,
                          fd, 0);
                  close (fd);
                  (void) pthread_mutex_init(&semap->lock, &psharedm);
                  (void) pthread_cond_init(&semap->nonzero, &psharedc);

                  //Initializes the count of the semaphore based on the parameter

                  semap->count = init;
                  
                  // Creation of  ID for each file created for debugging purpose

                  semap->id = ID;
                 
                  
                  return (semap);
              }


              semaphore_t *
              semaphore_open(char *semaphore_name)
              {
                  int fd;
                  semaphore_t *semap;


                  fd = open(semaphore_name, O_RDWR, 0666);
                  if (fd < 0)
                      return (NULL);
                  semap = (semaphore_t *) mmap(NULL, sizeof(semaphore_t),
                          PROT_READ | PROT_WRITE, MAP_SHARED,
                          fd, 0);
                  close (fd);
                  return (semap);
              }


              void
              semaphore_post(semaphore_t *semap)
              {

                  pthread_mutex_lock(&semap->lock);
                  semap->count++;
                  if (semap->count <= 0) {
                    pthread_cond_broadcast(&semap->nonzero);
                  }
                  
                  
                  pthread_mutex_unlock(&semap->lock);

              }


              void
              semaphore_wait(semaphore_t *semap)
              {

                  pthread_mutex_lock(&semap->lock);
                  semap->count--;
                  if ((semap->count) < 0)
                      pthread_cond_wait(&semap->nonzero, &semap->lock);
                  pthread_mutex_unlock(&semap->lock);
              }


              //Unmapping of the files that are mapped on to the memory
              void
              semaphore_close(semaphore_t *semap)
              {
                  munmap((void *) semap, sizeof(semaphore_t));
              }

              //Printing contents of each instance of the structure created
              
              void
              semaphore_print(semaphore_t *semap)
              {
                  printf("COUNT == %d\n", semap->count );
                  printf("ID == %d\n", semap->id );
              }

              