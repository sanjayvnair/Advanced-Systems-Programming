/* sem.h */
#ifndef sem_h
#define sem_h

#include "sem.c"



//typedef struct semaphore semaphore_t;

semaphore_t *semaphore_create(char *semaphore_name, int init, int ID);
semaphore_t *semaphore_open(char *semaphore_name);
void semaphore_post(semaphore_t *semap);
void semaphore_wait(semaphore_t *semap);
void semaphore_close(semaphore_t *semap);
#endif