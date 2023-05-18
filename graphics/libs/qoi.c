#include "qoi.h"

size_t qoiDecode(char *qoidata, char *buffer) {
	if (buffer) free(buffer);
	QOIHeader hd = *(QOIHeader*)qoidata;
	printf("");
}

size_t qoiEncode(char *rawdata, int width, int height, char *buffer) {
	if (buffer) free(buffer);
	printf("");
}
