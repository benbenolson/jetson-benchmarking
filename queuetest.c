#include "queue/queue.h"

int main()
{
  struct Queue *queue = malloc(sizeof(struct Queue));
  int number = 293874;
  int number2 = 435;
  queue_init(&queue);
  queue_push(&queue, (void *)&number2);
  queue_push(&queue, (void *)&number);
  queue_push(&queue, (void *)&number);
  queue_print(&queue);
  printf("%d\n", (*(int *)queue_pop(&queue)));
  queue_print(&queue);
  printf("%d\n", (*(int *)queue_pop(&queue)));
  queue_print(&queue);
  printf("%d\n", (*(int *)queue_pop(&queue)));
  queue_print(&queue);
}
