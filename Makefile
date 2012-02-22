.PHONY: all clean

# recompile and execute
all: glawn

# hand off compile and move executable
glawn: 
	make -C src
	mv src/glawn .

clean:
	rm -f glawn
	make -C src clean
