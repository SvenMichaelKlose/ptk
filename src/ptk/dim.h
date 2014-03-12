// $Id: dim.h,v 1.2 2001/02/03 07:12:31 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/dim.h,v $
//--------------------------------------------------------------------------
// Handles rectangular areas in a list tree.
//
// (c) 1997-1999 Sven Klose
//--------------------------------------------------------------------------
// too stoned to explain right now

#ifndef _ptk_dim_H
#define _ptk_dim_H

#include"tree.h"

class dim : public tree {
 private:
   dim* clipBase (dim*);
   dim* insideBase (dim*);
   dim* split (int edge, dim* c); 
   dim* split0 (dim*); // Clipping for each edge
   dim* split1 (dim*);
   dim* split2 (dim*);
   dim* split3 (dim*);

 public:
   enum metric {
     pixel,
     perc   = 0x8000 // Take value as percentage of calculated size
   };

   // Size attributes
   enum size {
     min    = 0x1000, // mininum size
     max    = 0x2000, // maximum size
     equal  = 0x3000, // equal to any other dim (defined by layout classes)
     preset = 0x4000  // don't change the current size
   };

   // Positioning
   enum gravity {
     center = 0x1000,
     top    = 0x2000,
     bottom = 0x3000,
     left   = 0x2000,
     right  = 0x3000
   };

   int pt[4]; // *** must be declared private!
   int flag (int a) {return a&0xf000;}
   int  val (int a) {return a&0x0fff;}

   dim () : tree () {}
   	dim (dim* d) : tree()
	{
		for (int i = 0; i < 4; i++)
			pt[i] = d->pt[i];
	}
   dim (int x, int y, int w, int h) : tree () {
     pt[0] = x; pt[1] = y; pt[2] = w + x; pt[3] = h + y;
   }

   // Methods of 'tree'
   virtual dim* last () {return (dim*) _last;}
   virtual dim* next () {return (dim*) _next;}
   virtual dim* child () {return (dim*) _child;}
   virtual dim* parent () {return (dim*) _parent;}
   virtual dim* link (dim* d) {return (dim*) tree::link (d);}

   // Getting coordinates
   inline int x () {return pt[0];}
   inline int y () {return pt[1];}
   inline int width () {return pt[2] - pt[0];}
   inline int height () {return pt[3] - pt[1];}
   int p (int t) {return pt[t&3];}
   dim* totalPosition ();

   // Setting coordinates
   virtual void move (int new_x, int new_y);
   virtual void resize (int new_width, int new_height);
   virtual void moveresize (int new_x, int new_y, int new_width, int new_height);
   void setdim (dim*);
   void setp (int t, int v) {pt[t & 3] = v;}

   // Arithmetics with rectangles
   dim* hole (); // Returns areas laying outside of this.
   bool hits (dim*); // Returns 'true' if d overlaps this
   bool isWithin (dim*); // Returns true if this lies within dim*
   dim*  clip (dim* d); // Returns this areas that lay outside 'd' 
   dim*  inside (dim*);
   dim*  duplicate (); // Returns a copy of this and its next objects in the list
   dim*  cleanup (dim* n); // Delete frames of no size and invalid frames
   dim*  xysort (dim* list, int xdir, int ydir);

#ifdef DEBUG
   void dump (ofstream&);
#endif
};
#endif
