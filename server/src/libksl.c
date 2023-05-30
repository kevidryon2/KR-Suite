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

#include <stdio.h>

//Search N in H
int needle(char *n, char **h, int lh) {
	for (int i=0; i<lh; i++) {
		if (!strcmp(h[i], n)) {
			return i;
		}
	}
	return -1;
}

char *argparses(int argc, char **argv, int start) {
	char *buffer = malloc(BUFSIZ);
	buffer[0] = 0;
	for (int i=start; i<=argc; i++) {
		strcat(buffer, argv[i]);
		if (i < argc) strcat(buffer, " ");
	}
	return buffer;
}
