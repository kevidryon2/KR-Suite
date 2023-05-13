all: clean tests testr testa testsr testsa testra testsra

test%: build/test%.txt build/krenc
	./build/krenc e $< c001cafe $(echo $@ | cut -c 5-)
	
clean:
	rm -f build/*
	
build/test%.txt: testfile.txt
	cp testfile.txt $@

build/krenc: main.c decrypt.c encrypt.c
	gcc main.c decrypt.c encrypt.c -o build/krenc
	
