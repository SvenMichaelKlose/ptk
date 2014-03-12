// $Id: plane.cc,v 1.1 2001/05/05 00:00:58 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/plane.cc,v $
//--------------------------------------------------------------------------
// Drawing filled rectangles.
//
// (c) 1997-1999 Sven Klose
//--------------------------------------------------------------------------

#include "drawer.h"
#include "frame.h"
#include "plane.h"

//////////////////////////////
// plane - filled rectangle //
/////////////////////////////
plane::plane (color* c,int x, int y, int w, int h)
  : frame (x, y, w, h), _color (c)
{
  clipThis (true);
}

plane::plane (dim* r, color* c)
{
  plane (c, r->x (), r->y (), r->width (), r->height ());
}

void plane::draw (dim* i, int, int, drawer* dr)
{
  if (i)
    do {
      dr->rect (i, _color);
    } while((i = i->next ()));
}

#ifdef DEBUG
void plane::dump (ofstream& o)
{
  o << "::plane";
  frame::dump (o);
  o << " " << _color;;
}
#endif
