// $Id: tkScrollFieldBase.h,v 1.2 2001/05/04 23:56:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptktoolkit/tkScrollFieldBase.h,v $
//--------------------------------------------------------------------------
// Abstract scrollfield.:
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#ifndef _ptk_tkScrollFieldBase_H
#define _ptk_tkScrollFieldBase_H

#include "layout.h"
#include "tkScrollBarBase.h"

class tkScrollFieldBase : public lmTable
{
  tkScrollBarBase* _barx;
  tkScrollBarBase* _bary;
  frame* _view;
  frame* _former_view;

 private:
  void _setView (int x, int y);
  void _update (frame*);

 public:
  tkScrollFieldBase (tkScrollBarBase* barx,
		     tkScrollBarBase* bary,
		     int x=0, int y=0, int w=0, int h=0);

  virtual void update ();
  virtual frame* link (frame*);
  virtual int receiver (msgbase*);

  tkScrollBarBase* bar_x() { return _barx; }
  tkScrollBarBase* bar_y() { return _bary; }
  void set_bar_x(tkScrollBarBase* bar) { _barx=bar; }
  void set_bar_y(tkScrollBarBase* bar) { _bary=bar; }
};

#endif
