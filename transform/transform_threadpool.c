#include "transform_threadpool.h"

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

void *gamma_subset(void *args)
{
  struct Gamargs *pic = (struct Gamargs *)args;
  unsigned char *beg = pic->pixmap;
  unsigned char tmp;
  if(pic->depth == 24) {
    for(int i = 0; i < (pic->height); ++i) {
      for(int n = 0; n < (pic->width); ++n) {
        for(int x = 0; x < 3; ++x) {
          tmp = pow((float)(*(pic->pixmap)) / 255, (float)(1 / pic->gam)) * 255;
          if(tmp < 5) {
            tmp = 0;
          } else if (tmp > 250) {
            tmp = 255;
          }
          *(pic->pixmapmod) = tmp;  
          ++(pic->pixmap);
          ++(pic->pixmapmod);
        }
        ++(pic->pixmap);
        ++(pic->pixmapmod);
      }
    }
  }
  (pic->pixmap) = beg;
  return NULL;
}

void *apply_gamma(void *args)
{
  int numthreads = 10;
  struct Gamargs *oldargs = args;
  struct Gamargs *newargs;

  // If you haven't already, create the threadpool
  if(!threadpool) {
    threadpool = malloc(sizeof(struct Threadpool));
    threadpool_create(threadpool, numthreads);
  }

  for(int i = 0; i < numthreads; ++i) {
    newargs = malloc(sizeof(struct Gamargs));
    newargs->width = oldargs->width;
    newargs->height = oldargs->height / numthreads;
    newargs->depth = oldargs->depth;
    newargs->gam = oldargs->gam;
    newargs->pixmap = oldargs->pixmap + (i * (4 * oldargs->width * 
                                        ((oldargs->height) / numthreads)));
    newargs->pixmapmod = oldargs->pixmapmod + (i * (4 * oldargs->width * 
                                              ((oldargs->height) / numthreads)));
    task_create(threadpool, &gamma_subset, newargs);
  }
  
  return NULL;
}
