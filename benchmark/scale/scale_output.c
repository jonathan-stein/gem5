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

int main(int argc, char* argv[]) {
  if (argc != 6) {
    fprintf(stderr, "Usage: ./scale_output input output.png scaleWidth scaleHeight channel\n");
    return -1;
  }

  // Allocate the memory for scaled image.
  const int scaleWidth = atoi(argv[3]);
  const int scaleHeight = atoi(argv[4]);
  const int channel = atoi(argv[5]);
  uint8_t *scaleImage = malloc(scaleWidth * scaleHeight * channel * sizeof(uint8_t));

  FILE* fp = fopen(argv[1], "r");
  int counter = 0;
  unsigned val[1];
  int ret = fscanf(fp, "%u\n", val);
  scaleImage[counter++] = val[0];
  while (ret != EOF) {
    ret = fscanf(fp, "%u\n", val);
    scaleImage[counter++] = val[0];
  }
  fclose(fp);

  // Write the output file.
  stbi_write_png(argv[2], scaleWidth, scaleHeight, channel, scaleImage, scaleWidth * channel);

  // Free resources.
  free(scaleImage);
  return 0;
}
