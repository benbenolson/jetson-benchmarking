#include "queue.h"

int main()
{
  int number = 1;
  int number2 = 2;
  int number3 = 3;
  int number4 = 4;
  struct Queue *queue = malloc(sizeof(struct Queue));
  queue_init(&queue);
  queue_push(&queue, (void *)(&number));
  queue_push(&queue, (void *)(&number2));
  printf("%d\n", *(int *)queue_pop(&queue));
  printf("%d\n", *(int *)queue_pop(&queue));
  queue_push(&queue, (void *)(&number3));
  printf("%d\n", *(int *)queue_pop(&queue));
  queue_push(&queue, (void *)(&number4));
  printf("%d\n", *(int *)queue_pop(&queue));

}
