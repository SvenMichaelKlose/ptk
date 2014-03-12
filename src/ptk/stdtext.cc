// $Id: stdtext.cc,v 1.1 2001/05/05 00:00:58 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/stdtext.cc,v $
//--------------------------------------------------------------------------
// Drawing standard text.
//
// (c) 1997-1999 Sven Klose
//--------------------------------------------------------------------------

#include "drawer.h"
#include "frame.h"
#include "stdtext.h"
#include "font.h"

static font* fnt;

font* stdtext::stdfont ()
{
  if (!fnt)
    fnt = new font ("6x13");
  return fnt;
}

stdtext::stdtext (char* text, color* f, color* b, int x, int y)
  : frame (x, y, (int) strlen (text) * stdfont ()->maxWidth () - 1, stdfont ()->height ()),
    _foreground (f), _background (b), _textstring (text)
{}

int stdtext::width () { return minx (); }
int stdtext::height () { return miny (); }

int stdtext::minx ()
{
#ifdef EMBED_X11
  return (strlen (_textstring)) * 8;
#endif
#ifdef EMBED_X11WRAP
  return (strlen (_textstring)) * stdfont ()->maxWidth ();
#endif
}

int stdtext::miny ()
{
#ifdef EMBED_X11
  return 8;
#endif
#ifdef EMBED_X11WRAP
  return stdfont ()->height ();
#endif
}

void stdtext::draw (dim* m, int xp, int yp, drawer* dr)
{
  font* f = stdfont ();
  dr->putstring (m, xp + x (), yp + y () + f->ascent (),
		_textstring, _foreground, _background, f);
}

#ifdef DEBUG
void stdtext::dump (ofstream& o)
{
  o << "::stdtext";
  frame::dump (o);
  o << " " << _foreground
    << " " << _background
    << " \"" << _textstring << "\"";
}
#endif
