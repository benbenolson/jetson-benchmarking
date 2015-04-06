#ifndef TRANSFORM_THREAD_H
#define TRANSFORM_THREAD_H

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

struct Gamargs
{
  int numthreads;
  int width, height, depth;
  float gam, prevgam;
  unsigned char *pixmap, *pixmapmod;
};

void invert_colors(unsigned char *pixmap, int width, int height, int depth);
void *gamma_subset(void *args);
void *apply_gamma(void *args);

#endif
