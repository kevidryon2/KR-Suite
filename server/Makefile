all: clean build/krsrv build/netcc

clean:
	rm -f build/*

build/krsrv: build/httpserver.o build/main.o build/bnsload.o
	gcc -g build/httpserver.o build/main.o build/bnsload.o -o build/krsrv
	
build/krsrv_portable: build/httpserver.o build/main.o build/bnsload.o
	gcc -g build/httpserver.o build/main.o build/bnsload.o -static -o build/krsrv_portable

build/netcc: build/netc.o build/libnetc.o
	gcc -g build/*netc.o -dynamic -o build/netc

build/%.o: src/%.c
	gcc -g -c -o $@ $<

