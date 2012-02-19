.PHONY: all test clean

# recompile and execute
all: glawn test

# hand off compile and move executable
glawn: 
	cd src; make
	mv src/glawn bin/glawn

test:
	./bin/glawn

clean:
	rm -f bin/*
	cd src; make clean
