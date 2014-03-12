// $Id: pencils.cc,v 1.2 2001/02/03 07:12:31 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/pencils.cc,v $
//--------------------------------------------------------------------------
// Drawing frames.
//
// (c) 1997-1999 Sven Klose
//--------------------------------------------------------------------------
// This objects draw something on the screen.
// Every object *must* know about its minimum size when fully initialised
// because auto-layout relies on this information.

#include "frame.h"
#include "pencils.h"
#include "ptk.h"

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

////////////
// border //
///////////
border::border (color* ct = 0, color* cr = 0, color* cb = 0, color* cl = 0,
	        int x = 0, int y = 0, int w = 0, int h = 0) 
  : frame (x, y, w, h), _ct (ct), _cr (cr), _cb (cb), _cl (cl)
{
  clipThis (true);
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

void border::subdraw (dim* p, dim* v, color* c, drawer* dr)
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
#endif DEBUG

/////////////
// stdtext //
////////////
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

////////////
// bitmap //
///////////
void bitmap::defaults (int w, int h)
{
  _image = new image (w, h);
  _minWidth = w;
  _minHeight = h;
  resizeable (false);
  clipChilds (true);
  clipThis (true);
}

bitmap::bitmap (int x, int y, int w, int h)
  : frame (x, y, w, h)
{
  defaults (w, h);
}

bitmap::bitmap (char* filename, int x, int y, int, int) : frame (x, y, 0, 0)
{
#ifndef PTK_NO_FSTREAM_H
  // read TGA
  unsigned char hdr [0x3c];
  ifstream img (filename, ios::bin | ios::in);
  img.read (hdr, 0x3c);
  int w = (hdr[13]<<8) + hdr[12];
  int h = (hdr[15]<<8) + hdr[14];
  dim::resize (w, h);
  defaults (w, h);
  img.read (_image->surface (), w * h * 3);
  img.close ();
#endif
}

bitmap::bitmap (char* filename, dim* r)
{
  bitmap (filename, r->x (), r->y (), r->width (), r->height ());
}

bitmap::~bitmap ()
{
  delete _image;
}

void bitmap::draw (dim* i, int xt, int yt, drawer* dr)
{
  if (i)
    do {
      dr->bmp (i,xt + x (), yt + y (), width (), dr->width (), _image);
    } while((i = i->next ()));
}

void bitmap::resize (int w, int h)
{
  dim::resize (w, h);
}
