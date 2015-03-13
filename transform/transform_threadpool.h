#ifndef TRANSFORM_THREADPOOL_H
#define TRANSFORM_THREADPOOL_H

#include <math.h>
#include <pthread.h>
#include "../threadpool/threadpool.h"

struct Threadpool *threadpool;

struct Args
{
  int width, height, depth;
  float gam;
  unsigned char *pixmap, *pixmapmod;
};

void invert_colors(unsigned char *pixmap, int width, int height, int depth);
void *gamma_subset(void *args);
void apply_gamma(unsigned char *pixmap, unsigned char *pixmapmod, int width, int height, int depth, float gam);

#endif
