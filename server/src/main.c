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
#include <stdbool.h>
#include <hirolib.h>
#include "bns.h"
#include "server.h"

#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((b)>(a)?(a):(b))

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

int startswith(char *s, char *c) {
	return !strncmp(s, c, min(strlen(c), strlen(s)));
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

char *ntoken(char *const s, char *d, int t) {
	char *tk = malloc(strlen(s)+1);
	strcpy(tk, s);
	strtok(tk, d);
	for (int i=0; i<t; i++) {
		tk = strtok(0, d);
	}
	return tk;
}

void set_env_variable() {
	printf("You should set the KRSERVER_PATH environment variable to the path you want to use as the server main directory.\n");
	exit(1);
}

int main(int argc, char **argv, char **envp) {
	printf("KR Server v0.04\n");
	
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
	char *fullpath;
	
	getcwd(cwdbuffer, BUFSIZ/2);
	strcpy(rootpath, cwdbuffer);
	
	if (!getenv("KRSERVER_PATH")) set_env_variable();
	
	if (!(fullpath = realpath(getenv("KRSERVER_PATH"), NULL))) {
		perror(getenv("KRSERVER_PATH"));
		return 127;
	}
	
	strcpy(rootpath, fullpath);
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
				
				printf("OK (Script %d)\n", sn);
				
				sn++;
				fclose(fp);
			}
		}
	};
	closedir(dp);
	
	printf("Loaded KR Server. Accepting requests.\n\n");
	
	int csock;
	struct sockaddr_in caddr;
	socklen_t caddrl;
	char reqbuff[BUFSIZ];
	char resbuff[BUFSIZ];
	struct {
		char protocol[8];
		char rverb[8];
		char path[PATH_MAX];
		struct {
			char key[32];
			char value[32];
		} headers[64];
	} resdata;
	
	/* I: Read Request
	 * P: Parse Request
	 * H: Check HTTP Version
	 * S: Execute Scripts
	 * O: Send Response
	 * 
	 */
	
	char *line;
	
	while (true) {
		csock = accept(serversock, &caddr, &caddrl);
		printf("Recieved Request (Address %d.%d.%d.%d, Port %d) ",
			   caddr.sin_addr.s_addr%256,
			   (caddr.sin_addr.s_addr>>8)%256,
			   (caddr.sin_addr.s_addr>>16)%256,
			   (caddr.sin_addr.s_addr>>24)%256,
			   be16toh(caddr.sin_port)
		);
		
		memset(resbuff, 0, BUFSIZ);
		memset(reqbuff, 0, BUFSIZ);
		memset(&resdata, 0, sizeof(resdata));
		
		//Read request
		putchar('I');
		read(csock, reqbuff, BUFSIZ);
		
		//Parse request
		putchar('P');
		line = ntoken(reqbuff, "\n", 0);
		
		fprintf(stderr, "%s\n", line);
		fprintf(stderr, "%s\n", line);
		
		if (!ntoken(line, " ", 2)) {
			SetColor16(COLOR_RED);
			printf("H");
			ResetColor16();
			sprintf(resbuff, "Bad Protocol\n");
			write(csock, resbuff, strlen(resbuff));
			goto endreq;
		}
		
		strncpy(resdata.rverb, ntoken(line, " ", 0), 7);
		strcpy(resdata.path, ntoken(line, " ", 1));
		strncpy(resdata.protocol, ntoken(line, " ", 2), 7);
		
		
		//Verify client is using HTTP 1.0 or HTTP 1.1 Protocol
		putchar('H');
		if (!(startswith(resdata.protocol, "HTTP/1.0") ||
			  startswith(resdata.protocol, "HTTP/1.1"))) {
				SetColor16(COLOR_RED);
				printf("H");
				ResetColor16();
				sprintf(resbuff, "Bad Protocol\n");
				write(csock, resbuff, strlen(resbuff));
				goto endreq;
			}
		
		//Send response
		putchar('O');
		sprintf(resbuff, "HTTP/1.1 200 OK\nServer: KRServer/0.04");
		write(csock, resbuff, strlen(resbuff));
		
endreq:
		//Finish and flush
		putchar('\n');
		fflush(stdout);
		close(csock);
	}
}
