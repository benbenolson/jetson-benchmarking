#ifndef BMP_H
#define BMP_H

#include <stdlib.h>
#include <stdio.h>

void print_24b_array(unsigned char *pixmap, int *width, int *height)
{
  for(int i = 0; i < (*height); ++i) {
    for(int n = 0; n < (*width); ++n) {
      for(int x = 0; x < 4; ++x) {
        printf(" %3.2x ", *pixmap);
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

  fseek(file, 2, SEEK_CUR);

  // Get the bit depth of the image
  fread(depth, 2, 1, file);

  // Compression type
  fread(compress, 4, 1, file);

  // Image size
  fread(size, 4, 1, file);

  // Skip the PPM
  fseek(file, 16, SEEK_CUR);
}

void os2(FILE *file, int *width, int *height, int *depth)
{
  // Get the width and height of the image
  fread(width, 2, 1, file);
  fread(height, 2, 1, file);

  fseek(file, 2, SEEK_CUR);

  // Get the bit depth of the image
  fread(depth, 2, 1, file);
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
    windows(file, width, height, depth, compress, size);
  } else if(*type == 12) {
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
void read_pixels(FILE *file, unsigned char *pixmap, unsigned char *pixmapmod, int width, int height, int depth)
{
  // Calculate the number of bytes of padding per row
  int rowsize = ((((depth) * (width)) + 31) / 32) * 4;
  int padding = rowsize - ((width) * 3);

  // Make sure we can get back to the beginning
  unsigned char *beg = pixmap;

  // Put the pointer at the end of the array (since BMPs are stored backwards)
  for(int i = 0; i < ((height) * (width) * 4); ++i) {
    ++pixmap;
    ++pixmapmod;
  }
  
  // Read in the image data (flipping horizontally)
  if(depth == 24) {
    for(int i = 0; i < (height); ++i) {
      pixmap -= ((width) * 4);
      pixmapmod -= ((width) * 4);
      for(int n = 0; n < (width); ++n) {
        for(int x = 0; x < 3; ++x) {
          fread(pixmap, 1, 1, file);
          *pixmapmod = *pixmap;
          ++pixmap;
          ++pixmapmod;
        }
        *pixmap = 0;
        *pixmapmod = 0;
        ++pixmap;
        ++pixmapmod;
      }
      fseek(file, padding, SEEK_CUR);
      pixmap -= ((width) * 4);
      pixmapmod -= ((width) * 4);
    }
  }
  pixmap = beg;
}

#endif
