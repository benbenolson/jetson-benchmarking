extern "C" {
  #include "transform_cuda.h"
}

/*********************************
*         GAMMA                  *
*********************************/

// Function for each thread to run
void *cpu_gamma_subset(void *args)
{
  struct Cpuargs *pic = (struct Cpuargs *)args;
  unsigned char tmp;
  for(int i = 0; i < pic->size; ++i) {
    tmp = pow((float)(*(pic->pixmap)) / 255, (float)(1 / pic->gam)) * 255;
    if((tmp > *(pic->pixmapmod)) && (pic->gam < pic->prevgam) && (pic->gam < 1.0)) {
      tmp = 0;
    }
    if((tmp < *(pic->pixmapmod)) && (pic->gam > pic->prevgam) && (pic->gam > 1.0)) {
      tmp = 255;
    }
    *(pic->pixmapmod) = tmp;
    ++(pic->pixmap);
    ++(pic->pixmapmod);
  }
  return NULL;
}


//cuda kernel
__global__
void gpu_gamma_subset(void *args)
{
  int i = blockDim.x * blockIdx.x + threadIdx.x;
  struct Gpuargs *pic = (struct Gpuargs *)args;
  unsigned char *pixmap = (unsigned char *)pic->pixmap;
  unsigned char *pixmapmod = (unsigned char *)pic->pixmapmod;
  unsigned char tmp;

  if(i < (pic->size)) {
    tmp = pow((float)(*(pixmap + i)) / 255, (float)(1 / pic->gam)) * 255;
    if((tmp > *(pixmapmod + i)) && (pic->gam < pic->prevgam) && (pic->gam < 1.0)) {
      tmp = 0;
    }
    if((tmp < *(pixmapmod + i)) && (pic->gam > pic->prevgam) && (pic->gam > 1.0)) {
      tmp = 255;
    }
    *(pixmapmod + i) = tmp;
  }
}

extern "C"
void *apply_gamma(void *args)
{
  struct Gamargs *oldargs = (struct Gamargs *)args;
  unsigned char *pixmap = oldargs->pixmap;
  unsigned char *pixmapmod = oldargs->pixmapmod;

  // Figure how much of the image the CPU/GPU is doing
  int gpusize, cpusize;
  if(oldargs->depth == 24) {
    gpusize = oldargs->height * oldargs->width * 4 * oldargs->split;
    cpusize = (oldargs->height * oldargs->width * 4) - gpusize;
  }

  // Set up data structures for the CPU threads
  int numthreads = oldargs->numthreads;
  struct Cpuargs *cpuargs;
  pthread_t *tids = (pthread_t *)malloc(sizeof(pthread_t) * numthreads);

  // Create all of the threads
  for(int i = 0; i < numthreads; ++i) {
    cpuargs = (struct Cpuargs *)malloc(sizeof(struct Cpuargs));
    cpuargs->size = cpusize / numthreads;
    cpuargs->depth = oldargs->depth;
    cpuargs->gam = oldargs->gam;
    cpuargs->prevgam = oldargs->prevgam;
    cpuargs->pixmap = oldargs->pixmap + gpusize + (i * (cpuargs->size));
    cpuargs->pixmapmod = oldargs->pixmapmod + gpusize + (i * (cpuargs->size));
    pthread_create(tids, NULL, cpu_gamma_subset, cpuargs);
    ++tids;
  }
  for(int i = 0; i < numthreads; ++i) {
    --tids;
  }
  
  // Set up the GPU data structures
  struct Gpuargs *gpuargs, *d_gpuargs;
  gpuargs = (struct Gpuargs *)malloc(sizeof(struct Gpuargs));
  gpuargs->depth = oldargs->depth;
  gpuargs->size = gpusize;
  gpuargs->gam = oldargs->gam;
  gpuargs->prevgam = oldargs->prevgam;

  // Allocate and move data to the GPU
  cudaMalloc((void **)(&d_gpuargs), sizeof(struct Gpuargs));
  cudaMalloc((void **)(&gpuargs->pixmap), sizeof(unsigned char) * gpuargs->size);
  cudaMalloc((void **)(&gpuargs->pixmapmod), sizeof(unsigned char) * gpuargs->size);
  cudaMemcpy(d_gpuargs, gpuargs, sizeof(struct Gpuargs), cudaMemcpyHostToDevice);
  cudaMemcpy(gpuargs->pixmap, pixmap, sizeof(unsigned char) * gpuargs->size, cudaMemcpyHostToDevice);
  cudaMemcpy(gpuargs->pixmapmod, pixmapmod, sizeof(unsigned char) * gpuargs->size, cudaMemcpyHostToDevice);

  // Run!
  gpu_gamma_subset<<<(gpuargs->size + (oldargs->threadsperblock -1)) / oldargs->threadsperblock, oldargs->threadsperblock>>>((void *)d_gpuargs);

  // Pass the data back
  cudaMemcpy(oldargs->pixmapmod, gpuargs->pixmapmod, sizeof(unsigned char) * gpuargs->size, cudaMemcpyDeviceToHost);

  // Sync all of the threads
  for(int i = 0; i < numthreads; ++i) {
    pthread_join(*tids, NULL);
    ++tids;
  }

  return NULL;
}
