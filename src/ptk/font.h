// $Id: font.h,v 1.1.1.1 2000/09/30 13:42:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/font.h,v $
//--------------------------------------------------------------------------
// Font object.
//
// (c) 1999 Sven Klose
//--------------------------------------------------------------------------
// X11 support only

#ifndef _ptk_font_H
#define _ptk_font_H

#include "config.h"

class font {
  void* _this; // drv_font object
 public:
  font(char* fontname);
  char * name();
  bool isLoaded();
  int ascent();
  int descent();
  int spacing();
  int height();
  int maxWidth();
  int widthOf(char);
  void* drv() { return _this; }
};
#endif
