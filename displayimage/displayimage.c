#include "displayimage.h"

void update_screen(XImage *ximage, unsigned char *pixmap, int width, int height, int depth, struct XWin *xwin)
{
  ximage->data = (char *)pixmap;
  XPutImage(xwin->dsp, xwin->win, xwin->gc, ximage, 0, 0, 0, 0, width, height);
  XFlush(xwin->dsp);
}

void event_loop(unsigned char *pixmap, unsigned char *pixmapmod, int width, int height, int depth, struct XWin **xwin)
{
  int loop = 1;
  float gamma = 1;
  struct timeval tv;

  int x11_fd = ConnectionNumber((*xwin)->dsp);
  fd_set in_fds;

  XImage *ximage = XCreateImage((*xwin)->dsp, (*xwin)->visual, 24, ZPixmap, 0, 
                                (char *)pixmap, (unsigned int)width, 
                                (unsigned int)height, 32, 0);

  srand(time(0));
  update_screen(ximage, pixmap, width, height, depth, (*xwin));

  while(loop) {
    FD_ZERO(&in_fds);
    FD_SET(x11_fd, &in_fds);
    tv.tv_usec = 33333;
    tv.tv_sec = 0;
    if(select(x11_fd + 1, &in_fds, 0, 0, &tv)) {
      switch((*xwin)->evt->type) {
        case(KeyRelease):
          if((*xwin)->evt->xkey.keycode == (*xwin)->keys->Q) {
            loop = 0; break;
          } else if((*xwin)->evt->xkey.keycode == (*xwin)->keys->W) {
            if(gamma < 10.0) {
              gamma += 0.1;
              timing(apply_gamma(pixmap, pixmapmod, width, height, depth, gamma));
            }
          } else if((*xwin)->evt->xkey.keycode == (*xwin)->keys->S) {
            if(gamma > 0.0) {
              gamma -= 0.1;
              timing(apply_gamma(pixmap, pixmapmod, width, height, depth, gamma));
            }
          }
          break;
        case(ConfigureNotify):
          if((*xwin)->evt->xconfigure.width != (*xwin)->xres || (*xwin)->evt->xconfigure.height != (*xwin)->yres) {
            (*xwin)->xres = (*xwin)->evt->xconfigure.width;
            (*xwin)->yres = (*xwin)->evt->xconfigure.height;
          }
          break;
        case(ClientMessage):
          if((*xwin)->evt->xclient.data.l[0] == (*xwin)->wmdelete) loop = 0;
          break;
        default:
          break;
      }
    } else {
      update_screen(ximage, pixmapmod, width, height, depth, (*xwin));
    }
    while(XPending((*xwin)->dsp)) {
      XNextEvent((*xwin)->dsp, (*xwin)->evt);
    }
  } 
}

void xwindow_init(unsigned char *pixmap, unsigned char *pixmapmod, int width, int height, int depth, struct XWin **xwin)
{
  unsigned int black;
  int screen;

  // Allocate some space for the xvars
  *xwin = calloc(sizeof(struct XWin), 1);
  (*xwin)->keys = malloc(sizeof(struct Keys));
  (*xwin)->evt = malloc(sizeof(XEvent));

  // Initialize the display
  (*xwin)->dsp = XOpenDisplay(NULL);
  if( !(*xwin)->dsp ){
    fprintf(stderr, "Couldn't get display. Abort.\n");
    exit(1);
  }
  
  // Get the screen
  screen = DefaultScreen((*xwin)->dsp);
  
  // Set the width and height of the window
  if(DisplayWidth((*xwin)->dsp, screen) >= width) {
    (*xwin)->xres = width;
  } else {
    (*xwin)->xres = DisplayWidth((*xwin)->dsp, screen);
  }
  if(DisplayHeight((*xwin)->dsp, screen) >= height) {
    (*xwin)->yres = height;
  } else {
    (*xwin)->yres = DisplayHeight((*xwin)->dsp, screen);
  }

  black = BlackPixel((*xwin)->dsp, screen);

  (*xwin)->win = XCreateSimpleWindow((*xwin)->dsp,
                            DefaultRootWindow((*xwin)->dsp),
                            0, 0,   // origin
                            (*xwin)->xres, (*xwin)->yres, // size
                            0, black, // border width/clr
                            black);   // backgrd clr

  
  // Window Manager setup
  (*xwin)->wmdelete = XInternAtom((*xwin)->dsp, "WM_DELETE_WINDOW", True);
  XSetWMProtocols((*xwin)->dsp, (*xwin)->win, &((*xwin)->wmdelete), 1);

  (*xwin)->gc = XCreateGC((*xwin)->dsp, (*xwin)->win, 0, NULL);

  XSetForeground((*xwin)->dsp, (*xwin)->gc, black);

  long eventMask = StructureNotifyMask;
  eventMask |= ButtonPressMask|ButtonReleaseMask|KeyPressMask|KeyReleaseMask;
  XSelectInput((*xwin)->dsp, (*xwin)->win, eventMask);

  (*xwin)->keys->Q = XKeysymToKeycode((*xwin)->dsp, XStringToKeysym("Q"));
  (*xwin)->keys->W= XKeysymToKeycode((*xwin)->dsp, XStringToKeysym("W"));
  (*xwin)->keys->S = XKeysymToKeycode((*xwin)->dsp, XStringToKeysym("S"));

  XMapWindow((*xwin)->dsp, (*xwin)->win);

  // wait until window appears
  do { XNextEvent((*xwin)->dsp, (*xwin)->evt); } while ((*xwin)->evt->type != MapNotify);
}

void xwindow_del(struct XWin *xwin)
{
  XDestroyWindow(xwin->dsp, xwin->win);
  XCloseDisplay(xwin->dsp);
}
