#ifndef TIMING_H
#define TIMING_H

#include <stdio.h>
#include <sys/time.h>

#define timing(a) gettimeofday(&before, NULL); a; gettimeofday(&after, NULL); timedifference(&after, &before);

struct timeval before;
struct timeval after;

void timedifference(struct timeval *x, struct timeval *y);

#endif
