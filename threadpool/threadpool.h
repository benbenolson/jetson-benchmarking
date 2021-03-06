#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>
#include "../queue/queue.h"

int NUMTHREADS;

struct Task
{
  void *args;
  void *(*function)(void *);
};

struct Thread
{
  pthread_t *tid;
  struct Threadpool *threadpool;
};

struct Threadpool
{
  int size;
  int shutdown;
  int pending;
  int waiting;
  struct Queue *tasks;
  pthread_mutex_t *tasklock;
  pthread_cond_t *cond;
  struct Thread **threads;
  pthread_mutex_t *threadlock;
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
void threadpool_sync(struct Threadpool *threadpool);

#endif
