#include "threadpool.h"

void *wait_thread(void *t)
{
  struct Thread *thread = (struct Thread *)t;
  struct Threadpool *threadpool = thread->threadpool;
  struct Task *task;
  void *retval;
  
  ++(threadpool->size);

  while(!(threadpool->shutdown)) {
    // Wait on a task to appear
    pthread_mutex_lock(threadpool->tasklock);
    while((threadpool->pending == 0) && (!threadpool->shutdown)) {
      ++(threadpool->waiting);
      pthread_cond_wait(threadpool->cond, threadpool->tasklock);
      --(threadpool->waiting);
    }
    
    --(threadpool->pending);
    task = (struct Task *)queue_pop(&(threadpool->tasks));

    // Release the threadpool's lock
    pthread_mutex_unlock(threadpool->tasklock);
    
    // Run the program
    retval = task->function(task->args);

    // Clean up the task
    pthread_mutex_lock(threadpool->tasklock);
    free(task);
    pthread_mutex_unlock(threadpool->tasklock);
  }
  return retval;
}

void task_create(struct Threadpool *threadpool, void *(*function)(void *), void *args)
{
  pthread_mutex_lock(threadpool->tasklock);
  struct Task *task = malloc(sizeof(struct Task));
  task->function = function;
  task->args = args;
  
  // Place the task in the queue
  queue_push(&(threadpool->tasks), (void *)(task));
  ++(threadpool->pending);

  // Signal the threadpool that a task has been added
  pthread_cond_signal(threadpool->cond);
  pthread_mutex_unlock(threadpool->tasklock);
}

void thread_create(struct Thread **thread, struct Threadpool *threadpool)
{
  pthread_attr_t attr;
  pthread_attr_init(&attr);

  // Lock the thread array
  pthread_mutex_lock(threadpool->threadlock);

  // Initialize the thread struct
  *thread = malloc(sizeof(struct Thread));
  (*thread)->threadpool = threadpool;
  (*thread)->tid = malloc(sizeof(pthread_t));
  
  // Start the thread
  pthread_create((*thread)->tid, &attr, &wait_thread, (void *)(*thread));

  // Now unlock the thread array
  pthread_mutex_unlock(threadpool->threadlock);
}

void threadpool_create(struct Threadpool *threadpool, int numthreads)
{
  struct Thread **thread;

  // Set the number of threads
  NUMTHREADS = numthreads;

  //Initialize the threadpool
  threadpool->shutdown = 0;
  threadpool->pending = 0;
  threadpool->waiting = 0;
  threadpool->size = 0;
  threadpool->tasklock = malloc(sizeof(pthread_mutex_t));
  threadpool->threadlock = malloc(sizeof(pthread_mutex_t));
  threadpool->cond = malloc(sizeof(pthread_cond_t));
  threadpool->tasks = malloc(sizeof(struct Queue));
  queue_init(&(threadpool->tasks));
  threadpool->threads = malloc(sizeof(struct Thread *) * numthreads);

  // Initialize the mutex and cond
  pthread_mutex_init(threadpool->tasklock, NULL);
  pthread_mutex_init(threadpool->threadlock, NULL);
  pthread_cond_init(threadpool->cond, NULL);

  // Create the threads and put them in the array
  thread = threadpool->threads;
  for(int i = 0; i < NUMTHREADS; ++i) {
    thread_create(thread, threadpool);
    ++thread;
  }
}

void threadpool_end(struct Threadpool *threadpool)
{
  struct Thread **thread;
  while(threadpool->pending != 0) {}
  pthread_mutex_lock(threadpool->tasklock);
  threadpool->shutdown = 1;
  
  // Grab the locks
  pthread_mutex_lock(threadpool->threadlock);
  pthread_cond_signal(threadpool->cond);

  thread = threadpool->threads;
  for(int i = 0; i < NUMTHREADS; ++i) {
    pthread_join(*((*thread)->tid), NULL);
    free((*thread)->tid);
    free(*thread);
    ++thread;
  }

  // Clean up
  free(threadpool->tasklock);
  free(threadpool->threadlock);
  free(threadpool->cond);
  free(threadpool->tasks);
  free(threadpool->threads);
}

void threadpool_sync(struct Threadpool *threadpool)
{
  while(1) {
    pthread_mutex_lock(threadpool->tasklock);
    if((threadpool->pending == 0) && (threadpool->waiting == NUMTHREADS)) {
      pthread_mutex_unlock(threadpool->tasklock);
      return;
    }
    pthread_mutex_unlock(threadpool->tasklock);
  }
}
