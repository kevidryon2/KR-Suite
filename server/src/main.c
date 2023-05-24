/*
KR Server, a program for creating servers and customizing them with Network Scripts.
Copyright (C) 2023 kevidryon2

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include "bns.h"
#include "server.h"

#define max(a,b) ((a)>(b)?(a):(b))

LoadedScript *loadScript(char *data, int len);

char *compile(char *script);
void exec(char *binscript);
int initserver();

int search_begin(char **restrict array, int num_elements, char *restrict string) {
	for (int i=0; i<num_elements; i++) {
		if (!strncmp(array[i], string, strlen(string))) {
			return i;
		}
	}
	return -1;
}

int endswith(char *restrict s, char *restrict end) {
	char *so = s+(strlen(s)-strlen(end));
	return !strcmp(so, end);
}

char *combine(char *restrict a, char *restrict b) {
	char *buffer = malloc( strlen(a)+strlen(b)+1 );
	strcpy(buffer, a);
	strcat(buffer, b);
	return buffer;
}

char *getstr(FILE *fp) {
	char buffer[BUFSIZ];
	fscanf(fp, "%s", buffer);
	return buffer;
}

void testmalloc() {
	char *p = malloc(4096);
	memset(p, 0xfe, 4096);
	free(p);
	printf("(tested malloc)\n");
}

void set_env_variable() {
	printf("You should set the KRSERVER_PATH environment variable to the path you want to use as the server main directory.\n");
	exit(1);
}

int main(int argc, char **argv, char **envp) {
	printf("KR Server v0.03\n");
	
	srand(time(NULL));
	
	unsigned short port;
	
	if (argc<2) {
		port = 8080;
	} else {
		port = atoi(argv[1]);
	}

	printf("Using port %d\n\n", port);
	
	int serversock = initserver(port);
	char rootpath[BUFSIZ];
	char cwdbuffer[BUFSIZ/2];
	
	getcwd(cwdbuffer, BUFSIZ/2);
	strcpy(rootpath, cwdbuffer);
	
	if (!getenv("KRSERVER_PATH")) set_env_variable();
	
	if (!realpath(getenv("KRSERVER_PATH"), NULL)) {
		perror(getenv("KRSERVER_PATH"));
		return 127;
	}
	
	strcpy(rootpath, realpath(getenv("KRSERVER_PATH"), NULL));
	strcat(rootpath, "/");
	
	printf("Using directory %s\n", rootpath);
	
	printf("\nLoading scripts...\n");
	
	char scriptpath[BUFSIZ];
	
	sprintf(scriptpath, "%s/scripts/", rootpath);
	
	DIR *dp = opendir(scriptpath);
	
	char *buffer;
	
	if (!dp) {
		printf("can't open script directory :( (Errno %d)\n", errno);
		printf("You should create the 'scripts' and 'public' directories in the main server folder.\n");
	}
	
	void *tempptr;
	struct dirent *ent;
	FILE *fp;
	int len;
	int sn = 0;
	
	while ((ent = readdir(dp))) {
		if (ent->d_name[0] != '.') {
			if (endswith(ent->d_name, ".bns")) {
				
				//Open file
				free(buffer);
				buffer = combine(scriptpath, ent->d_name);
				printf("Loading file %s... ", ent->d_name);
				fp = fopen(buffer, "r");
				
				if (!fp) {
					printf("can't open :( (Error %d)\n", errno);
					return 1;
				}
				
				//Read file
				free(buffer);
				fseek(fp, 0, SEEK_END);
				len = ftell(fp);
				buffer = malloc(len);
				
				fseek(fp, 0, SEEK_SET);
				fread(buffer, 1, len, fp);
				
				//Load file
				tempptr = loadScript(buffer, len);
				if (!tempptr) {
					printf("can't load :( (Error %d)\n", errno);
					return 1;
				}
				
				scripts[sn] = *(LoadedScript*)tempptr;
				
				printf("OK\n");
				
				sn++;
				fclose(fp);
			}
		}
	}
	
	closedir(dp);
}
