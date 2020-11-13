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
void scale(uint8_t *image, const int width, const int height, const int channel,
    uint8_t *scaleImage, const float scaleRatio) {
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
  if (argc != 7) {
    fprintf(stderr, "Usage: ./scale input output width height channel scaleRatio\n");
    return -1;
  }

  FILE* input = fopen(argv[1], "r");

  // Determine the scale and load the image.
  int width, height, channel;
  width = atoi(argv[3]);
  height = atoi(argv[4]);
  channel = atoi(argv[5]);
  uint8_t *image = malloc(width * height * channel * sizeof(uint8_t));
  int counter = 0;
  unsigned val[1];
  int ret = fscanf(input, "%u\n", val);
  image[counter++] = val[0];
  while (ret != EOF) {
    ret = fscanf(input, "%u\n", val);
    image[counter++] = val[0];
  }
  fclose(input);


  const float scaleRatio = strtof(argv[6], NULL);

  // Allocate the memory for scaled image.
  const int scaleWidth = width * scaleRatio;
  const int scaleHeight = height * scaleRatio;
  uint8_t *scaleImage = malloc(scaleWidth * scaleHeight * channel * sizeof(uint8_t));

  scale(image, width, height, channel, scaleImage, scaleRatio);

  // Write output file
  FILE* output = fopen(argv[2], "w+");
  for (int i = 0; i < scaleWidth * scaleHeight * channel; i++) {
    fprintf(output, "%u\n", scaleImage[i]);
  }

  fclose(output);

  // Free resources.
  free(scaleImage);
  free(image);
  return 0;
}
