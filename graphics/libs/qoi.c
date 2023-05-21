#include "qoi.h"

size_t qoiDecode(char *restrict qoidata, char *restrict buffer) {
	if (buffer) free(buffer);
	QOIHeader hd = *(QOIHeader*)qoidata;
	hd.width = be32toh(hd.width);
	hd.height = be32toh(hd.height);
	printf("\
QOI Header:\n\
	Size: %dx%d (%d bytes)\n\
	Channels: %s\n\
	Colorspace: %s\n", hd.width, hd.height, hd.width*hd.height*4,
		   				(hd.channels-3)?((hd.channels>4)?"invalid":"rgba"):"rgb",hd.colorspace?"linear":"srgb");
	
	buffer = malloc((hd.width*hd.height)*4);
	
	int prevpixel = -1;
	
	for (int i=0; i<hd.width*hd.height; i++) {
		
	}
	
	return hd.width*hd.height;
}

size_t qoiEncode(char *rawdata, int width, int height, char *buffer) {
	if (buffer) free(buffer);
	printf("");
}
