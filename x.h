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

void update_screen(unsigned char *pixmap, int width, int height, int depth)
{
  XImage *ximage = XCreateImage(dsp, visual, 24, ZPixmap, 0, (char *)pixmap, (unsigned int)width, (unsigned int)height, 32, 0);
  XPutImage(dsp, win, gc, ximage, 0, 0, 0, 0, width, height);

  XFlush(dsp);
}

int display_image(unsigned char *pixmap, int width, int height, int depth)
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

  KeyCode keyQ, keyW, keyS;
  keyQ = XKeysymToKeycode(dsp, XStringToKeysym("Q"));
  keyW = XKeysymToKeycode(dsp, XStringToKeysym("W"));
  keyS = XKeysymToKeycode(dsp, XStringToKeysym("S"));

  XMapWindow(dsp, win);

  // wait until window appears
  do { XNextEvent(dsp,&evt); } while (evt.type != MapNotify);

  srand(time(0)); // only 1 sec resolution so use once per run
  update_screen(pixmap, width, height, depth);

  int loop = 1;
  while(loop) {
    XNextEvent(dsp, &evt);
    switch(evt.type) {
      case(KeyRelease):
        if(evt.xkey.keycode == keyQ) {
          loop = 0; break;
        } else if(evt.xkey.keycode == keyW) {
          timing(apply_gamma(pixmap, width, height, depth, 1.2));
        } else if(evt.xkey.keycode == keyS) {
          apply_gamma(pixmap, width, height, depth, 1 / 1.2);
        }
        break;
      case(ConfigureNotify):
        if(evt.xconfigure.width != XRES || evt.xconfigure.height != YRES) {
          XRES = evt.xconfigure.width;
          YRES = evt.xconfigure.height;
        }
        break;
      case(ClientMessage):
        if(evt.xclient.data.l[0] == wmDelete) loop = 0;
        break;
      default:
        break;
    }
    update_screen(pixmap, width, height, depth);
  } 

  XDestroyWindow(dsp, win);
  XCloseDisplay(dsp);

  return 0;
}
