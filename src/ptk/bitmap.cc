// $Id: bitmap.cc,v 1.1 2001/05/05 00:00:58 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/bitmap.cc,v $
//--------------------------------------------------------------------------
// Drawing bitmaps.
//
// (c) 1997-1999 Sven Klose
//--------------------------------------------------------------------------
// This objects draw something on the screen.
// Every object *must* know about its minimum size when fully initialised
// because auto-layout relies on this information.

#include "drawer.h"
#include "frame.h"
#include "bitmap.h"
#include "ptk.h"

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
