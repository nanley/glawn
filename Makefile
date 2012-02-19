.PHONY: all test clean

# recompile and execute
all: glawn test

# link objects
glawn: 
	make src/Makefile
	mv src/glawn bin/glawn

test:
	./bin/glawn

clean:
	rm -f bin/*
	rm -f src/*.o
