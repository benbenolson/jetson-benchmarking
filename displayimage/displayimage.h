#ifndef DISPLAYIMAGE_H
#define DISPLAYIMAGE_H

#include <X11/Xlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

// Globals necessary for X11
int XRES;
int YRES;
int NPTS=10000;
Display *dsp;
Window win;
Visual *visual;
int screen;
GC gc;
unsigned int white;
unsigned int black;

// Struct to pass the keys around
struct Keys
{
  KeyCode Q, W, S;
};

void update_screen(unsigned char *pixmap, int width, int height, int depth)
{
  XImage *ximage = XCreateImage(dsp, visual, 24, ZPixmap, 0, (char *)pixmap, (unsigned int)width, (unsigned int)height, 32, 0);
  XPutImage(dsp, win, gc, ximage, 0, 0, 0, 0, width, height);

  XFlush(dsp);
}

void event_loop(unsigned char *pixmap, unsigned char *pixmapmod, int width, int height, int depth, struct Keys *keys, XEvent *evt, Atom *wmDelete)
{
  float gamma = 1;
  struct timeval tv;

  int x11_fd = ConnectionNumber(dsp);
  fd_set in_fds;

  int loop = 1;
  while(loop) {
    FD_ZERO(&in_fds);
    FD_SET(x11_fd, &in_fds);
    tv.tv_usec = 33333.3;
    tv.tv_sec = 0;
    if(select(x11_fd + 1, &in_fds, 0, 0, &tv)) {
      switch(evt->type) {
        case(KeyRelease):
          if(evt->xkey.keycode == keys->Q) {
            loop = 0; break;
          } else if(evt->xkey.keycode == keys->W) {
            if(gamma < 10.0) {
              gamma += 0.1;
              timing(apply_gamma(pixmap, pixmapmod, width, height, depth, gamma));
            }
          } else if(evt->xkey.keycode == keys->S) {
            if(gamma > 0.0) {
              gamma -= 0.1;
              timing(apply_gamma(pixmap, pixmapmod, width, height, depth, gamma));
            }
          }
          break;
        case(ConfigureNotify):
          if(evt->xconfigure.width != XRES || evt->xconfigure.height != YRES) {
            XRES = evt->xconfigure.width;
            YRES = evt->xconfigure.height;
          }
          break;
        case(ClientMessage):
          if(evt->xclient.data.l[0] == *wmDelete) loop = 0;
          break;
        default:
          break;
      }
    } else {
      update_screen(pixmapmod, width, height, depth);
    }
    while(XPending(dsp)) {
      XNextEvent(dsp, evt);
    }
  } 
}

int display_image(unsigned char *pixmap, unsigned char *pixmapmod, int width, int height, int depth)
{
  dsp = XOpenDisplay( NULL );
  if( !dsp ){ return 1; }

  screen = DefaultScreen(dsp);

  if (DefaultDepth(dsp,screen) != 24) {
    printf("Not a 24bit (truecolour) display");
    XCloseDisplay(dsp);
    return -1;
  }
  
  if(DisplayWidth(dsp, screen) >= width) {
    XRES = width;
  } else {
    XRES = DisplayWidth(dsp, screen);
  }
  
  if(DisplayHeight(dsp, screen) >= height) {
    YRES = height;
  } else {
    YRES = DisplayHeight(dsp, screen);
  }

  white = WhitePixel(dsp,screen);
  black = BlackPixel(dsp,screen);

  win = XCreateSimpleWindow(dsp,
                            DefaultRootWindow(dsp),
                            0, 0,   // origin
                            XRES, YRES, // size
                            0, black, // border width/clr
                            black);   // backgrd clr


  Atom wmDelete=XInternAtom(dsp, "WM_DELETE_WINDOW", True);
  XSetWMProtocols(dsp, win, &wmDelete, 1);

  gc = XCreateGC(dsp, win,
                 0,       // mask of values
                 NULL);   // array of values

  XSetForeground(dsp, gc, black);

  XEvent evt;
  long eventMask = StructureNotifyMask;
  eventMask |= ButtonPressMask|ButtonReleaseMask|KeyPressMask|KeyReleaseMask;
  XSelectInput(dsp, win, eventMask);

  struct Keys keys;
  keys.Q = XKeysymToKeycode(dsp, XStringToKeysym("Q"));
  keys.W= XKeysymToKeycode(dsp, XStringToKeysym("W"));
  keys.S = XKeysymToKeycode(dsp, XStringToKeysym("S"));

  XMapWindow(dsp, win);

  // wait until window appears
  do { XNextEvent(dsp,&evt); } while (evt.type != MapNotify);

  srand(time(0));
  update_screen(pixmap, width, height, depth);
  
  event_loop(pixmap, pixmapmod, width, height, depth, &keys, &evt, &wmDelete);

  XDestroyWindow(dsp, win);
  XCloseDisplay(dsp);

  return 0;
}

#endif
