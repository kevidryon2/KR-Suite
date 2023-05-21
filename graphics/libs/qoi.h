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
} QOIChunkType;

typedef struct {
  unsigned type : 2;
  unsigned index : 6;
} QOIChunkIndex;

typedef struct {
  unsigned type : 2;
  signed dr : 2;
  signed dg : 2;
  signed db : 2;
} QOIChunkDiff;

typedef struct {
  unsigned type : 2;
  signed dg : 6;
  signed dr : 4;
  signed db : 4;
} QOIChunkLuma;

typedef struct {
  unsigned type : 2;
  signed run : 6;
} QOIChunkRun;

typedef struct {
  uint8_t type, r, g, b;
} QOIChunkRGB;

typedef struct {
  uint8_t type, r, g, b, a;
} QOIChunkRGBA;

size_t qoiDecode(char *input, char *output);
size_t qoiEncode(char *input, int width, int height, char *output);

