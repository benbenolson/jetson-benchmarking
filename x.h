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
int screen;
GC gc;
unsigned int white;
unsigned int black;

void update_screen(unsigned char *pixmap, int *width, int *height)
{
  XClearWindow(dsp, win);

  int x, y, pixsize;
  unsigned char *r, *g, *b;
  for(y = (*height) - 1; y > 0; --y) {
    for(x = 0; x < (*width); ++x) {
      b = pixmap++;
      g = pixmap++;
      r = pixmap++;
      XSetForeground(dsp, gc, ((*r)<<16)|((*g)<<8)|(*b));
      XDrawPoint(dsp, win, gc, x, y);
    }
  }

/*
  pixsize = 4;
  for(y = ((*height) / pixsize) - 1; y > 0; y -= pixsize) {
    for(x = 0; x < ((*width) / pixsize); x += pixsize) {
      r = pixmap++;
      g = pixmap++;
      b = pixmap++;
      XSetForeground(dsp, gc, ((*r)<<16)|((*g)<<8)|(*b));
      XFillRectangle(dsp, win, gc, x, y, pixsize, pixsize);
    }
  }
*/

  XFlush(dsp);
}

int display_image(unsigned char *pixmap, int *width, int *height)
{
  dsp = XOpenDisplay( NULL );
  if( !dsp ){ return 1; }

  screen = DefaultScreen(dsp);

  if (DefaultDepth(dsp,screen) != 24) {
    printf("Not a 24bit (truecolour) display");
    XCloseDisplay(dsp);
    return -1;
  }

  XRES = DisplayWidth(dsp,screen)/2;
  YRES = DisplayHeight(dsp,screen)/2;

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

  KeyCode keyQ;
  keyQ = XKeysymToKeycode(dsp, XStringToKeysym("Q"));

  XMapWindow(dsp, win);

  // wait until window appears
  do { XNextEvent(dsp,&evt); } while (evt.type != MapNotify);

  srand(time(0)); // only 1 sec resolution so use once per run
  update_screen(pixmap, width, height);

  int loop = 1;
  while(loop) {
    XNextEvent(dsp, &evt);
    switch(evt.type) {
      case(ButtonRelease):
        update_screen(pixmap, width, height);
        break;
      case(KeyRelease):
        if(evt.xkey.keycode == keyQ) { loop = 0; break; }
        update_screen(pixmap, width, height);
        break;
      case(ConfigureNotify):
        if(evt.xconfigure.width != XRES || evt.xconfigure.height != YRES) {
          XRES = evt.xconfigure.width;
          YRES = evt.xconfigure.height;
          update_screen(pixmap, width, height);
        }
        break;
      case(ClientMessage):
        if(evt.xclient.data.l[0] == wmDelete) loop = 0;
        break;
      default:
        break;
    }
  } 

  XDestroyWindow(dsp, win);
  XCloseDisplay(dsp);

  return 0;
}

