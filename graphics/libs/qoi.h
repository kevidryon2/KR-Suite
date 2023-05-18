#include <stdint.h>
#include <stdlib.h>

typedef struct {
  char magic[4]; //Always "qoif"
  uint32_t width, height;
  uint8_t channels, colorspace;
} QOIHeader;

typedef enum {
  QOI_INDEX = 0,
  QOI_DIFF = 1,
  QOI_LUMA = 2,
  QOI_RUN_RGB = 3,
} QOIChunk;

size_t qoiDecode(char *input, char *output);
size_t qoiEncode(char *input, int width, int height, char *output);

