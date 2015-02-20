#include <stdlib.h>
#include <stdio.h>

void print_24b_array(unsigned char *pixmap, int *width, int *height)
{
  for(int i = 0; i < (*height); ++i) {
    for(int n = 0; n < (*width); ++n) {
      for(int x = 0; x < 3; ++x) {
        printf(" %3.5x ", *pixmap);
        ++pixmap;
      }
      printf("\n");
    }
  }
}

void windows(FILE *file, int *width, int *height, int *depth, int *compress, int *size)
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

  // Skip the PPM
  fseek(file, 16, SEEK_CUR);
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

/*********************************
*          READ_HEADER           *
*********************************/
void read_header(FILE *file, int *type, int *width, int *height, int *depth, int *compress, int *size)
{
  // Read in junk
  fseek(file, 14, SEEK_CUR);
  
  // Get the type of bitmap header
  fread(type, 4, 1, file);

  // Now read in the OS-specific header
  if(*type == 40) {
    printf("Reading in a Windows NT 3.1 or later BMP image.\n");
    windows(file, width, height, depth, compress, size);
  } else if(*type == 12) {
    printf("Reading in an OS/2 BMP image.\n");
    os2(file, width, height, depth);
  } else if((*type == 52) || (*type == 56)) {
    fprintf(stderr, "This is a crazy kind of BMP file only output by Adobe Photoshop. Abort.\n");
    exit(1);
  } else {
    fprintf(stderr, "I've never seen this type of bitmap file before.\n");
    exit(1);
  }
}

/*********************************
*        READ_PIXELS             *
*********************************/
void read_pixels(FILE *file, unsigned char *pixmap, int *width, int *height, int *depth)
{
  // Calculate the number of bytes of padding per row
  int rowsize = ((((*depth) * (*width)) + 31) / 32) * 4;
  int padding = rowsize - ((*width) * 3);

  // Make sure we can get back to the beginning
  unsigned char *beg = pixmap;

  if(*depth == 24) {
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

  pixmap = beg;
}

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
