CC=clang
CFLAGS=-c -Wall
LDFLAGS=-lpthread -lm -lX11
SOURCES=queue/queue.c threadpool/threadpool.c image.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=bimage

.PHONY: all clean

all: $(OBJECTS) $(EXECUTABLE)

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@
