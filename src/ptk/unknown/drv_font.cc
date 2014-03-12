// $Id: drv_font.cc,v 1.1.1.1 2000/09/30 13:42:10 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/unknown/drv_font.cc,v $
//--------------------------------------------------------------------------
// font.cc -- Font object.
//
// (c) 1999 Sven Klose
//--------------------------------------------------------------------------

//#include <iostream.h>
#include <vector>

#include "font.h"
#include "ptk.h"

#include "drv_display.h"

class dbrec {
public:
  XFontStruct *info;
  char *name;
  dbrec(XFontStruct *i, char *n)
    : info(i),name(n) {}
};

static vector<dbrec*>* db=0;

int font::widthOf(char) { return maxWidth(); }

void font::set_metrics()
{
  _ascent=_fontinfo->ascent;
  _descent=_fontinfo->descent;
  _spacing=_fontinfo->ascent - _fontinfo->max_bounds.ascent;
  _maxwidth=_fontinfo->max_bounds.width;
  _loaded=true;
}

font::font(char* name) : _loaded(false)
{
  if (!db) db=new vector<dbrec*>; // Create font list

  // Check if the font is already loaded and use it.
  for (vector<dbrec*>::iterator i=db->begin();i!=db->end();i++) {
    if (!strcmp(name,(*i)->name)) {
      _fontinfo=(*i)->info;
      _fontname=(*i)->name;
      set_metrics();
      return;
    }
  }

  // Load font
  Display* disp=x11_display();
  int screen_num=x11_screen();
  if (!(_fontinfo=XLoadQueryFont(disp,name))) {
    //      cerr << "Can't load font. Trying default.\n";
    _fontinfo=XQueryFont(disp,
			 XGContextFromGC(DefaultGC(disp,screen_num))
			);
      
  } else
    db->push_back(new dbrec(_fontinfo,name));
  _fontname=name; set_metrics();
}

#ifdef EMBED_ANYX11
XFontStruct* font::fontInfo()
{
  return _fontinfo;
}
#endif

// $Log: drv_font.cc,v $
// Revision 1.1.1.1  2000/09/30 13:42:10  sven
// ptk is a lightweight cross-platform user interface written in C++.
//
// Revision 1.1.1.1  2000/02/06 01:23:29  sven
// Let's go!
//
// Revision 1.1.1.1  2000/02/04 21:39:17  sven
// Let's go!
//
