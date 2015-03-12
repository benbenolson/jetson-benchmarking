#include "threadpool.h"

struct Args
{
  int number;
};

void *testfunc(void *args)
{
  printf("The argument is %d\n", (*(int *)args));
  return NULL;
}

int main()
{
  struct Threadpool threadpool;
  struct Args *args;
  struct Args *args2;
  struct Args *args3;
  args = malloc(sizeof(struct Args));
  args2 = malloc(sizeof(struct Args));
  args3 = malloc(sizeof(struct Args));
  args->number = 1;
  args2->number = 2;
  args3->number = 3;
  threadpool_create(&threadpool, 4);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  task_create(&threadpool, &testfunc, args);
  threadpool_end(&threadpool);
}
