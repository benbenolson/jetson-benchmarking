#ifndef DISPLAYIMAGE_H
#define DISPLAYIMAGE_H

#include <X11/Xlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "../timing/timing.h"
#include "../transform/transform_threadpool.h"

struct Keys
{
  KeyCode Q, W, S;
};

struct XWin
{
  // Resolution of the window
  int xres, yres;

  // Generic X11 stuff
  Atom wmdelete;
  XEvent *evt;
  Display *dsp;
  Window win;
  Visual *visual;
  GC gc;


  // Key events
  struct Keys *keys;
};

void update_screen(XImage *ximage, unsigned char *pixmap, int width, int height, 
                   int depth, struct XWin *xwin);
void event_loop(unsigned char *pixmap, unsigned char *pixmapmod, int width, int height, 
                int depth, struct XWin **xwin);
void xwindow_init(unsigned char *pixmap, unsigned char *pixmapmod, int width, int height, 
                  int depth, struct XWin **xwin);
void xwindow_del(struct XWin **xwin);

#endif
