.SUFFIXES: .c .h .o .cu

# Set up the environment
CC=clang
CUDACC=nvcc
CFLAGS=-c -g
CUDACFLAGS=-c -arch sm_32
LDFLAGS=-lpthread -lm -lX11 -L/usr/local/cuda/lib -lcuda -lcudart
INCLUDE=-I/usr/local/cuda-6.5/include

# Define the deps
SOURCES=queue/queue.c displayimage/displayimage.c timing/timing.c threadpool/threadpool.c transform/transform_thread.c image.c
CUDASOURCES=#transform/transform_cuda.cu
OBJECTS=$(SOURCES:.c=.o)
CUDAOBJECTS=$(CUDASOURCES:.cu=.o)

# Ready
EXECUTABLE=bimage

.PHONY: all clean

all: $(CUDAOBJECTS) $(OBJECTS) $(EXECUTABLE)

clean:
	rm -rf $(OBJECTS) $(CUDAOBJECTS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(CUDAOBJECTS)
	$(CC) $(LDFLAGS) $(INCLUDE) $(OBJECTS) $(CUDAOBJECTS) -g -o $@

.c.o:
	$(CC) $< -o $@ $(INCLUDE) $(CFLAGS)

.cu.o:
	$(CUDACC) $(CUDACFLAGS) $< -o $@
