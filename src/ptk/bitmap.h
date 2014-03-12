// $Id: bitmap.h,v 1.1 2001/05/05 00:00:58 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/bitmap.h,v $
//--------------------------------------------------------------------------
// Drawing bitmaps.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#ifndef _ptk_bitmap_H
#define _ptk_bitmap_H

#include "image.h"

class bitmap : public frame {
   image* _image;
   int _minWidth;
   int _minHeight;
   void defaults (int, int);

 public:
   bitmap (char*, int x = 0, int y = 0, int w = 0, int h = 0);
   bitmap (int x = 0, int y = 0, int w = 0, int h = 0);
   bitmap (char*, dim*);
   virtual ~bitmap();

   virtual void draw (dim*, int, int, drawer*);
   virtual int minx () {return _minWidth;}
   virtual int miny () {return _minHeight;}
   virtual void resize (int w, int h);
   inline image* get () { return _image; }
};

#endif
