#ifndef TRANSFORM_CUDA_H
#define TRANSFORM_CUDA_H

#include <math.h>
#include <stdio.h>
#include <cuda_runtime.h>
#include <pthread.h>

struct Cpuargs
{
  int size, depth;
  float gam, prevgam;
  unsigned char *pixmap, *pixmapmod;
};

struct Gpuargs
{
  int size, depth;
  float gam, prevgam;
  unsigned char *pixmap, *pixmapmod;
};

struct Gamargs
{
  int numthreads, threadsperblock;
  double split;
  int width, height, depth;
  float gam, prevgam;
  unsigned char *pixmap, *pixmapmod;
};

void invert_colors(unsigned char *pixmap, int width, int height, int depth);
__global__ void gamma_subset(void *args);
void *apply_gamma(void *args);

#endif
