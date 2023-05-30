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
#include <sys/stat.h>
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

//KR Server Version String
#define KRS_VERS "0.05"

const char *verbs[] = {"GET","POST","PUT","PATCH","DELETE","HEAD","OPTIONS"};
typedef enum {
	VERB_GET,
	VERB_POST,
	VERB_PUT,
	VERB_PATCH,
	VERB_DELETE,
	VERB_HEAD,
	VERB_OPTIONS
} HTTPVerb;

#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((b)>(a)?(a):(b))

typedef struct {
	char protocol[8];
	char rverb[8];
	char path[4096];
	struct {
		char key[32];
		char value[32];
	} headers[64];
	char *body;
} RequestData;

LoadedScript *loadScript(char *data, int len);

char *compile(char *script);
void exec(char *binscript);
int initserver();
void execscript(LoadedScript script, RequestData reqdata, char *resbuff);

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

char *randstr(int len) {
	char *s = malloc(len+1);
	for (int i=0; i<len; i++) {
		s[i] = rand()%26+'a';
	}
	s[len] = 0;
	return s;
}

char *escapestr(char *s) {
	char *o = malloc(1);
	int csiz = 1;
	for (int i=0; i<strlen(s); i++) {
		csiz++; o = realloc(o, csiz);
		switch (s[i]) {
			case '/':
				o[csiz-1] = '_';
				break;
			case '_':
				csiz+=2; o = realloc(o, csiz);
				strcat(o, "__");
			default:
				o[csiz-1] = s[i];
				break;
		}
	};
	
	o++;
	o[csiz-1] = 0;
	
	return o;
}

char *unescapestr(char *s) {
	char *un = malloc(1);
	int usiz = 1;
	
	for (int i=0; i<strlen(s); i++) {
		usiz++; un = realloc(un, usiz);
		switch (s[i]) {
			case '_':
				if (s[i+1] == '_')
					un[usiz-1] = '_';
				else un[usiz-1] = '/';
				break;
			default:
				un[usiz-1] = s[i];
		}
	}
	
}

void set_env_variable() {
	printf("You should set the KRSERVER_PATH environment variable to the path you want to use as the server main directory.\n");
	exit(1);
}

