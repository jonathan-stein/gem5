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
#include <time.h>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


static const char* inputPath = "/home/user/Desktop/CS523/gem5/benchmark/scale/img.png";
static const char* outputPath = "/home/user/Desktop/CS523/gem5/benchmark/scale/out.png";
static const float scaleRatio = 1.0;


// NOTE: both integer <-> floating point conversions are executed using FPU.
// All cvtf_i2d cvtf_i2d_hi cvtf_d2i are in arch/x86/isa/microops/fpop.isa.
// We inject errors using FPU rate.
static const float aluReliability = 0.98;
static const float fpuReliability = 1.0;

float fpuErrorInject(const float f) {
  float r = ((float) rand()) / RAND_MAX;
  if (r <= fpuReliability) {
    return f;
  } else {
    return f * rand();
  }
}

int aluErrorInject(const int i) {
  float r = ((float) rand()) / RAND_MAX;
  if (r <= aluReliability) {
    return i;
  } else {
    return i & 0x00800000;
  }
}

int fpuFloatToInt(const float f) {
  return (int) fpuErrorInject(f);
}

float fpuIntToFloat(const int i) {
  return fpuErrorInject((float) i);
}


// The main function: scale a image.
// Source code level error injection.
void scale(uint8_t *image, uint8_t *scaleImage,
    const int width, const int height, const int channel,
    const float scaleRatio, const int scaleWidth, const int scaleHeight) {

  for (int srow = 0; srow < scaleHeight; srow += 1) {
    for (int scol = 0; scol < scaleWidth; scol += 1) {
      // const int row = srow / scaleRatio;
      const int row = fpuFloatToInt(fpuErrorInject(fpuIntToFloat(srow) / scaleRatio));
      // const int col = scol / scaleRatio;
      const int col = fpuFloatToInt(fpuErrorInject(fpuIntToFloat(scol) / scaleRatio));

      // const int sIdx = (srow * scaleWidth + scol) * channel;
      const int sIdx = aluErrorInject(aluErrorInject(aluErrorInject(srow * scaleWidth) + scol) * channel);
      // const int idx = (row * width + col) * channel;
      const int idx = aluErrorInject(aluErrorInject(aluErrorInject(row * width) + col) * channel);
      // scaleImage[sIdx] = image[idx];
      scaleImage[sIdx] = image[idx];
      // scaleImage[sIdx + 1] = image[idx + 1];
      scaleImage[aluErrorInject(sIdx + 1)] = image[aluErrorInject(idx + 1)];
      // scaleImage[sIdx + 2] = image[idx + 2];
      scaleImage[aluErrorInject(sIdx + 2)] = image[aluErrorInject(idx + 2)];
    }
  }

}


int main(int argc, char* argv[]) {
  // Determine the scale and load the image.
  int width, height, channel;
  uint8_t *image = stbi_load(inputPath, &width, &height, &channel, 0);

  // Allocate the memory for scaled image.
  const int scaleWidth = width * scaleRatio;
  const int scaleHeight = height * scaleRatio;
  uint8_t *scaleImage = malloc(scaleWidth * scaleHeight * channel * sizeof(uint8_t));

  time_t t;
  srand((unsigned) time(&t));

  scale(image, scaleImage,
      width, height, channel,
      scaleRatio, scaleWidth, scaleHeight);

  // Write the output file.
  stbi_write_png(outputPath, scaleWidth, scaleHeight, channel, scaleImage, scaleWidth * channel);

  // Free resources.
  stbi_image_free(image);
  free(scaleImage);
  return 0;
}