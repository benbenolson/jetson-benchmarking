# jetson-benchmarking
Custom program to benchmark the NVidia Jetson.

Compile with the complicated command: `make`.

The program accepts one argument: the filepath of a BMP image.  It only supports 24-bit images right now,
but could rather easily support other image depths in the future, if my project calls for it.

Upon launching, the program simply displays the image 
(or as much of the image as your screen resolution allows),
at which point you can press a number of keys:

  - W to increase the gamma
  - S to decrease the gamma
  - Q to quit

Each of these operations takes a while right now, but that's the point of this project: to optimize them as much as possible. In the future, the program is going to be able to use a CUDA-enabled GPU to do these calculations.
As of now, it only uses threads, although a nice threadpool implementation is on the way.

Another purpose of this project is to have hardly any external dependencies; thus, the dependencies are:

  - X11. If you're not using this, you're in Windows, and you shouldn't be running this program anyway.
  - Pthreads.
  - stdlib.h. If you don't have this, you're not using a computer. 