int main(int argc, char **argv, char **envp) {
	printf("KR Server "KRS_VERS"\n");
	
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
		printf("You should create the 'scripts', 'public', 'cache' (the latter as a ramdisk) directories in the main server folder.\n");
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
	int cverb;
	int statcode;
	int script;
	int bodylen;
	
	struct sockaddr_in caddr;
	socklen_t caddrl;
	char reqbuff[BUFSIZ];
	char *resbuff = malloc(BUFSIZ);
	RequestData reqdata;
	
	/* I: Read Request
	 * P: Parse Request
	 * H: Check HTTP Version and Verb
	 * 	S: Search and Execute Script
	 * 	F: Return file, checking cache
	 *   C: File is cached
	 *   N: File is not cached
	 * O: Send Response
	 * 
	 */
	
	FILE *tmpfp;
	
	FILE *publicfp;
	FILE *cachedfp;
	
	char *line;
	char *body;
	
	while (true) {
		csock = accept(serversock, &caddr, &caddrl);
		printf("Recieved Request (Address %d.%d.%d.%d, Port %d) ",
			   caddr.sin_addr.s_addr%256,
			   (caddr.sin_addr.s_addr>>8)%256,
			   (caddr.sin_addr.s_addr>>16)%256,
			   (caddr.sin_addr.s_addr>>24)%256,
			   be16toh(caddr.sin_port)
		);
		
		memset(buffer, 0, BUFSIZ);
		memset(resbuff, 0, BUFSIZ);
		memset(reqbuff, 0, BUFSIZ);
		memset(&reqdata, 0, sizeof(reqdata));
		
		//Read request
		putchar('I');
		read(csock, reqbuff, BUFSIZ);
		
		//Parse request
		putchar('P');
		line = ntoken(reqbuff, "\n", 0);
		
		if (!ntoken(line, " ", 2)) {
			SetColor16(COLOR_RED);
			printf("H");
			ResetColor16();
			sprintf(resbuff, "Bad Protocol\n");
			write(csock, resbuff, strlen(resbuff));
			goto endreq;
		}
		
		strncpy(reqdata.rverb, ntoken(line, " ", 0), 7);
		strcpy(reqdata.path, ntoken(line, " ", 1));
		strncpy(reqdata.protocol, ntoken(line, " ", 2), 7);
		
		
		//Verify client is using HTTP 1.0 or HTTP 1.1 Protocol
		if (!(startswith(reqdata.protocol, "HTTP/1.0") ||
			  startswith(reqdata.protocol, "HTTP/1.1"))) {
				SetColor16(COLOR_RED);
				putchar('H');
				ResetColor16();
				sprintf(resbuff, "Bad Protocol\n");
				write(csock, resbuff, strlen(resbuff));
				goto endreq;
		} else if (needle(reqdata.rverb, verbs, 7) < 0) {
			SetColor16(COLOR_RED);
			printf("V (Bad verb %s)", reqdata.rverb);
			ResetColor16();
			sprintf(resbuff, "HTTP/1.0 501 Not Implemented\nServer: KRServer/"KRS_VERS"\n\nInvalid Method");
			write(csock, resbuff, strlen(resbuff));
			goto endreq;
		}
		putchar('H');
		
		for (int i=0; i<256; i++) {
			for (int j=0; j<16; j++) {
				if (scripts[i].paths[j])
				if (!strcmp(scripts[i].paths[j], reqdata.path)) {
					goto script;
				}
			}
		};
		
		//TODO: Parse headers
		
		goto noscript;
script:
		putchar('S');
		execscript(scripts[script], reqdata, resbuff);
		goto response;
		
noscript:
		//Fetch file
		putchar('F');
		memset(buffer, 0, BUFSIZ);
		sprintf(buffer, "%s/public/%s", rootpath, reqdata.path);
		
		//If file doesn't exist in public directory return 404
		if (!(publicfp = fopen(buffer, "r"))) {
			SetColor16(COLOR_RED);
			printf(" %s ", reqdata.path);
			ResetColor16();
			sprintf(resbuff, "HTTP/1.0 404 Not Found\nServer: KRServer/"KRS_VERS);
			write(csock, resbuff, strlen(resbuff));
			goto endreq;
		}
		
		//File exists
		printf(" %s ", reqdata.path);
		memset(buffer, 0, BUFSIZ);
										 
		sprintf(buffer, "%s/cache/%s", rootpath, escapestr(reqdata.path));
										 
		//Is file in cache?
		if ((cachedfp = fopen(buffer, "r"))) {
			putchar('C');
										 
			//Read cached file
			fseek(cachedfp, 0, SEEK_END);
			body = malloc(ftell(cachedfp));
			fseek(cachedfp, 0, SEEK_SET);
			fread(body, 1, bodylen, cachedfp);
			
			fclose(cachedfp);
			
		} else {
			putchar('N');
			//If not cache file
			cachedfp = fopen(buffer, "w"); //open cached file
			
			memset(buffer, 0, BUFSIZ);
			sprintf(buffer, "%s/public/%s", rootpath, reqdata.path);
			
    		struct stat path_stat;
    		stat(buffer, &path_stat);
			
			if (!S_ISREG(path_stat.st_mode)) {
				//TODO: List directory
				SetColor16(COLOR_RED);
				putchar('D');
				ResetColor16();
				sprintf(resbuff, "HTTP/1.0 400 Bad Request\nServer: KRServer/"KRS_VERS);
				write(csock, resbuff, strlen(resbuff));
				goto endreq;
			}
			
			//read data from file and write it to cached file
			fseek(publicfp, 0, SEEK_END);
			body = malloc((bodylen = ftell(publicfp)));
			fseek(publicfp, 0, SEEK_SET);
			fread(body, 1, bodylen, publicfp);
			fwrite(body, 1, bodylen, cachedfp);
										 
			//close file handles
			fclose(cachedfp);
			fclose(publicfp);
		};
		
response:
		//Send response
		putchar('O');
		
		sprintf(resbuff, "HTTP/1.0 200 OK\nServer: KRServer/"KRS_VERS"\n\n");
		write(csock, resbuff, strlen(resbuff));
		write(csock, body, bodylen);
										 
endreq:
		//Finish and flush
		putchar('\n');
		fflush(stdout);
		close(csock);
	}
}
