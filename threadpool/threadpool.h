#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>
#include "../queue/queue.h"

#define NUMTHREADS 4

struct Task
{
  pthread_mutex_t *lock;
  void *args;
  void *(*function)(void *);
};

struct Thread
{
  pthread_t tid;
  pthread_mutex_t *lock;
  pthread_cond_t *ready;
  struct Threadpool *threadpool;
};

struct Threadpool
{
  int size;
  int shutdown;
  pthread_mutex_t *lock;
  pthread_cond_t *cond;
  struct Queue *tasks;
  struct Thread *threads;
};

void *wait_thread(void *t)
{
  struct Thread *thread = (struct Thread *)t;
  struct Threadpool *threadpool = thread->threadpool;
  struct Task *task;

  while(!threadpool->shutdown) {
    // Signal that you're ready
    pthread_mutex_unlock(thread->lock);

    // Wait on a task to appear
    pthread_cond_wait(threadpool->cond, threadpool->lock);
    
    // Pop the task off the queue
    task = (struct Task *)queue_pop(&(threadpool->tasks));
    printf("Grabbed a task:\n");
    queue_print(&(threadpool->tasks));
    fflush(stdout);
    
    // Release the threadpool's lock
    pthread_mutex_unlock(threadpool->lock);
    
    // Run the program
    *(task->function)(task->args);
  }
  printf("Killing thread %lu\n", thread->tid);
  return NULL;
}

void task_create(struct Threadpool *threadpool, void *(*function)(void *), void *args)
{
  struct Task *task = malloc(sizeof(struct Task));
  
  // Create the task
  task->function = function;
  task->args = args;
  task->lock = malloc(sizeof(pthread_mutex_t));
  pthread_mutex_init(task->lock, NULL);
  
  // Lock everything up
  pthread_mutex_lock(threadpool->lock);

  // Place the task in the queue
  queue_push(&(threadpool->tasks), (void *)(&task));

  printf("A task is now ready:\n");
  fflush(stdout);
  queue_print(&(threadpool->tasks));

  // Signal the threadpool that a task has been added
  printf("SIGNAL\n");
  pthread_cond_signal(threadpool->cond);
  pthread_mutex_unlock(threadpool->lock);
}

void thread_create(struct Thread *thread, struct Threadpool *threadpool)
{
  // Initialize the thread struct
  thread->threadpool = threadpool;
  thread->lock = malloc(sizeof(pthread_mutex_t));
  thread->ready = malloc(sizeof(pthread_cond_t));
  pthread_mutex_init(thread->lock, NULL);
  pthread_cond_init(thread->ready, NULL);
  
  // Start the thread
  pthread_mutex_lock(thread->lock);
  pthread_create(&(thread->tid), NULL, &wait_thread, (void *)thread);

  // Put the thread in the pool
  ++(threadpool->size);
}

void threadpool_create(struct Threadpool *threadpool)
{
  struct Thread *thread;

  //Initialize the threadpool
  threadpool->size = 0;
  threadpool->shutdown = 0;
  threadpool->lock = malloc(sizeof(pthread_mutex_t));
  threadpool->cond = malloc(sizeof(pthread_cond_t));
  threadpool->tasks = malloc(sizeof(struct Queue));
  queue_init(&(threadpool->tasks));
  threadpool->threads = malloc(sizeof(struct Thread) * NUMTHREADS);

  // Initialize the mutex and cond
  pthread_mutex_init(threadpool->lock, NULL);
  pthread_cond_init(threadpool->cond, NULL);

  // Create the threads and put them in the array
  thread = threadpool->threads;
  for(int i = 0; i < NUMTHREADS; ++i) {
    thread_create(thread, threadpool);
    ++thread;
  }

  while(threadpool->size != NUMTHREADS) {}

  // Wait for the threads to be ready
  thread = threadpool->threads;
  for(int i = 0; i < NUMTHREADS; ++i) {
    pthread_mutex_lock(thread->lock);
    pthread_mutex_unlock(thread->lock);
    ++thread;
  }
}

void threadpool_end(struct Threadpool *threadpool)
{
  struct Thread *thread;
  // Wait on all the threads to finish
  for(int i = 0; i < NUMTHREADS; ++i) {
    thread = threadpool->threads + i;
    printf("Waiting for thread %lu to end.\n", thread->tid);
    pthread_join(thread->tid, NULL);
  }
}

#endif
