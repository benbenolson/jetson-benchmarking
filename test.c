#include <math.h>
#include <stdio.h>

int main()
{
  unsigned char result;
  float gamma;
  for(gamma = 1.0; gamma > -5; gamma -= 0.1) {
    for(unsigned char i = 0; i < 255; ++i) {
      result = pow((float)i / 255, (float)(1 / gamma)) * 255;
      if((result == 0) && (i != 0)) {
        printf("ERROR when gamma is %f: %u to %u\n", gamma, i, result);
      }
    }
  }
}
