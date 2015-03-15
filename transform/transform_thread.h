#ifndef TRANSFORM_THREAD_H
#define TRANSFORM_THREAD_H

#include <math.h>
#include <pthread.h>

struct thread_arguments
{
  int width, height, depth;
  float gam;
  unsigned char *pixmap, *pixmapmod;
};

void invert_colors(unsigned char *pixmap, int width, int height, int depth);
void *gamma_subset(void *args);
void apply_gamma(unsigned char *pixmap, unsigned char *pixmapmod, int width, int height, int depth, float gam);

#endif
