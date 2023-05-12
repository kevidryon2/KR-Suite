#include <hirolib.h>
#include <unistd.h>

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

int boxw, boxh;

void updateScreen() {
	MoveCursorHome();
	ClearScreen();
	drawBox(2,2,boxw+6,boxh+6,'O','|','-','X');
}

int main(int argc, char **argv) {
	while (1) {
		updateScreen();
		switch (getchar_unlocked()) {
			case 'w': boxh--; break;
			case 's': boxh++; break;
			case 'a': boxw--; break;
			case 'd': boxw++; break;
			case EOF: MoveCursorHome(); ClearScreen(); return 0;
		}
	}
}
