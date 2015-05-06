# jetson-benchmarking
Custom program to benchmark the NVidia Jetson.

## Compiling

To compile, first choose a version of the program that you would like to run. At the moment, your options are: 
  - nothread
  - thread
  - threadpool
  - cuda
 
Once you choose this, use `make` to compile the program; for example:
  ```
  make -f Makefile_cuda
  ```

Once this finishes, you'll have an executable in the current directory named `bimage_[something]`, where `[something]`
is the version that you chose.

## Running
You can then run the program without any argument to see its `usage` command. The arguments, for obvious reasons,
differs between the executables (for example, the `cuda` version takes a `split` argument that determines the amount
of work that goes on the GPU).


## Results
The program will run the benchmark and output a number on `stdout` indicating the amount of time that it took to
complete, with a lower number being better. Check in `image.c` to see how many frames it did (depending on when
you `pull`ed this repo, it could be either `60` or `1200`). The number of frames, divided by the result that you got,
will determine your FPS result.

## Purposes
This program is a custom application, built from scratch, that is intended to benchmark the NVidia Jetson (using
both the GPU and CPU concurrently, just the CPU, or just the GPU). It provides the user with a convenient interface
for loading in bitmap images, and doing various image processing calculations on it in order to see how quickly it
can actually do them, in a real-life scenario.

Another purpose of this project is to have hardly any external dependencies; thus, the dependencies are:
  - X11.
  - Pthreads.
This requirement, which was completely optional on my part, turned out to be more of a challenge than I originally
anticipated.  For example, for the `threadpool` implementation, I needed a `queue`; as such, I had to write my own.
Also, I quickly found out that Xlib came straight from Satan's buttcrack, but after much toiling got it to work.

## Closing Thoughts
The application runs much faster without having to display the image to the screen, so I think there must be
something wrong with my Xlib code.  Either that, or raw Xlib is just extremely slow regardless.