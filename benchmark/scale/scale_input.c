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
  if (argc != 3) {
    fprintf(stderr, "Usage: ./scale_input input.png output\n");
    return -1;
  }

  // Determine the scale and load the image.
  int width, height, channel;
  uint8_t *image = stbi_load(argv[1], &width, &height, &channel, 0);

  FILE* fp = fopen(argv[2], "w+");
  
  for (int i = 0; i < width * height * channel; i++) {
    fprintf(fp, "%u\n", image[i]);
  }

  // Free resources.
  stbi_image_free(image);
  fclose(fp);
  printf("width: %d height: %d channels: %d\n", width, height, channel);
  return 0;
}
