#include <stdlib.h>
#include <stdio.h>
#include "bmp/bmp.h"
#include "displayimage/displayimage.h"

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
  event_loop(pixmap, pixmapmod, width, height, depth, xwin);

  fclose(file);
  return 0;
}
