CC=clang
CFLAGS=-c -Wall -g
LDFLAGS=-lpthread -lm -lX11
SOURCES=../queue/queue.c threadpool.c test.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=test

.PHONY: all clean

all: $(OBJECTS) $(EXECUTABLE)

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -g -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@
