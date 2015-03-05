#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "queue.h"

struct Threadpool
{
  int size;
  queue *tasks;
  pthread_mutex_t *lock;
};

Threadpool *threadpool_create(Threadpool **threadpool)
{
  //Initialize the threadpool
  (*threadpool) = malloc(sizeof(int) + sizeof(queue *));
  pthread_mutex_init((*threadpool)->lock, NULL);
}

#endif
