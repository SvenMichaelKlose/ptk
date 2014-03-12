// $Id: tkScrollBarBase.h,v 1.2 2001/05/04 23:56:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptktoolkit/tkScrollBarBase.h,v $
//--------------------------------------------------------------------------
// Abstract scrollbar.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#ifndef _ptk_tkScrollBarBase_H
#define _ptk_tkScrollBarBase_H

#include "frame.h"
#include "layout.h"

class tkScrollBarBase : public frame
{
 private:
  unsigned _thumbsizex; // Size of thumb (%*100)
  unsigned _thumbsizey;
  bool _dragged;
  bool _clicked;
  int _posx;
  int _posy;
  frame* _bgr;
  frame* _thumb;

  void _setThumb();
  void _setThumbPos(int,int);
  int _thumbWidth();
  int _thumbHeight();

 protected:
  void defineStyle(frame* newbgr,frame* newbar);
  virtual void onEnter() = 0;
  virtual void onLeave() = 0;

 public:
  tkScrollBarBase() {};
  tkScrollBarBase(unsigned sizex,unsigned sizey,
		  int x=0, int y=0, int w=0, int h=0);
  //virtual ~tkScrollBarBase() {}

  virtual void update();
  virtual int receiver(msgbase* m);

  virtual int minx() { return 8; }
  virtual int miny() { return 8; }

  void setSize(unsigned sizex,unsigned sizey);
  int getx();
  int gety();
};

#endif
