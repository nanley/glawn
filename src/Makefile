# compiler and linker
CC = gcc
CCLD = gcc

# compiler and linker flags
CFLAGS = -Wall `pkg-config --cflags gtk+-2.0`
LDFLAGS = `pkg-config --libs gtk+-2.0 libcurl`

# sources and headers to use
SOURCES = main.c callbacks.c gui.c
HEADERS = callbacks.h

.PHONY: all clean

# recompile and execute
all: glawn

# link objects
glawn: $(SOURCES:.c=.o)
	$(CCLD) $^ $(LDFLAGS) -o $@
	mv glawn ../bin

# compile sources into objects
%.o: %.c
	$(CC) -g $(CFLAGS) -c $^

clean:
	rm -f *.o
	rm -f *.gch
	rm -f *.~
