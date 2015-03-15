#include "transform_thread.h"

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
  struct thread_arguments *pic = (struct thread_arguments *)args;
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

void apply_gamma(unsigned char *pixmap, unsigned char *pixmapmod, int width, int height, int depth, float gam)
{
  pthread_t tid;
  struct thread_arguments args;
  args.width = width;
  args.height = height / 3;
  args.depth = depth;
  args.gam = gam;
  args.pixmap = pixmap;
  args.pixmapmod = pixmapmod;
  pthread_create(&tid, NULL, gamma_subset, &args);

  pthread_t tid2;
  struct thread_arguments args2;
  args2.width = width;
  args2.height = height / 3;
  args2.depth = depth;
  args2.gam = gam;
  args2.pixmap = pixmap + (4 * width * (height / 3));
  args2.pixmapmod = pixmapmod + (4 * width * (height / 3));
  pthread_create(&tid2, NULL, gamma_subset, &args2);

  pthread_t tid3;
  struct thread_arguments args3;
  args3.width = width;
  args3.height = height / 3;
  args3.depth = depth;
  args3.gam = gam;
  args3.pixmap = pixmap + 2 * (4 * width * (height / 3));
  args3.pixmapmod = pixmapmod + 2 * (4 * width * (height / 3));
  pthread_create(&tid3, NULL, gamma_subset, &args3);

  pthread_join(tid, NULL);
  pthread_join(tid2, NULL);
  pthread_join(tid3, NULL);
}
