// $Id: drv_font.h,v 1.1.1.1 2000/09/30 13:42:10 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/x11/drv_font.h,v $
//--------------------------------------------------------------------------
// X11 font driver.
//
// (c) 1999 Sven Klose
//--------------------------------------------------------------------------
// X11 support only

#ifndef _ptk_driver_font_H
#define _ptk_driver_font_H

#include "config.h"
#ifdef EMBED_ANYX11
#include <X11/Xlib.h>
#endif

class drv_font {
  int _ascent;
  int _descent;
  int _spacing;
  int _maxwidth;
  bool _loaded;
  XFontStruct* _fontinfo;
  char* _fontname;
  void set_metrics();
  friend class window_drawer;
#ifdef EMBED_ANYX11
  XFontStruct* fontInfo();
#endif

 public:
  drv_font(char* fontname);
  char * name() { return _fontname; }
  bool isLoaded() {return _loaded;}
  int ascent() {return _ascent;}
  int descent() {return _descent;}
  int spacing() {return _spacing;}
  int height() {return _ascent+_descent+_spacing;}
  int maxWidth() {return _maxwidth;}
  int widthOf(char);
  //  int setHeight(int height);  // Will scale font closest size available and
  //  int setMaxWidth(int width); // return the new values.
};

inline drv_font* x11_font(void* f) { return (drv_font*) f; }
#endif
