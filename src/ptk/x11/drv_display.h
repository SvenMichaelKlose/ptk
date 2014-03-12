// $Id: drv_display.h,v 1.1.1.1 2000/09/30 13:42:10 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/x11/drv_display.h,v $
//--------------------------------------------------------------------------
// X11 display driver
//
// (c)2000 Sven Klose
//--------------------------------------------------------------------------

#ifndef _ptk_driver_display_H
#define _ptk_driver_display_H

#include <X11/Xlib.h>

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
