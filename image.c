#include <stdlib.h>
#include <stdio.h>
#include "bmp.h"
#include "x.h"

/*********************************
*             MAIN               *
*********************************/
int main(int argc, char **argv)
{
  char *head;
  int *size, *start, *width, *height,
      *depth, *compress, *horiz, 
      *vert, *colors, *type;
  unsigned char *pixmap;

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
  pixmap = malloc((*size));
  read_pixels(file, pixmap, width, height, depth);

  // Now do the transformation
  //invert_colors(pixmap, width, height, depth);

  // Now finally display the image
  display_image(pixmap, width, height);

  fclose(file);
  return 0;
}
