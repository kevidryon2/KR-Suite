all: clean build/krgfx

clean:
	rm -rf build/*

build/krgfx: build/krgfx.o build/spng.o build/libz.a build/qoi.o
	gcc -g build/*.o build/*.a -lm -o build/krgfx
	gcc -g build/*.o -lm -L/usr/lib/x86_64-linux-gnu -lz -o build/krgfx_dyn

build/%.o: src/%.c
	gcc -O2 -c $< -o $@
	
build/%.o: libs/%.c
	gcc -O2 -c $< -o $@

build/libz.a: libs/zlib-1.2.13.tar.gz
	cp libs/zlib-1.2.13.tar.gz build
	cd build && tar -xf zlib-1.2.13.tar.gz
	cd build/zlib-1.2.13 && ./configure
	cd build/zlib-1.2.13 && make libz.a
	cp build/zlib-1.2.13/libz.a build
