all: install clean build/krnet

install:
	

clean:
	rm -f build/*

build/%: src/%.c
	gcc -o $@ $< -g -lcurl
