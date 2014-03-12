// $Id: tkScrollBar.h,v 1.2 2001/05/04 23:56:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptktoolkit/tkScrollBar.h,v $
//--------------------------------------------------------------------------
// Scrollbar style.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#ifndef _ptk_tkScrollBar_H
#define _ptk_tkScrollBar_H

#include "tkScrollBarBase.h"

class tkPanel;

class tkScrollBar : public tkScrollBarBase {
  tkPanel* _bg;
  tkPanel* _fg;
 protected:
  virtual void onEnter ();
  virtual void onLeave ();

 public:
  tkScrollBar (unsigned sizex = 0, unsigned sizey = 0,
	       int x = 0, int y = 0, int w = 0, int h = 0);
  virtual ~tkScrollBar () {}

  virtual int minx () { return 16; }
  virtual int miny () { return 16; }

#ifdef DEBUG
  virtual void dump (ofstream&);
#endif
};
#endif
