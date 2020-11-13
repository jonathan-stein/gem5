// Check the percentage of pixel difference between two images/
// Author: Ziang Wan, Jonathan Stein.
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


int main(int argc, char* argv[]) {
  const char* image1Path = "/home/user/Desktop/CS523/gem5/benchmark/scale/images/kite.png";
  const char* image2Path = "/home/user/Desktop/CS523/gem5/benchmark/scale/images/0.98_out.png";

  // Determine the scale and load the image.
  int image1Width;
  int image1Height;
  int image1Channel;
  uint8_t *image1 = stbi_load(image1Path, &image1Width, &image1Height, &image1Channel, 0);

  int image2Width;
  int image2Height;
  int image2Channel;
  uint8_t *image2 = stbi_load(image2Path, &image2Width, &image2Height, &image2Channel, 0);

  assert(image1Width == image2Width);
  assert(image1Height == image2Height);
  assert(image1Channel == image2Channel);

  const int numPixel = image1Width * image1Height;
  int matchedPixel = 0;
  for (int row = 0; row < image1Height; row += 1) {
    for (int col = 0; col < image1Width; col += 1) {
      const int idx = (row * image1Width + col) * image1Channel;
      if (image1[idx] == image2[idx] && image1[idx + 1] == image2[idx + 1] && image1[idx + 2] == image2[idx + 2]) {
        matchedPixel += 1;
      }
    }
  }

  printf("Pixel match: %f\n", ((double) matchedPixel) / numPixel);
  return 0;
}