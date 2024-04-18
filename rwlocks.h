#ifndef __rwlocks_h__
#define __rwlocks_h__

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#include "common.h"
#include "common_threads.h"
#include "chash.h"

typedef struct _rwlock_t {
    sem_t writelock;
    sem_t lock;
    int readers;
} rwlock_t;

void rwlock_init(rwlock_t *lock); //initialze values in new lock

void rwlock_acquire_readlock(rwlock_t *lock); //acquire the read lock

void rwlock_release_readlock(rwlock_t *lock); //release the write lock

void rwlock_acquire_writelock(rwlock_t *lock); //acquire the read lock

void rwlock_release_writelock(rwlock_t *lock); //release the write lock

void countAcquired();

void countReleased();

extern rwlock_t mutex;
extern rwlock_t count;
extern int num_lock_aq;
extern int num_lock_rel;

#endif // __rwlocks_h__