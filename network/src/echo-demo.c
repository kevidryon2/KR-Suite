#include <hirolib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "verbs.h"

#define max(a,b) (a>b?a:b)

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
	printf(text, verb);
	cy++;
}

void updateScreen() {
	MoveCursorHome();
    ioctl(0, TIOCGWINSZ, &ws);
	drawHLine(0, 0, ws.ws_col, '-');
	drawBox(1, ws.ws_row-2, ws.ws_col-1, 3, 'O', '|', '-', ' ');
	MoveCursorLeft(ws.ws_col-3);
}

int main(int argc, char **argv) {
	char buffer[BUFSIZ];
	updateScreen();
	echo("Ready.");
	MoveCursor(2, ws.ws_row-1);
	
	while (1) {
		if (scanf("%s", buffer) == EOF) {
			MoveCursorHome();
			ClearScreen();
			return 1;
		}
		updateScreen();
		echo(buffer);
		MoveCursor(2, ws.ws_row-1);
	}
}
