// $Id: border.cc,v 1.1 2001/05/05 00:00:58 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/border.cc,v $
//--------------------------------------------------------------------------
// Drawing frames.
//
// (c) 1997-1999 Sven Klose
//--------------------------------------------------------------------------
// This objects draw something on the screen.
// Every object *must* know about its minimum size when fully initialised
// because auto-layout relies on this information.

#include "border.h"
#include "drawer.h"

////////////
// border //
///////////
border::border (color* ct, color* cr, color* cb, color* cl,
	        	int x, int y, int w, int h) 
  : frame (x, y, w, h), _ct (ct), _cr (cr), _cb (cb), _cl (cl)
{
}

void border::setColors (color* ct, color* cr, color* cb, color* cl)
{
  _ct = ct;
  _cr = cr;
  _cb = cb;
  _cl = cl;
}

void border::resize (int w, int h)
{
  dim::resize (w, h);
}

void subdraw (dim* p, dim* v, color* c, drawer* dr)
{
  dim* d;
  dim* i;
  if ((d = p->inside (v))) {
    i = d;
    while (i) {
      dr->rect (i, c);
      i = i->next ();
    }
    d->clearAll ();
  }
}

void border::draw (dim *v, int xo, int yo, drawer* dr)
{
  // Create 4 rectangles, clip and draw them
  int x = this->x () + xo;
  int y = this->y () + yo;
  int w = width();
  int h = height();

  dim p (x, y, w - 1, 1);
  subdraw (&p, v, _ct, dr); // top
  p.resize (1, h);
  subdraw (&p, v, _cl, dr); // left
  p.move (x + w - 1, y);
  p.resize (1, h - 1);
  subdraw (&p, v, _cr, dr); // right
  p.move (x + 1, y + h - 1);
  p.resize (w - 1, 1);
  subdraw (&p, v, _cb, dr); // bottom
}

#ifdef DEBUG
void border::dump (ofstream& o)
{
  o << "::border";
  frame::dump (o);
  o << " " << _ct
    << " " << _cr
    << " " << _cb
    << " " << _cl;
}
#endif // #ifdef DEBUG
