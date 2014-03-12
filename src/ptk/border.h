// $Id: border.h,v 1.1 2001/05/05 00:00:58 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/border.h,v $
//--------------------------------------------------------------------------
// Drawing frames.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#ifndef _ptk_border_H
#define _ptk_border_H

#include "color.h"
#include "frame.h"

class border : public frame {

 protected:
  color* _ct;
  color* _cr;
  color* _cb;
  color* _cl;

 public:
  border(color* ct=0, color* cr=0, color* cb=0, color* cl=0,
	 int x=0,int y=0,int w=0,int h=0);
  virtual ~border() {}

  virtual void resize(int width,int height);
  void setColors(color* ct, color* cr, color* cb, color* cl);
  virtual void draw (dim*,int,int,drawer*);

#ifdef DEBUG
  virtual void dump(ofstream&);
#endif
};

#endif
