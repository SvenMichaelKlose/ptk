// $Id: tkScrollBar.cc,v 1.2 2001/10/11 03:26:18 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptktoolkit/tkScrollBar.cc,v $
//--------------------------------------------------------------------------
// Scrollbar style.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#include "frame.h"
#include "tkPanel.h"
#include "tkScrollBar.h"
#include "console.h"

tkScrollBar::tkScrollBar (unsigned sizex, unsigned sizey,
			  int x, int y, int w, int h)
  : tkScrollBarBase (sizex, sizey, x, y, w, h)
{
  //  (tkScrollBar)this ->   (panel) _fg
  //                       (lmEqual) tmp -> (panel)_bg
  frame* tmp;
  tkLoweredPanel* lp;
  link (_fg = new tkRaisedPanel);
  (tmp = new lmEqual)->link (_bg = lp=new tkLoweredPanel);
  lp->setColor (console::instance ()->sysColor (1));
  _fg->add (tmp);
  _fg->clipThis (true);
  tkScrollBarBase::defineStyle (_bg, _fg);
}

void tkScrollBar::onEnter ()
{
  _fg->setColor (console::instance ()->sysColor (4));
  _fg->repaint ();
}

void tkScrollBar::onLeave ()
{
  _fg->setColor(console::instance ()->sysColor (2));
  _fg->repaint ();
}

#ifdef DEBUG
void tkScrollBar::dump (ofstream& o)
{
  o << "::tkScrollBar"; frame::dump (o);
}
#endif
