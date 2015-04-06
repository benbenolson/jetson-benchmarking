#include "threadpool.h"

struct Args
{
  int number;
};

void *testfunc(void *args)
{
  int test = 0;
  for(int i = 0; i < 100000; ++i) {
    ++test;
  }
  printf("Finished.\n");
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
  threadpool_create(&threadpool, 10);
  for(int i = 0; i < 2; ++i) {
    task_create(&threadpool, &testfunc, args);
  }
  threadpool_sync(&threadpool);
  for(int i = 0; i < 2; ++i) {
    task_create(&threadpool, &testfunc, args);
  }
  threadpool_sync(&threadpool);
  free(args);
  free(args2);
  free(args3);
}
