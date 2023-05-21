#include <stdio.h>
#include <stdlib.h>
#include "qoi.h"
#include "spng.h"

int main(int argc, char **argv) {
	printf("QOI Decoder v0.01\n");
	if (argc<4) {
		printf("Usage: %s <file> p/r <output>\np: PNG\nr: Raw\n", argv[0]);
		return 1;
	}
	FILE *fpi = fopen(argv[1], "r");
	FILE *fpo = fopen(argv[3], "w");
	
	if (!fpi) {
		printf("Can't open file %s\n", argv[1]);
		return 1;
	} else if (!fpo) {
		printf("Can't open file %s\n", argv[3]);
		return 1;
	}
	
	int qlen;								
	
	fseek(fpi, 0, SEEK_END);
	char *qoidata = malloc(qlen = ftell(fpi));
	fseek(fpi, 0, SEEK_SET);
	fread(qoidata, 1, qlen, fpi);
	
	char *decoded = NULL;
	int blen = qoiDecode(qoidata, decoded);
	
	if (!strcmp(argv[2], "p")) {
		printf("Encoding with PNG\n");
		int pblen;
		int rt;
		spng_ctx *ctx = spng_ctx_new(0);
		struct spng_ihdr ihdr;
		spng_set_option(ctx, SPNG_ENCODE_TO_BUFFER, 1);
		ihdr.width = ((QOIHeader*)qoidata)->width;
		ihdr.height = ((QOIHeader*)qoidata)->height;
		ihdr.bit_depth = 8;
		ihdr.color_type = 6;
		spng_set_ihdr(ctx, &ihdr);
		rt = spng_encode_image(ctx, decoded, blen, SPNG_FMT_RAW, SPNG_ENCODE_FINALIZE);
		
	}
}
