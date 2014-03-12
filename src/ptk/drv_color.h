// $Id: drv_color.h,v 1.1.1.1 2000/09/30 13:42:10 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/x11/drv_color.h,v $
//--------------------------------------------------------------------------
// Color management
//
// (c)1998-2000 Sven Klose
//--------------------------------------------------------------------------
// Allocate a color object for each color you need.
// X11 support only.

#ifndef _ptk_driver_color_H
#define _ptk_driver_color_H

class drv_color {
  unsigned long _pixel;
  int _r;
  int _g;
  int _b;
 public:

  drv_color(int r,int g,int b);

  inline void set(int r,int g,int b) { _r=r;_b=b;_g=g; }

  inline int r() {return _r;}
  inline int g() {return _g;}
  inline int b() {return _b;}

  inline unsigned long pixel() { return _pixel; }
};
#endif
