extern "C" {
  #include "transform_cuda.h"
}

/*********************************
*         INVERT_COLORS          *
*********************************/
void invert_colors(unsigned char *pixmap, int width, int height, int depth)
{
  unsigned char *beg = pixmap;
  if(depth == 24) {
    for(int i = 0; i < (height); ++i) {
      for(int n = 0; n < (width); ++n) {
        for(int x = 0; x < 3; ++x) {
          *pixmap = 255 - (*pixmap);
          ++pixmap;
        }
        ++pixmap;
      }
    }
  }
  pixmap = beg;
}

/*********************************
*         GAMMA                  *
*********************************/

__global__
void gamma_subset(void *args, void *d_pixmap, void *d_pixmapmod)
{
  int i = blockDim.x * blockIdx.x + threadIdx.x;
  struct Gamargs *pic = (struct Gamargs *)args;
  unsigned char *pixmap = (unsigned char *)d_pixmap;
  unsigned char *pixmapmod = (unsigned char *)d_pixmapmod;
  unsigned char tmp;

  if(pic->depth == 24) {
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
    struct Gamargs *newargs, *d_newargs;
    unsigned char *d_pixmap, *d_pixmapmod;
    int numpixels;

    newargs = (struct Gamargs *)malloc(sizeof(struct Gamargs));
    newargs->width = oldargs->width;
    newargs->height = oldargs->height;
    newargs->depth = oldargs->depth;
    newargs->gam = oldargs->gam;
    newargs->prevgam = oldargs->prevgam;
    newargs->pixmap = oldargs->pixmap;
    newargs->pixmapmod = oldargs->pixmapmod;
    
    if(newargs->depth == 24) {
      printf("Setting numpixels\n");
      numpixels = (newargs->width) * (newargs->height) * 4;
    }

    // Allocate the memory on the GPU and run the kernel
    cudaMalloc((void **)(&d_newargs), sizeof(struct Gamargs));
    cudaMalloc((void **)(&d_pixmap), sizeof(unsigned char) * 4 * newargs->height * newargs->width);
    cudaMalloc((void **)(&d_pixmapmod), sizeof(unsigned char) * 4 * newargs->height * newargs->width);
    cudaMemcpy(d_newargs, newargs, sizeof(struct Gamargs), cudaMemcpyHostToDevice);
    cudaMemcpy(d_pixmap, newargs->pixmap, sizeof(unsigned char) * 4 * newargs->height * newargs->width, cudaMemcpyHostToDevice);
    cudaMemcpy(d_pixmapmod, newargs->pixmapmod, sizeof(unsigned char) * 4 * newargs->height * newargs->width, cudaMemcpyHostToDevice);
    printf("%d\n", numpixels);
    printf("spawning %d blocks\n", (numpixels + 255) / 256);
    gamma_subset<<<(numpixels + 255) / 256, 256>>>((void *)d_newargs, (void *)d_pixmap, (void *)d_pixmapmod);

    // Pass the data back
    cudaMemcpy(newargs, d_newargs, sizeof(struct Gamargs), cudaMemcpyDeviceToHost);
    cudaMemcpy(newargs->pixmap, d_pixmap, sizeof(unsigned char) * 4 * newargs->height * newargs->width, cudaMemcpyDeviceToHost);
    cudaMemcpy(newargs->pixmapmod, d_pixmapmod, sizeof(unsigned char) * 4 * newargs->height * newargs->width, cudaMemcpyDeviceToHost);

    return NULL;
  }

