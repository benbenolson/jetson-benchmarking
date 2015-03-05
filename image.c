#include <stdlib.h>
#include <stdio.h>
#include "timing.h"
#include "transform_thread.h"
#include "bmp.h"
#include "displayimage.h"

/*********************************
*             MAIN               *
*********************************/
int main(int argc, char **argv)
{
  char *head;
  int *size, *width, *height,
      *depth, *compress, *horiz, 
      *vert, *colors, *type;
  unsigned char *pixmap, *pixmapmod;

  if(argc != 2) {
    fprintf(stderr, "Usage: ./image [filename]\n");
    exit(1);
  }

  // Open the file
  FILE *file = fopen(argv[1], "r");

  // Allocate the necessary variables
  width = malloc(4); height = malloc(4);
  depth = malloc(2); compress = malloc(4);
  horiz = malloc(4); vert = malloc(4);
  size = malloc(4); type = malloc(4);

  // Read in the header information
  read_header(file, type, width, height, depth, compress, size);

  // Now read in the pixel array
  pixmap = malloc(((*width) * (*height) * 4));
  pixmapmod = malloc(((*width) * (*height) * 4));
  read_pixels(file, pixmap, pixmapmod, *width, *height, *depth);
  display_image(pixmap, pixmapmod, *width, *height, *depth);

  fclose(file);
  return 0;
}
