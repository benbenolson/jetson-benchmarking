#ifndef TRANSFORM_NOTHREAD_H
#define TRANSFORM_NOTHREAD_H

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

struct Gamargs
{
  int width, height, depth;
  float gam, prevgam;
  unsigned char *pixmap, *pixmapmod;
};

void invert_colors(unsigned char *pixmap, int width, int height, int depth);
void *apply_gamma(void *args);

#endif
