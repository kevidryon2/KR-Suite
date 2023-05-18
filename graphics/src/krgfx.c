#include "spng.h"
#include "qoi.h"

const char qoi_magic[4] = "qoif";
const char png_magic[4] = "\x89PNG";

int main(int argc, char **argv) {
	if (argc<3) {
		printf("Usage: %s <file>\nThe script gets read from STDIN. Use a tool such as 'cat' to use a file as a script.\n", argv[0]);
		return 1;
	}
	
	printf("KR Graphics v0.02\n");
									
	char scbuff[BUFSIZ];
	fread(scbuff, 1, BUFSIZ, stdin);
	
	
	return 0;
}
