#include "rwlocks.h"

rwlock_t mutex;
rwlock_t count;
int num_lock_aq = 0;
int num_lock_rel = 0;

void rwlock_init(rwlock_t *lock) {
    lock->readers = 0;
    Sem_init(&lock->lock, 1); 
    Sem_init(&lock->writelock, 1); 
}

void rwlock_acquire_readlock(rwlock_t *lock) {
    Sem_wait(&lock->lock);
    fprintf(outputFile,"READ LOCK ACQUIRED\n");
    count_acquire_writelock(&count);
    countAcquired();
    count_release_writelock(&count);
    lock->readers++;
    if (lock->readers == 1)
	Sem_wait(&lock->writelock);
    Sem_post(&lock->lock);
}

void rwlock_release_readlock(rwlock_t *lock) {
    Sem_wait(&lock->lock);
    fprintf(outputFile,"READ LOCK RELEASED\n");
    count_acquire_writelock(&count);
    countReleased();
    count_release_writelock(&count);
    lock->readers--;
    if (lock->readers == 0)
	Sem_post(&lock->writelock);
    Sem_post(&lock->lock);
}

void rwlock_acquire_writelock(rwlock_t *lock) {
    Sem_wait(&lock->writelock);
    fprintf(outputFile,"WRITE LOCK ACQUIRED\n");
    count_acquire_writelock(&count);
    countAcquired();
    count_release_writelock(&count);
}

void rwlock_release_writelock(rwlock_t *lock) {
    fprintf(outputFile,"WRITE LOCK RELEASED\n");
    count_acquire_writelock(&count);
    countReleased();
    count_release_writelock(&count);
    Sem_post(&lock->writelock);
}

void count_acquire_readlock(rwlock_t *lock) {
    Sem_wait(&lock->lock);
    lock->readers++;
    if (lock->readers == 1)
	Sem_wait(&lock->writelock);
    Sem_post(&lock->lock);
}

void count_release_readlock(rwlock_t *lock) {
    Sem_wait(&lock->lock);
    lock->readers--;
    if (lock->readers == 0)
	Sem_post(&lock->writelock);
    Sem_post(&lock->lock);
}

void count_acquire_writelock(rwlock_t *lock) {
    Sem_wait(&lock->writelock);
}

void count_release_writelock(rwlock_t *lock) {
    Sem_post(&lock->writelock);
}

void countAcquired()
{
    num_lock_aq++;
}

void countReleased()
{
    num_lock_rel++;
}

