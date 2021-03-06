/* Benchmark Application in C.
 * The error injection function is scale.
 * 
 * Compile the code with: clang scale.c -lm
 *  -lm: link the C math library.
 *
 * Scale an image.
 * Image IO: the stb_image library from https://github.com/nothings/stb.
 * The entire library is stb_image.h and stb_image_write.h.
 *
 * Author: Ziang Wan, Jonathan Stein
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


// The main function: scale a image.
void scale(uint8_t *image, int width, int height, int channel,
    uint8_t *scaleImage, float scaleRatio) {
  const int scaleWidth = width * scaleRatio;
  const int scaleHeight = height * scaleRatio;

  for (int srow = 0; srow < scaleHeight; srow += 1) {
    for (int scol = 0; scol < scaleWidth; scol += 1) {
      const int row = srow / scaleRatio;
      const int col = scol / scaleRatio;

      const int sIdx = (srow * scaleWidth + scol) * channel;
      const int idx = (row * width + col) * channel;
      scaleImage[sIdx] = image[idx];
      scaleImage[sIdx + 1] = image[idx + 1];
      scaleImage[sIdx + 2] = image[idx + 2];
    }
  }
}


int main(int argc, char* argv[]) {
  const char* inputPath = "/home/user/Desktop/CS523/gem5/benchmark/scale/img.png";
  const char* outputPath = "/home/user/Desktop/CS523/gem5/benchmark/scale/out.png";
  const float scaleRatio = 0.5;

  // Determine the scale and load the image.
  int width, height, channel;
  uint8_t *image = stbi_load(inputPath, &width, &height, &channel, 0);

  // Allocate the memory for scaled image.
  const int scaleWidth = width * scaleRatio;
  const int scaleHeight = height * scaleRatio;
  uint8_t *scaleImage = malloc(scaleWidth * scaleHeight * channel * sizeof(uint8_t));

  scale(image, width, height, channel, scaleImage, scaleRatio);

  // Write the output file.
  stbi_write_png(outputPath, scaleWidth, scaleHeight, channel, scaleImage, scaleWidth * channel);

  // Free resources.
  stbi_image_free(image);
  free(scaleImage);
  return 0;
}