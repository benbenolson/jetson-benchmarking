#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdio.h>

struct Queue
{
  void *data;
  struct Queue *next, *prev;
};

void queue_print(struct Queue **queue)
{
  struct Queue *tmp = *queue;

  if(!tmp) {
    fprintf(stderr, "Oh shit.\n");
    exit(1);
  } else {
    if(tmp->next) {
      printf("data = %p\n", tmp->data);
      while(tmp->next) {
        tmp = tmp->next;
        printf("data = %p\n", tmp->data);
      }

    } else {
      printf("(first) data = %p\n", tmp->data);
    }
  }
}

void queue_init(struct Queue **queue)
{
  struct Queue *tmp = *queue;
  tmp->data = NULL;
  tmp->next = NULL;
  tmp->prev = NULL;
}

void *queue_pop(struct Queue **queue)
{
  void *retval = (*queue)->data;
  
  if((*queue)->next) {
    (*queue)->data = NULL;
    *queue = (*queue)->next;
    (*queue)->prev = NULL;
  } else {
    (*queue)->data = NULL;
    (*queue)->next = NULL;
  }
  
  if(retval == NULL) {
    fprintf(stderr, "I'm about to return a NULL pointer. Why?\n");
    exit(1);
  }
  return retval;
}

void queue_push(struct Queue **queue, void *data)
{
  struct Queue *tmp = *queue;

  if(!tmp) {
    fprintf(stderr, "Oh shit.\n");
    exit(1);
  }

  // Find the last element in the queue
  while(tmp->next) {
    tmp = tmp->next;
  }
  
  // Allocate more room for the element
  if(tmp->data == NULL) {
    tmp->data = data;
  } else {
    tmp->next = malloc(sizeof(struct Queue));
    tmp->next->data = data;
    tmp->next->prev = tmp;
  }
}

#endif
