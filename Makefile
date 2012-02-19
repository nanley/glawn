# compiler and linker
CC = gcc
CCLD = gcc

# compiler and linker flags
CFLAGS = -Wall `pkg-config --cflags gtk+-2.0`
LDFLAGS = `pkg-config --libs gtk+-2.0 libcurl`

# sources and headers to use
SOURCES = main.c callbacks.c gui.c
HEADERS = callbacks.h

.PHONY: all test clean

# recompile and execute
all: glawn test

# link objects
glawn: $(SOURCES:.c=.o)
	$(CCLD) $^ $(LDFLAGS) -o $@

# compile sources into objects
%.o: %.c
	$(CC) -g $(CFLAGS) -c $^

test:
	./bin/glawn

clean:
	rm -f bin/*.o
	rm -f bin/*.gch
	rm -f src/*.~
