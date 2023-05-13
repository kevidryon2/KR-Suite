/* krnet.c
 *
 * Copyright 2023 Kevin Rossi <unknown@domain.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <hirolib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "verbs.h"
#include "parse.h"


void drawHLine(int x, int y, int l, char c) {
	MoveCursor(x, y);
	for (int i=0; i<l; i++) putchar(c);
}

void drawVLine(int x, int y, int l, char c) {
	MoveCursor(x, y);
	for (int i=0; i<l; i++) {
		MoveCursorDown(1);
		MoveCursorLeft(1);
		putchar(c);
	}
}

void drawBox(int x, int y, int w, int h, char corner, char borderSides, char borderTopBottom, char fill) {
	//Draw top
	MoveCursor(x,y);
	putchar(corner);
	drawHLine(x+1, y, w-2, borderTopBottom);
	putchar(corner);
	
	//Draw bottom
	MoveCursor(x,y+h-1);
	putchar(corner);
	drawHLine(x+1, y+h-1, w-2, borderTopBottom);
	putchar(corner);
	
	//Draw left
	drawVLine(x+1, y, h-2, borderSides);
	
	//Draw right
	drawVLine(x+w, y, h-2, borderSides);
	
	//Fill
	for (int i=0; i<h-2; i++) {
		drawHLine(x+1, y+i+1, w-2, fill);
	}
}

struct winsize ws;
HTTPVerb verb = HTTP_GET;

int cy;

void echo(char *text) {
	MoveCursor(1, cy+2);
	if ((cy+1+strlen(text)/ws.ws_col)>ws.ws_row-5) {
		ClearScreen();
		cy = 0;
		MoveCursor(1, 2);
		echo("...\n");
	}
	printf(text, verb);
	cy += 1 + strlen(text)/ws.ws_col;
}

void updateScreen() {
	MoveCursorHome();
    ioctl(0, TIOCGWINSZ, &ws);
	drawHLine(0, 0, ws.ws_col, '-');
	drawBox(1, ws.ws_row-2, ws.ws_col-1, 3, 'O', '|', '-', ' ');
	MoveCursorLeft(ws.ws_col-3);
}

int main(int argc, char **argv) {
	MoveCursorHome();
	ClearScreen();
	
	char buffer[BUFSIZ];
	char **buffer2;
	updateScreen();
	echo("KR Network v0.07p Coryright (C) 2023 kevidryon2\n");
	echo("This program comes with ABSOLUTELY NO WARRANTY; for details type 'warranty'.");
	echo("This is free software, and you are welcome to redistribute it under certain conditions; type 'copying' for details.");
	echo("");
	MoveCursor(2, ws.ws_row-1);
	
	while (1) {
		if (fgets(buffer, BUFSIZ, stdin) == EOF) {
			MoveCursorHome();
			ClearScreen();
			return 1;
		} else {
				switch (parse(buffer)) {
				case 0:
					break;
				case -1:
					echo("Invalid command.");
			}
		}
		updateScreen();
		MoveCursor(2, ws.ws_row-1);
	}
}
