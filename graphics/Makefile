CFLAGS = "-flto -ffast-math -fno-threadsafe-statics -fno-unroll-loops -finline-limit=n"

all: clean src/qoi.h build/krgfx_opt build/krgfx_dbg build/krgfx build/qoie build/qoid
clean:
	rm -rf build/*

build/krgfx_static: build/krgfx.o build/spng.o build/libz.a build/qoi.o
	gcc -g build/krgfx.o build/spng.o build/libz.a build/qoi.o -lm -o build/krgfx_static

build/krgfx: build/krgfx.o build/spng.o build/qoi.o
	gcc -g build/*.o -lm -lz -o build/krgfx
	
build/krgfx_opt_static: build/krgfx.o build/spng.o build/libz.a build/qoi.o
	gcc -Os build/krgfx.o build/spng.o build/libz.a build/qoi.o -lm -o build/krgfx_opt_static

build/krgfx_opt: build/krgfx.o build/spng.o build/qoi.o
	gcc -Os build/*.o -lm -lz -o build/krgfx_opt
	
build/krgfx_dbg_static: build/krgfx.o build/spng.o build/libz.a build/qoi.o
	gcc -Os -g build/krgfx.o build/spng.o build/libz.a build/qoi.o -lm -o build/krgfx_dbg_static

build/krgfx_dbg: build/krgfx.o build/spng.o build/qoi.o
	gcc -Os -g build/*.o -lm -lz -o build/krgfx_dbg




build/%.o: src/%.c
	gcc -O2 -c $< -o $@

build/%.o: libs/%.c
	gcc -O2 -c $< -o $@

build/qoid: build/qoi.o build/qoidecode.o build/spng.o
	gcc build/qoi.o build/qoidecode.o build/spng.o -lz -lm -o build/qoid
	
build/qoie: build/qoi.o build/qoiencode.o build/spng.o
	gcc build/qoi.o build/qoiencode.o build/spng.o -lz -lm -o build/qoie

build/libz.a: libs/zlib-1.2.13.tar.gz
	cp libs/zlib-1.2.13.tar.gz build
	cd build && tar -xf zlib-1.2.13.tar.gz
	cd build/zlib-1.2.13 && ./configure
	cd build/zlib-1.2.13 && make libz.a
	cp build/zlib-1.2.13/libz.a build
	
src/qoi.h: libs/qoi.h
	cp libs/qoi.h src
