// $Id: plane.h,v 1.1 2001/05/05 00:00:58 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/plane.h,v $
//--------------------------------------------------------------------------
// Drawing frames.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#ifndef _ptk_plane_H
#define _ptk_plane_H

#include "color.h"

class plane : public frame {
   color* _color;

 public:
   plane () : frame() {}
   plane (color* c, int x=0, int y=0, int w=0, int h=0);
   plane (dim*, color*);
   virtual ~plane() {}

   virtual void draw (dim*, int, int, drawer*);
   inline void setColor(color* c) {_color=c;}

#ifdef DEBUG
   virtual void dump(ofstream&);
#endif
};

#endif
