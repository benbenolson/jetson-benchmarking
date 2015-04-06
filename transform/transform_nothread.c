#include <math.h>
#include "transform_nothread.h"

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
void *apply_gamma(void *args)
{
  struct Gamargs *pic = args;
  unsigned char *pixmapbeg = pic->pixmap;
  unsigned char *pixmapmodbeg = pic->pixmapmod;
  unsigned char tmp;

  if(pic->depth == 24) {
    for(int i = 0; i < (pic->height); ++i) {
      for(int n = 0; n < (pic->width); ++n) {
        for(int x = 0; x < 3; ++x) {
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
        ++(pic->pixmap);
        ++(pic->pixmapmod);
      }
    }
  }
  (pic->pixmap) = pixmapbeg;
  (pic->pixmapmod) = pixmapmodbeg;
  return NULL;
}
