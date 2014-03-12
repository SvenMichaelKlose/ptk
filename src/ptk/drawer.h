// $Id: drawer.h,v 1.1.1.1 2000/09/30 13:42:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/drawer.h,v $
//--------------------------------------------------------------------------
// Display interface
//
// (c)1999-2000 Sven Klose
//--------------------------------------------------------------------------
// Any frame can own a drawer which does real painting.
// With X11 a drawer is used for each window; on embedded systems the console
// would own the only drawer - except we'd do some buffering.
 
#ifndef _ptk_drawer_H
#define _ptk_drawer_H

#include "color.h"
#include "dim.h"
#include "font.h"
#include "image.h"

class drawer {
friend class embed;
  int _width;
  int _height;

 public:
  drawer(char*, int xp, int yp, int w, int h) {_width=w; _height=h;}
  virtual ~drawer() {}
  virtual void rect(dim*,color*)= 0;
  virtual void bmp(dim*,int x,int y,int ow,int xm,image*)=0;
  virtual void putchar(dim*,int,int,char,color*,color*,font*)=0;
  virtual void putstring(dim*,int x,int y,char*,color*,color*,font*)=0;
  virtual void copy(int xs,int ys,int yd,int xd,int w,int h)=0;

#ifdef EMBED_NONE
  virtual void draw_pointer(int x, int y, unsigned char* pointer) {}
  virtual void save_pointer(int x, int y, unsigned char* buffer) {}
  virtual void restore_pointer(int x, int y, unsigned char* buffer) {}
#endif

  virtual void draw(dim* d)=0;
  int width() { return _width; }
  int height() { return _height; }
  virtual int x()=0;
  virtual int y()=0;
  virtual void move(int x, int y)=0;
  virtual void resize(int w, int h)=0;
#ifndef EMBED_NONE
  virtual void show() = 0;
  virtual void hide() = 0;
#endif
};
#endif
