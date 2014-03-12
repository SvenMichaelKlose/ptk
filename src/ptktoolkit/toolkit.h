// $Id: toolkit.h,v 1.2 2001/02/03 07:12:31 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptktoolkit/toolkit.h,v $
//--------------------------------------------------------------------------
// This is of no use AFAIK. Will just get rid of it. 
//
// (c) 1999 Sven Klose
//--------------------------------------------------------------------------

#ifndef _ptk_toolkit_H
#define _ptk_toolkit_H

class toolkit : public frame {
 public:
  toolkit() : frame() { clipAble(false); }
  toolkit(dim* a) : frame(a) {}
  toolkit(int x, int y, int w, int h) : frame(x,y,w,h) {}
  virtual int receiver(msgbase*);
  virtual void onClick() {}
  virtual void onEnter() {}
  virtual void onLeave() {}
};

#endif
