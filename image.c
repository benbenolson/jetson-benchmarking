#include <stdlib.h>
#include <stdio.h>
#include "bmp/bmp.h"
#include "displayimage/displayimage.h"
#ifdef TRANSFORM_THREAD
  #include "transform/transform_thread.h"
#endif
#ifdef TRANSFORM_THREADPOOL
  #include "transform/transform_threadpool.h"
#endif
#ifdef TRANSFORM_CUDA
  #include "transform/transform_cuda.h"
#endif
#ifdef TRANSFORM_NOTHREAD
  #include "transform/transform_nothread.h"
#endif
#include "timing/timing.h"


/******************************
*           NO X11            *
******************************/
#if defined(TRANSFORM_CUDA)
void fps_test(unsigned char *pixmap, unsigned char *pixmapmod, int width, int height, int depth, int numthreads, double split, int threadsperblock)
#elif defined(TRANSFORM_THREAD) || defined(TRANSFORM_THREADPOOL)
void fps_test(unsigned char *pixmap, unsigned char *pixmapmod, int width, int height, int depth, int numthreads)
#else
void fps_test(unsigned char *pixmap, unsigned char *pixmapmod, int width, int height, int depth)
#endif
{
  float gamma = 1;
  int up = 0;

  // Set up the arguments for transform function
  struct Gamargs *wargs = malloc(sizeof(struct Gamargs));
  wargs->pixmap = pixmap;
  wargs->pixmapmod = pixmapmod;
  wargs->width = width;
  wargs->height = height;
  wargs->depth = depth;
  wargs->prevgam = gamma;
  wargs->gam = gamma;
#if defined(TRANSFORM_CUDA)
  wargs->numthreads = numthreads;
  wargs->split = split;
  wargs->threadsperblock = threadsperblock;
#elif defined(TRANSFORM_THREAD) || defined (TRANSFORM_THREADPOOL)
  wargs->numthreads = numthreads;
#endif
  
  // Set up the arguments for transform function
  struct Gamargs *sargs = malloc(sizeof(struct Gamargs));
  sargs->pixmap = pixmap;
  sargs->pixmapmod = pixmapmod;
  sargs->width = width;
  sargs->height = height;
  sargs->depth = depth;
  sargs->prevgam = gamma;
  sargs->gam = gamma;
#if defined(TRANSFORM_CUDA)
  sargs->numthreads = numthreads;
  sargs->split = split;
  sargs->threadsperblock = threadsperblock;
#elif defined(TRANSFORM_THREAD) || defined (TRANSFORM_THREADPOOL)
  sargs->numthreads = numthreads;
#endif
  
  for(int i = 0; i < 60; ++i) {
    if(up == 0) {
      gamma += 0.1;
      wargs->prevgam = gamma - 0.1;
      wargs->gam = gamma;
      apply_gamma(wargs);
    } else if(up == 1) {
      if(gamma >= 0) {
        gamma -= 0.1;
      }
      sargs->prevgam = gamma + 0.1;
      sargs->gam = gamma;
      apply_gamma(sargs);
    }
    if(up == 0) {
      ++up;
    } else {
      --up;
    }
  }

  free(wargs);
  free(sargs);
}


/******************************
*        NONINTERACTIVE       *
******************************/
#if defined(TRANSFORM_CUDA)
void fps_test_x11(unsigned char *pixmap, unsigned char *pixmapmod, struct XWin **xwin, int numthreads, double split, int threadsperblock)
#elif defined(TRANSFORM_THREAD) || defined(TRANSFORM_THREADPOOL)
void fps_test_x11(unsigned char *pixmap, unsigned char *pixmapmod, struct XWin **xwin, int numthreads)
#else
void fps_test_x11(unsigned char *pixmap, unsigned char *pixmapmod, struct XWin **xwin)
#endif
{
  float gamma = 1;
  int w_key, s_key;
  int pressed_key;

  // Handle the W key
  struct Gamargs *wargs = malloc(sizeof(struct Gamargs));
  wargs->pixmap = pixmap;
  wargs->pixmapmod = pixmapmod;
  wargs->width = (*xwin)->width;
  wargs->height = (*xwin)->height;
  wargs->depth = (*xwin)->depth;
  wargs->prevgam = gamma;
  wargs->gam = gamma;
#if defined(TRANSFORM_CUDA)
  wargs->numthreads = numthreads;
  wargs->split = split;
  wargs->threadsperblock = threadsperblock;
#elif defined(TRANSFORM_THREAD) || defined (TRANSFORM_THREADPOOL)
  wargs->numthreads = numthreads;
#endif
  w_key = create_key("W", &apply_gamma, (void *)wargs, xwin);
  
  // Handle the S key
  struct Gamargs *sargs = malloc(sizeof(struct Gamargs));
  sargs->pixmap = pixmap;
  sargs->pixmapmod = pixmapmod;
  sargs->width = (*xwin)->width;
  sargs->height = (*xwin)->height;
  sargs->depth = (*xwin)->depth;
  sargs->prevgam = gamma;
  sargs->gam = gamma;
#if defined(TRANSFORM_CUDA)
  sargs->numthreads = numthreads;
  sargs->split = split;
  sargs->threadsperblock = threadsperblock;
#elif defined(TRANSFORM_THREAD) || defined (TRANSFORM_THREADPOOL)
  sargs->numthreads = numthreads;
#endif
  s_key = create_key("S", &apply_gamma, (void *)sargs, xwin);
  
  pressed_key = w_key;
  for(int i = 0; i < (60); ++i) {
    if(pressed_key == w_key) {
      gamma += 0.1;
      wargs->prevgam = gamma - 0.1;
      wargs->gam = gamma;
      change_args(w_key, wargs, xwin);
      handle_key(w_key, xwin);
    } else if(pressed_key == s_key) {
      if(gamma >= 0) {
        gamma -= 0.1;
      }
      sargs->prevgam = gamma + 0.1;
      sargs->gam = gamma;
      change_args(s_key, sargs, xwin);
      handle_key(s_key, xwin);
    }
    flush_input(xwin);
    update_screen(pixmapmod, xwin);
    if(pressed_key == w_key) {
      pressed_key = s_key;
    } else {
      pressed_key = w_key;
    }
  }
}

