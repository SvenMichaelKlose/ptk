// $Id: font.cc,v 1.1.1.1 2000/09/30 13:42:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/font.cc,v $
//--------------------------------------------------------------------------
// Font object.
//
// (c) 1999 Sven Klose
//--------------------------------------------------------------------------

//#include <iostream.h>

#include "ptk.h"
#include "font.h"
#include "drv_display.h"
#include "drv_font.h"

font::font(char* name) {_this=(void*) new drv_font(name);}

char * font::name() { return x11_font(_this)->name(); }
bool font::isLoaded() {return x11_font(_this)->isLoaded();}
int font::ascent() {return x11_font(_this)->ascent();}
int font::descent() {return x11_font(_this)->descent();}
int font::spacing() {return x11_font(_this)->spacing();}
int font::height() {return x11_font(_this)->height();}
int font::maxWidth() {return x11_font(_this)->maxWidth();}
int font::widthOf(char c) {return x11_font(_this)->widthOf(c);}
