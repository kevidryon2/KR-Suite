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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define NETC_VERSION_MAJOR 0x00
#define NETC_VERSION_MINOR 0x01

#define NUM_KEYWORDS 1
const char *keywords[NUM_KEYWORDS] = {
	"path"
};

//Search N in H
int needle(char *n, char **h, int lh) {
	for (int i=0; i<lh; i++) {
		if (!strcmp(h[i], n)) {
			return i;
		}
	}
	return -1;
}

typedef struct {
	char magic[4]; //Always 'nets'
	unsigned char ver_major;
	unsigned char ver_minor;
	char paths[64]; //An array of zero-terminated path strings
} NetScriptHeader;

char *compile(char *code) {
	int linecount = 1;
	char **lines = calloc(sizeof(char*), 16384);
	char **tokens;
	
	//Split CODE into LINES
	lines[0] = strtok(code, "\n");
	for (int i=1; (lines[i] = strtok(NULL, "\n")); i++, linecount++);

	//Build header
	NetScriptHeader h;
	memcpy(h.magic, "nets", 4);
	h.ver_major = NETC_VERSION_MAJOR;
	h.ver_minor = NETC_VERSION_MINOR;
	int tokencount;
	
	int empty = 0;
	
	for (int i=0; i<linecount; i++) {
		
		if (!lines[i][0]) continue;
		
		//Find first non-whitespace character of line
		while (lines[i][0] == ' '  || 
			   lines[i][0] == '\t') {
				   lines[i]++;
				   if (!lines[i][0]) empty = 1;
			   }
		
		//If empty string skip
		if (empty) {
			empty = 0;
			goto end; //Yes, I'm one of the very few C programmers in the world to use gotos.
		}
		
		//Allocate tokens
		tokens = calloc(sizeof(char*), 16384);
		tokencount = 0;
		
		//Split current line into tokens
		tokens[0] = strtok(lines[i], " ");
		for (int j=1; (tokens[j] = strtok(NULL, "\n")); j++, tokencount++);
		
		//Do the appropriate action for command
		switch (needle(tokens[0], keywords, NUM_KEYWORDS)) {
			case -1:
				printf("Error: Unknown keyword %s at line %d\n", tokens[0], i+1);
				break;
			
			case 0:
				printf("\n");
				break;
		}
		
		end:
		free(tokens);	
	}
	
	return "";
}
