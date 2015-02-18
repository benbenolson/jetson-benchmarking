#include <stdlib.h>
#include <stdio.h>

void print_array(unsigned char *pixmap, int *width, int *height)
{
  for(int i = 0; i < (*height); ++i) {
    for(int n = 0; n < (*width); ++n) {
      for(int x = 0; x < 3; ++x) {
        printf(" %2x ", *pixmap);
        ++pixmap;
      }
      printf("\n");
    }
  }
}

void windows(FILE *file, int *width, int *height, int *depth, int *compress, int *size, int *horiz, int *vert)
{
  // Get the width and height of the image
  fread(width, 4, 1, file);
  fread(height, 4, 1, file);
  printf("Width: %d\n", *width);
  printf("Height: %d\n", *height);

  fseek(file, 2, SEEK_CUR);

  // Get the bit depth of the image
  fread(depth, 2, 1, file);
  printf("Bit depth: %d\n", *depth);

  // Compression type
  fread(compress, 4, 1, file);
  printf("Compression type: %d\n", *compress);

  // Image size
  fread(size, 4, 1, file);
  printf("Image size: %d\n", *size);

  // Get the ppm
  fread(horiz, 4, 1, file);
  fread(vert, 4, 1, file);
  printf("Horizontal DPI: %d\n", (int)(*horiz / 39.3701));
  printf("Vertical DPI: %d\n", (int)(*vert / 39.3701));

  fseek(file, 4, SEEK_CUR);
  fseek(file, 4, SEEK_CUR);
}

void os2(FILE *file, int *width, int *height, int *depth)
{
  // Get the width and height of the image
  fread(width, 2, 1, file);
  fread(height, 2, 1, file);
  printf("Width: %d\n", *width);
  printf("Height: %d\n", *height);

  fseek(file, 2, SEEK_CUR);

  // Get the bit depth of the image
  fread(depth, 2, 1, file);
  printf("Bit depth: %d\n", *depth);
}

int main(int argc, char **argv)
{
  char *head;
  int *size, *start, *width, *height,
      *depth, *compress, *horiz, 
      *vert, *colors, *type;
  unsigned char *pixmap, *val;
  // Check the arguments
  if(argc != 2) {
    fprintf(stderr, "Usage: ./image [filename]\n");
    exit(1);
  }

  // Open the file
  FILE *file = fopen(argv[1], "r");
  
  // Read in the header
  head = malloc(2);
  fread(head, 1, 2, file);

  // Read in junk
  fseek(file, 4, SEEK_CUR);
  fseek(file, 4, SEEK_CUR);

  // Get the start of the image data
  start = malloc(4);
  fread(start, 4, 1, file);
  
  // Get the type of bitmap header
  type = malloc(4);
  fread(type, 4, 1, file);

  // Read in the header
  if(*type == 40) {
    printf("Reading in a Windows NT 3.1 or later BMP image.\n");
    width = malloc(4); height = malloc(4);
    depth = malloc(2); compress = malloc(4);
    horiz = malloc(4); vert = malloc(4);
    size = malloc(4);
    windows(file, width, height, depth, compress, size, horiz, vert);
  } else if(*type == 12) {
    printf("Reading in an OS/2 BMP image.\n");
    width = malloc(2); height = malloc(2);
    depth = malloc(2);
    os2(file, width, height, depth);
  } else if((*type == 52) || (*type == 56)) {
    printf("This is a crazy kind of BMP file only output by Adobe Photoshop. Abort.\n");
  } 
  
  // Calculate the number of bytes of padding per row
  int rowsize = ((((*depth) * (*width)) + 31) / 32) * 4;
  int padding = rowsize - ((*width) * 3);
  printf("Padding with %d bytes\n", padding);

  // Now read in the pixel array
  if(*depth == 24) {
    pixmap = malloc((*size));
    unsigned char *beg = pixmap;
    for(int i = 0; i < (*height); ++i) {
      for(int n = 0; n < (*width); ++n) {
        for(int x = 0; x < 3; ++x) {
          fread(pixmap, 1, 1, file);
          ++pixmap;
        }
      }
      fseek(file, padding, SEEK_CUR);
    }
  }

  fclose(file);
  return 0;
}