/******************************
*        INTERACTIVE          *
******************************/
void event_loop(unsigned char *pixmap, unsigned char *pixmapmod, struct XWin **xwin)
{
  float gamma = 1;
  int w_key, s_key;
  int pressed_key;

  // Handle the W key
  struct Gamargs *wargs = malloc(sizeof(struct Gamargs));
  wargs->pixmap = pixmap;
  wargs->pixmapmod = pixmapmod;
  wargs->width = (*xwin)->width;
  wargs->height = (*xwin)->height;
  wargs->depth = (*xwin)->depth;
  wargs->prevgam = gamma;
  wargs->gam = gamma;
  w_key = create_key("W", &apply_gamma, (void *)wargs, xwin);
  
  // Handle the S key
  struct Gamargs *sargs = malloc(sizeof(struct Gamargs));
  sargs->pixmap = pixmap;
  sargs->pixmapmod = pixmapmod;
  sargs->width = (*xwin)->width;
  sargs->height = (*xwin)->height;
  sargs->depth = (*xwin)->depth;
  sargs->prevgam = gamma;
  sargs->gam = gamma;
  s_key = create_key("S", &apply_gamma, (void *)sargs, xwin);

  while(!((*xwin)->should_close)) {
    if(input_ready(xwin)) {
      pressed_key = get_key(xwin);
      if(pressed_key == w_key) {
        gamma += 0.1;
        wargs->prevgam = gamma - 0.1;
        wargs->gam = gamma;
        change_args(w_key, wargs, xwin);
        timing(handle_key(w_key, xwin));
      } else if(pressed_key == s_key) {
        if(gamma >= 0) {
          gamma -= 0.1;
        }
        sargs->prevgam = gamma + 0.1;
        sargs->gam = gamma;
        change_args(s_key, sargs, xwin);
        timing(handle_key(s_key, xwin));
      }
      flush_input(xwin);
      update_screen(pixmapmod, xwin);
    }
  }
}

/*********************************
*             MAIN               *
*********************************/
int main(int argc, char **argv)
{
  int size = 0;
  int width = 0;
  int height = 0;
  int depth = 0;
  int compress = 0;
  int type = 0;
  unsigned char *pixmap, *pixmapmod;

#if defined(TRANSFORM_CUDA)
  if(argc != 5) {
    fprintf(stderr, "Usage: ./image [filename] [numthreads] [split] [threadsperblock]\n");
    exit(1);
  }
  int numthreads, threadsperblock;
  double split;
  numthreads = atoi(argv[2]);
  split = atof(argv[3]);
  threadsperblock = atoi(argv[4]);
#elif defined(TRANSFORM_THREAD) || defined (TRANSFORM_THREADPOOL)
  if(argc != 3) {
    fprintf(stderr, "Usage: ./image [filename] [numthreads]\n");
    exit(1);
  }
  int numthreads;
  numthreads = atoi(argv[2]);
#else
  if(argc != 2) {
    fprintf(stderr, "Usage: ./image [filename]\n");
    exit(1);
  }
#endif

  // Open the file
  FILE *file = fopen(argv[1], "r");

  // Read in the header information
  read_header(file, &type, &width, &height, &depth, &compress, &size);

  // Now read in the pixel array
  pixmap = calloc(((width) * (height) * 4), 1);
  pixmapmod = calloc(((width) * (height) * 4), 1);
  read_pixels(file, pixmap, pixmapmod, width, height, depth);

  // Finally, display the image.
  // struct XWin **xwin = calloc(sizeof(struct XWin *), 1);
  // xwindow_init(pixmap, pixmapmod, width, height, depth, xwin);
#if defined(TRANSFORM_CUDA)
  timing(fps_test(pixmap, pixmapmod, width, height, depth, numthreads, split, threadsperblock));
#elif defined(TRANSFORM_THREAD) || defined(TRANSFORM_THREADPOOL) || defined(TRANSFORM_CUDA)
  timing(fps_test(pixmap, pixmapmod, width, height, depth, numthreads));
#else
  timing(fps_test(pixmap, pixmapmod, width, height, depth));
#endif

  // Clean up
  //xwindow_del(xwin);
  free(pixmap);
  free(pixmapmod);
  //free(xwin);

  fclose(file);
  return 0;
}
