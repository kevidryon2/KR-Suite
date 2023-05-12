all: clean build/krnet

clean:
	rm -f build/*

build/%: src/%.c
	gcc -o $@ $< -g
