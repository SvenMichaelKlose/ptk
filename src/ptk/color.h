// $Id: color.h,v 1.2 2001/02/03 07:12:31 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/color.h,v $
//--------------------------------------------------------------------------
// Color management
//
// (c)1998-2000 Sven Klose
//--------------------------------------------------------------------------
// Allocate a color object for each color you need.
// X11 support only.

#ifndef _ptk_color_H
#define _ptk_color_H

class color {
  void* _this;
  unsigned long _pixel;
  int _r;
  int _g;
  int _b;
 public:

  color (int r, int g, int b);

  inline void set (int r, int g, int b)
  {
    _r = r;
    _b = b;
    _g = g;
  }

  inline int r () {return _r;}
  inline int g () {return _g;}
  inline int b () {return _b;}

  inline unsigned long pixel () { return _pixel; }
};
#endif
