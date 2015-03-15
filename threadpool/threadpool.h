#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>
#include "../queue/queue.h"

int NUMTHREADS;

struct Task
{
  pthread_mutex_t *lock;
  void *args;
  void *(*function)(void *);
};

struct Thread
{
  pthread_t *tid;
  pthread_mutex_t *lock;
  pthread_cond_t *ready;
  struct Threadpool *threadpool;
};

struct Threadpool
{
  int size;
  int shutdown;
  int pending;
  pthread_mutex_t *tasklock;
  pthread_mutex_t *threadlock;
  pthread_cond_t *cond;
  struct Queue *tasks;
  struct Thread **threads;
};

void *wait_thread(void *t);
void task_create(struct Threadpool *threadpool, 
                 void *(*function)(void *),
                 void *args);
void thread_create(struct Thread **thread,
                   struct Threadpool *threadpool);
void threadpool_create(struct Threadpool *threadpool,
                       int numthreads);
void threadpool_end(struct Threadpool *threadpool);

#endif
