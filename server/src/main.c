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

#define max(a,b) ((a)>(b)?(a):(b))

char *compile(char *script);
void exec(char *binscript);
int initserver();

int search_begin(char **array, int num_elements, char *string) {
	for (int i=0; i<num_elements; i++) {
		if (!strncmp(array[i], string, strlen(string))) {
			return i;
		}
	}
}

char *combine(char *restrict a, char *restrict b) {
	char *buffer = malloc( max(strlen(a), strlen(b)) );
	strncpy(buffer, a, BUFSIZ);
	strcat(buffer, b);
	return buffer;
}

char *getstr(FILE *fp) {
	char buffer[BUFSIZ];
	fscanf(fp, "%s", buffer);
	return buffer;
}

void set_env_variable() {
	printf("You should set the KRSERVER_PATH environment variable to the path you want to use as the server main directory.\n");
	exit(1);
}

int main(int argc, char **argv, char **envp) {
	printf("KR Server v0.02\n");
	
	srand(time(NULL));
	
	unsigned short port;
	
	if (argc<2) {
		port = 8080;
	} else {
		port = atoi(argv[1]);
	}

	printf("Using port %d.\n\n", port);
	
	int serversock = initserver(port);
	char rootpath[BUFSIZ];
	char cwdbuffer[BUFSIZ/2];
	
	getcwd(cwdbuffer, BUFSIZ/2);
	strcpy(rootpath, cwdbuffer);
	
	if (!getenv("KRSERVER_PATH")) set_env_variable();	
	strcpy(rootpath, realpath(getenv("KRSERVER_PATH"), NULL));
	
	printf("Using directory %s\n", rootpath);
	
	printf("\nLoading scripts...\n");
	
	char scriptpath[BUFSIZ];
	
	sprintf(scriptpath, "%s/scripts", rootpath);
	
	DIR *dp = opendir(scriptpath);
	
	char *buffer;
	
	if (!dp) {
		printf("can't open script directory :( (Errno %d)\n", errno);
		printf("You should create the 'scripts' and 'public' directories in the main server folder.\n");
	}
	
	struct dirent *ent;
	FILE *fp;
	
	while ((ent = readdir(dp))) {
		if (ent->d_name[0] != '.') {
			printf("Loading file %s\n", ent->d_name);
			
			
		}
	}
	
	closedir(dp);
}
