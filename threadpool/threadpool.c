#include "threadpool.h"

void *wait_thread(void *t)
{
  struct Thread *thread = (struct Thread *)t;
  struct Threadpool *threadpool = thread->threadpool;
  struct Task *task;
  struct timespec timer;
  
  while(!(threadpool->shutdown) || (threadpool->pending != 0)) {
    // Wait on a task to appear
    pthread_mutex_lock(threadpool->tasklock);
    clock_gettime(CLOCK_REALTIME, &timer);
    timer.tv_nsec += 1;
    while(!(threadpool->shutdown) && (threadpool->pending == 0)) {
      printf("Thread %lu is waiting on a task still, %d\n", *(thread->tid), threadpool->pending);
      pthread_cond_timedwait(threadpool->cond, threadpool->tasklock, &timer);
      printf("Tired of waiting.\n");
    }
    
    if((threadpool->pending == 0) && (threadpool->shutdown)) {
      pthread_mutex_unlock(threadpool->tasklock);
      return NULL;
    }

    // Pop the task off the queue
    task = (struct Task *)queue_pop(&(threadpool->tasks));
    --(threadpool->pending);

    // Release the threadpool's lock
    pthread_mutex_unlock(threadpool->tasklock);
    
    // Run the program
    *(task->function)(task->args);
  }
  printf("THREAD DYING\n");
  return NULL;
}

void task_create(struct Threadpool *threadpool, void *(*function)(void *), void *args)
{
  // Lock up to let everyone know that a new task is on the way
  pthread_mutex_lock(threadpool->tasklock);

  struct Task *task = malloc(sizeof(struct Task));

  task->function = function;
  task->args = args;
  task->lock = malloc(sizeof(pthread_mutex_t));
  pthread_mutex_init(task->lock, NULL);
  
  // Place the task in the queue
  queue_push(&(threadpool->tasks), (void *)(task));
  ++(threadpool->pending);

  // Signal the threadpool that a task has been added
  pthread_cond_signal(threadpool->cond);

  // Unlock to allow the threads to eat the task up
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
  (*thread)->lock = malloc(sizeof(pthread_mutex_t));
  (*thread)->ready = malloc(sizeof(pthread_cond_t));
  (*thread)->tid = malloc(sizeof(pthread_t));
  pthread_mutex_init((*thread)->lock, NULL);
  pthread_cond_init((*thread)->ready, NULL);
  
  // Start the thread
  pthread_create((*thread)->tid, &attr, &wait_thread, (void *)(*thread));

  // Put the thread in the pool
  ++(threadpool->size);

  // Now unlock the thread array
  pthread_mutex_unlock(threadpool->threadlock);
}

void threadpool_create(struct Threadpool *threadpool, int numthreads)
{
  struct Thread **thread;

  // Set the number of threads
  NUMTHREADS = numthreads;

  //Initialize the threadpool
  threadpool->size = 0;
  threadpool->shutdown = 0;
  threadpool->pending = 0;
  threadpool->tasklock = malloc(sizeof(pthread_mutex_t));
  threadpool->threadlock = malloc(sizeof(pthread_mutex_t));
  threadpool->cond = malloc(sizeof(pthread_cond_t));
  threadpool->tasks = malloc(sizeof(struct Queue));
  queue_init(&(threadpool->tasks));
  threadpool->threads = malloc(sizeof(struct Thread *) * numthreads);
  printf("Allocated %d bytes for threads\n", sizeof(struct Thread *) * numthreads);

  // Initialize the mutex and cond
  pthread_mutex_init(threadpool->tasklock, NULL);
  pthread_mutex_init(threadpool->threadlock, NULL);
  pthread_cond_init(threadpool->cond, NULL);

  // Create the threads and put them in the array
  thread = threadpool->threads;
  for(int i = 0; i < NUMTHREADS; ++i) {
    thread_create(thread, threadpool);
    printf("Successfully created process %lu\n", *((*thread)->tid));
    ++thread;
  }

//  while(threadpool->size != NUMTHREADS) {}

  // Wait for the threads to be ready
  /*
  thread = threadpool->threads;
  for(int i = 0; i < NUMTHREADS; ++i) {
    pthread_mutex_lock((*thread)->lock);
    pthread_mutex_unlock((*thread)->lock);
    ++thread;
  }
  */
}

void threadpool_end(struct Threadpool *threadpool)
{
  struct Thread **thread;
  
  pthread_mutex_lock(threadpool->threadlock);
  threadpool->shutdown = 1;
  thread = threadpool->threads;
  for(int i = 0; i < NUMTHREADS; ++i) {
    printf("Waiting on a thread to die.\n");
    pthread_join(*((*thread)->tid), NULL);
    printf("Successfully joined thread %lu\n", *((*thread)->tid));
    ++thread;
  }
  pthread_mutex_unlock(threadpool->threadlock);
}
