// $Id: tkPanel.cc,v 1.4 2001/05/04 23:56:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptktoolkit/tkPanel.cc,v $
//--------------------------------------------------------------------------
// Panels. (style)
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#include "frame.h"
#include "plane.h"
#include "tkPanel.h"
#include "console.h"

tkPanel::tkPanel (color* cp, color* ct,
	     	  color* cr, color* cb, color* cl,
	     	  int x, int y, int w, int h)
  : border (ct, cr, cb, cl, x, y, w, h), _cp (cp)
{
  lmHint _hint (center, center, (lmHint::size) (max | 2), (lmHint::size) (max | 2));
  frame::link (new lmEqual (&_hint))
    -> link (_plane = new plane (cp))
    -> link (_connection = new lmEqual (&_hint));
}

void tkPanel::setColors (color* cp, color* ct, color* cr, color* cb, color* cl)
{
  border::setColors (ct, cr, cb, cl);
  _plane->setColor (cp);
}

frame* tkPanel::link(frame* f)
{
  return _connection->link (f);
}

void tkPanel::setColor (color* c)
{
  _plane->setColor (c);
}

#ifdef DEBUG
void tkPanel::dump (ofstream& o)
{
  o << "::tkPanel";
  border::dump (o);
  o << " " << _cp << " " << _ct << " " << _cr << " " << _cb << " " << _cl << "\n";;
}

void tkPanel::dumplist(ofstream& o)
{
  dump(o); if (_plane->child()) _plane->child()->dumplist(o);
  if (next()) next()->dumplist(o);
}
#endif

tkLoweredPanel::tkLoweredPanel()
  : tkPanel(console::instance()->sysColor(2),
	  console::instance()->sysColor(1),
	  console::instance()->sysColor(3),
	  console::instance()->sysColor(3),
	  console::instance()->sysColor(1))
{}

tkRaisedPanel::tkRaisedPanel()
  : tkPanel(console::instance()->sysColor(2),
	  console::instance()->sysColor(3),
	  console::instance()->sysColor(1),
	  console::instance()->sysColor(1),
	  console::instance()->sysColor(3))
{}

