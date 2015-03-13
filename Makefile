CC=clang
CFLAGS=-c -Wall -g
LDFLAGS=-lpthread -lm -lX11
SOURCES=queue/queue.c threadpool/threadpool.c displayimage/displayimage.c timing/timing.c transform/transform_threadpool.c image.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=bimage

.PHONY: all clean

all: $(OBJECTS) $(EXECUTABLE)

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -g -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

