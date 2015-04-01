#include "threadpool.h"

void *wait_thread(void *t)
{
  struct Thread *thread = (struct Thread *)t;
  struct Threadpool *threadpool = thread->threadpool;
  struct Task *task;
  void *retval;
  
  pthread_mutex_lock(threadpool->sizelock);
  ++(threadpool->size);
  pthread_mutex_unlock(threadpool->sizelock);

  while(!(threadpool->shutdown)) {
    // Wait on a task to appear
    pthread_mutex_lock(threadpool->tasklock);
    ++(threadpool->waiting);
    pthread_cond_wait(threadpool->cond, threadpool->tasklock);
    --(threadpool->waiting);
    
    if(threadpool->shutdown) {
      printf("Dying\n");
      return NULL;
    }
    // Pop the task off the queue
    task = (struct Task *)queue_pop(&(threadpool->tasks));

    // Release the threadpool's lock
    pthread_mutex_unlock(threadpool->tasklock);
    
    // Run the program
    retval = task->function(task->args);

    // Clean up the task
    free(task);

    // Update pending
    pthread_mutex_lock(threadpool->pendinglock);
    --(threadpool->pending);
    pthread_mutex_unlock(threadpool->pendinglock);
  }
  printf("Thread dying\n");
  return retval;
}

void task_create(struct Threadpool *threadpool, void *(*function)(void *), void *args)
{
  // Wait for the threads to be created before creating any tasks
  while(1) {
    pthread_mutex_lock(threadpool->sizelock);
    if(threadpool->size == NUMTHREADS) {
      printf("Size is 4. Time to start making tasks.\n");
      pthread_mutex_unlock(threadpool->sizelock);
      break;
    }
    pthread_mutex_unlock(threadpool->sizelock);
  }

  // Grab the tasklock
  pthread_mutex_lock(threadpool->tasklock);
  
  struct Task *task = malloc(sizeof(struct Task));

  task->function = function;
  task->args = args;
  
  // Place the task in the queue
  queue_push(&(threadpool->tasks), (void *)(task));
  pthread_mutex_lock(threadpool->pendinglock);
  ++(threadpool->pending);
  pthread_mutex_unlock(threadpool->pendinglock);

  // Signal the threadpool that a task has been added
  pthread_cond_signal(threadpool->cond);
  pthread_mutex_unlock(threadpool->tasklock);
  printf("Signalled that a task is ready.\n");
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
  (*thread)->lock = malloc(sizeof(pthread_mutex_t));
  (*thread)->ready = malloc(sizeof(pthread_cond_t));
  (*thread)->tid = malloc(sizeof(pthread_t));
  pthread_mutex_init((*thread)->lock, NULL);
  pthread_cond_init((*thread)->ready, NULL);
  
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
  threadpool->sizelock = malloc(sizeof(pthread_mutex_t));
  threadpool->waitinglock = malloc(sizeof(pthread_mutex_t));
  threadpool->threadlock = malloc(sizeof(pthread_mutex_t));
  threadpool->pendinglock = malloc(sizeof(pthread_mutex_t));
  threadpool->cond = malloc(sizeof(pthread_cond_t));
  threadpool->tasks = malloc(sizeof(struct Queue));
  queue_init(&(threadpool->tasks));
  threadpool->threads = malloc(sizeof(struct Thread *) * numthreads);

  // Initialize the mutex and cond
  pthread_mutex_init(threadpool->tasklock, NULL);
  pthread_mutex_init(threadpool->sizelock, NULL);
  pthread_mutex_init(threadpool->waitinglock, NULL);
  pthread_mutex_init(threadpool->threadlock, NULL);
  pthread_mutex_init(threadpool->pendinglock, NULL);
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
    printf("Waiting on thread %lu to die.\n", *((*thread)->tid));
    pthread_join(*((*thread)->tid), NULL);
    free((*thread)->tid);
    free((*thread)->lock);
    free((*thread)->ready);
    free(*thread);
    ++thread;
  }

  // Clean up
  free(threadpool->tasklock);
  free(threadpool->threadlock);
  free(threadpool->pendinglock);
  free(threadpool->cond);
  free(threadpool->tasks);
  free(threadpool->threads);
}

void threadpool_sync(struct Threadpool *threadpool)
{
  while(1) {
    pthread_mutex_lock(threadpool->pendinglock);
    if(threadpool->pending == 0) {
      pthread_mutex_unlock(threadpool->pendinglock);
      return;
    }
    pthread_mutex_unlock(threadpool->pendinglock);
  }
}
