.PHONY: all clean

# create executable
all: glawn

# hand off compilation and move executable
glawn: 
	make -C src
	mv src/glawn .

clean:
	rm -f glawn
	make -C src clean
