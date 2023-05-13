#include <stdio.h>
#include <stdlib.h>

int flen(FILE *fp) {
	int op = ftell(fp);
	fseek(fp, 0, SEEK_END);
	int len = ftell(fp);
	fseek(fp, op, SEEK_SET);
	return len;
}

int main(int argc, char **argv) {
	printf("KR Encrypt v0.01\n");
	if (argc<5) {
		printf("Usage: %s e/d <file> <key> <method>\nMethod can be any string of 's', 'r', 'a'\n\t\n", argv[0]);
		return 1;
	}
	
	FILE *fp = fopen(argv[2], "r");
	if (!fp) {
		printf("Can't open file %s\n", argv[1]);
		return 2;
	}
									
	int l;
	char *filedata = malloc(l = flen(fp));
	printf("Read %lu bytes\n", fread(filedata, 1, l, fp));
	
	fclose(fp);
	
	switch (argv[0][0]) {
		case 'c':
			encrypt(filedata, strtol(argv[2], 0, 16), argv[3]);
			break;
		case 'd':
			decrypt(filedata, strtol(argv[2], 0, 16), argv[3]);
			break;
	}
	
	
}
