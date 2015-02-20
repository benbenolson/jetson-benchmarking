//----- x3.c simple xwindows example -----
// compile with gcc -lX11 -o x3 x3.c

#include <X11/Xlib.h> // must precede most other headers!
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

void update_screen();

int XRES;
int YRES;
int NPTS=10000;

Display *dsp;
Window win;
int screen;
GC gc;
unsigned int white;
unsigned int black;

int main()
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


  Atom wmDelete = XInternAtom(dsp, "WM_DELETE_WINDOW", True);
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

  //srand(time(0)); // only 1 sec resolution so use once per run
  update_screen();

  int loop = 1;
  while(loop) {
    XNextEvent(dsp, &evt);
    switch (evt.type) {
      case(ButtonRelease):
        update_screen();
        break;
      case(ConfigureNotify):
        if(evt.xconfigure.width != XRES || evt.xconfigure.height != YRES) {
          XRES = evt.xconfigure.width;
          YRES = evt.xconfigure.height;
          update_screen();
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

void update_screen()
{
  XClearWindow(dsp, win);

  // draw colour palette
  int x, y;
  int r=0, g=0, b=255;

  for (; g<256; g++) {
    for (x=0; x<256; x++) {
      XSetForeground(dsp,gc, (r<<16)|(g<<8)|b);
      XDrawPoint(dsp,win,gc,x,g);
      r = (r+1)%256;
      b = (b+255)%256; 
    }
  }

  XFlush(dsp);
}
