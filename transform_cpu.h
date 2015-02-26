#include <math.h>

/*********************************
*         INVERT_COLORS          *
*********************************/
void invert_colors(unsigned char *pixmap, int *width, int *height, int *depth)
{
  unsigned char *beg = pixmap;
  if(*depth == 24) {
    for(int i = 0; i < (*height); ++i) {
      for(int n = 0; n < (*width); ++n) {
        for(int x = 0; x < 3; ++x) {
          *pixmap = 255 - (*pixmap);
          ++pixmap;
        }
      }
    }
  }
  pixmap = beg;
}

/*********************************
*         GAMMA                  *
*********************************/
void apply_gamma(unsigned char *pixmap, int *width, int *height, int *depth, float gam)
{
  unsigned char *beg = pixmap;
  if(*depth == 24) {
    for(int i = 0; i < (*height); ++i) {
      for(int n = 0; n < (*width); ++n) {
        for(int x = 0; x < 3; ++x) {
          *pixmap = pow((*pixmap), gam);
          ++pixmap;
        }
      }
    }
  }
  pixmap = beg;
}
