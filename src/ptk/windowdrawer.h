// $Id: windowdrawer.h,v 1.2 2001/02/03 05:18:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/windowdrawer.h,v $
//--------------------------------------------------------------------------
// X11 window drawer.
//
// (c)1998-2000 Sven Klose
//--------------------------------------------------------------------------

#ifndef ptk_window_drawer_H
#define ptk_window_drawer_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include "color.h"
#include "drawer.h"

class window_drawer : public drawer {
friend class drv_embed;
  XWMHints *wm_hints;
  XSizeHints *size_hints;
  XClassHint *class_hints;
  XSetWindowAttributes* setattr;
  XTextProperty windowName;
  XTextProperty iconName;
  XWindowAttributes windowattr;
  XFontStruct *font_info;
  XGCValues values;
  unsigned long valuemask;
  int border_width;
  int screen_num;
  char* window_name;
  char* icon_name;
  GC _gc;
  Window _win;
#ifdef EMBED_X11
  XImage img;
#endif

 public:
  window_drawer(char*, int xp, int yp, int w, int h,
		bool popup, color* bgcolor=0);
  virtual ~window_drawer();

  virtual void rect(dim*,color*);
  virtual void bmp(dim*,int x,int y,int ow,int xm,image*);
  virtual void putchar(dim*,int,int,char,color*,color*,font*);
  virtual void putstring(dim*,int x,int y,char*,color*,color*,font*);
  virtual void copy(int xs,int ys,int yd,int xd,int w,int h);

// This does not belong here.
#ifdef EMBED_NONE
  virtual void draw_pointer(int x, int y, unsigned char* pointer) {}
  virtual void save_pointer(int x, int y, unsigned char* buffer) {}
  virtual void restore_pointer(int x, int y, unsigned char* buffer) {}
#endif

  virtual void draw(dim* d);
  virtual int x();
  virtual int y();
  virtual void move(int x, int y);
  virtual void resize(int w,int h);
  virtual void show();
  virtual void hide();
};
#endif
