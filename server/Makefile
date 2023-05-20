CFLAGS="-g -Wall -Weverything -fsanitize=all"

all: clean build/krsrv build/netcc

clean:
	rm -f build/*

build/krsrv: build/httpserver.o build/main.o
	gcc build/httpserver.o build/main.o -llua50 -dynamic -o build/krsrv

build/netcc: build/netc.o build/libnetc.o
	gcc build/*netc.o -dynamic -o build/netc

build/%.o: src/%.c
	gcc -c -o $@ $<

