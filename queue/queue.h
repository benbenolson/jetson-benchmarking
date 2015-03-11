#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdio.h>

struct Queue
{
  void *data;
  struct Queue *next, *prev;
};

void queue_print(struct Queue **queue);
void queue_init(struct Queue **queue);
void *queue_pop(struct Queue **queue);
void queue_push(struct Queue **queue, void *data);

#endif
