#ifndef QUEUE_H
#define QUEUE_H

struct queue
{
  void *data;
  queue *next;
};

void *queue_get(queue **element)
{
  void *retval = (*element)->data;
  *element = (*element)->next;
  return (*element)->data;
}

void queue_push(queue **element, void *data)
{
  // Find the last element in the queue
  queue *tmp = *element;
  while(tmp->next != NULL) {
    tmp = tmp->next;
  }

  // Allocate more room for the element
  (*element) = malloc(sizeof(void *) + (sizeof(queue *));
  
  // Now fill it up and place it in the queue
  (*element)->data = data;
  tmp->next = *element;
}

#endif
