// $Id: drv_font.h,v 1.1.1.1 2000/09/30 13:42:10 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/unknown/drv_font.h,v $
//--------------------------------------------------------------------------
// font.h -- Font object.
//
// (c) 1999 Sven Klose
//--------------------------------------------------------------------------
// X11 support only

#ifndef _font_H
#define _font_H

#include "config.h"
#ifdef EMBED_ANYX11
#include <X11/Xlib.h>
#endif

class font {
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
  font(char* fontname);
  char * name() { return _fontname; }
  bool isLoaded() {return _loaded;}
  int ascent() {return _ascent;}
  int descent() {return _descent;}
  int spacing() {return _spacing;}
  int height() {return _ascent+_descent+_spacing;}
  int maxWidth() {return _maxwidth;}
  int widthOf(char);
  int setHeight(int height);  // Will scale font closest size available and
  int setMaxWidth(int width); // return the new values.
};
#endif
