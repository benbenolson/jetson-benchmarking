#include <stdlib.h>
#include <stdio.h>
#include "bmp/bmp.h"
#include "displayimage/displayimage.h"
#include "transform/transform_threadpool.h"

void event_loop(unsigned char *pixmap, unsigned char *pixmapmod, struct XWin **xwin)
{
  float gamma = 1;
  int w_key, s_key;
  int pressed_key;

  // Construct the arguments to be sent to apply_gamma
  struct Gamargs *wargs = malloc(sizeof(struct Gamargs));
  wargs->pixmap = pixmap;
  wargs->pixmapmod = pixmapmod;
  wargs->width = (*xwin)->width;
  wargs->height = (*xwin)->height;
  wargs->depth = (*xwin)->depth;
  wargs->gam = gamma;

  // Bind apply_gamma to the W key
  w_key = create_key("W", &apply_gamma, (void *)wargs, xwin);

  struct Gamargs *sargs = malloc(sizeof(struct Gamargs));
  sargs->pixmap = pixmap;
  sargs->pixmapmod = pixmapmod;
  sargs->width = (*xwin)->width;
  sargs->height = (*xwin)->height;
  sargs->depth = (*xwin)->depth;
  sargs->gam = gamma;

  s_key = create_key("S", &apply_gamma, (void *)sargs, xwin);

  while(!((*xwin)->should_close)) {
    if(input_ready(xwin)) {
      pressed_key = get_key(xwin);
      if(pressed_key == w_key) {
        if((gamma < 5)) {
          gamma += 0.1;
        }
        wargs->gam = gamma;
        change_args(w_key, wargs, xwin);
        handle_key(w_key, xwin);
      } else if(pressed_key == s_key) {
        if((gamma > -5)) {
          gamma -= 0.1;
        }
        sargs->gam = gamma;
        change_args(s_key, sargs, xwin);
        handle_key(s_key, xwin);
      }
    } else {
      update_screen(pixmapmod, xwin);
    }
    flush_input(xwin);
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

  if(argc != 2) {
    fprintf(stderr, "Usage: ./image [filename]\n");
    exit(1);
  }

  // Open the file
  FILE *file = fopen(argv[1], "r");

  // Read in the header information
  read_header(file, &type, &width, &height, &depth, &compress, &size);

  // Now read in the pixel array
  pixmap = calloc(((width) * (height) * 4), 1);
  pixmapmod = calloc(((width) * (height) * 4), 1);
  read_pixels(file, pixmap, pixmapmod, width, height, depth);

  // Finally, display the image.
  struct XWin **xwin = calloc(sizeof(struct XWin *), 1);
  xwindow_init(pixmap, pixmapmod, width, height, depth, xwin);
  event_loop(pixmap, pixmapmod, xwin);

  // Clean up
  xwindow_del(xwin);
  free(pixmap);
  free(pixmapmod);
  free(xwin);

  fclose(file);
  return 0;
}
