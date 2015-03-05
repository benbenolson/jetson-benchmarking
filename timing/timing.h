#ifndef TIMING_H
#define TIMING_H

#define timing(a) gettimeofday(&before, NULL); a; gettimeofday(&after, NULL); timedifference(&after, &before);

struct timeval before;
struct timeval after;

void timedifference(struct timeval *x, struct timeval *y)
{
  if (x->tv_usec < y->tv_usec) {
    int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
    y->tv_usec -= 1000000 * nsec;
    y->tv_sec += nsec;
  }
  if (x->tv_usec - y->tv_usec > 1000000) {
    int nsec = (x->tv_usec - y->tv_usec) / 1000000;
    y->tv_usec += 1000000 * nsec;
    y->tv_sec -= nsec;
  }

  printf("The operation took %ld seconds and %ld microseconds.\n", x->tv_sec - y->tv_sec, x->tv_usec - y->tv_usec);
}

#endif
