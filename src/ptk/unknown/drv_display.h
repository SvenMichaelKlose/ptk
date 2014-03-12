#ifndef _ptk_driver_display_H
#define _ptk_driver_display_H

#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <X11/Xos.h>
//#include <X11/Xatom.h>

#include "config.h"

class drv_display {
friend class color;
friend class drawer;
friend class embed;
friend class font;
friend class image;
friend class window_drawer;

 static drv_display* _instance;

 int xm;
 int ym;
 unsigned int depth;
 int _colors;

#ifdef EMBED_X11
 inline char* getAdr(int,int);
#endif

#ifdef EMBED_X11
 char* _charset8x8;
#endif

 public:
 Display* _display;
 int _screen_num;
 drv_display();
 virtual ~drv_display();
 static drv_display* instance();
 virtual void bell();
 virtual void bell(int ms);
 virtual void flush();
};

int x11_screen();
Display* x11_display();

#endif
