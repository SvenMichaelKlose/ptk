// $Id: tkButton.cc,v 1.5 2001/05/04 23:56:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptktoolkit/tkButton.cc,v $
//--------------------------------------------------------------------------
// Button widget
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#include "frame.h"
#include "tkButton.h"
#include "console.h"

void tkButton::setColor (color* c)
{
  _panel->setColor (c);
}

frame* tkButton::link (frame* d)
{
  if (_connection->child ()) _connection->child ()->clearAll ();
  return _connection->link (d);
}

tkButton::tkButton (frame* cont, int xp, int yp, int w, int h)
  : border (0, 0, 0, 0, xp, yp, w, h)
{
  color* black = console::instance ()->sysColor (0);
  setColors (black, black, black, black);
  lmHint hint (center, center, (lmHint::size) (lmHint::max | 2), (lmHint::size) (lmHint::size) (max | 2));
  lmHint hint2 (center, center, (lmHint::size) (min | 10), (lmHint::size) (min | 4));
  frame::link (new lmEqual (&hint))->link (_panel = new tkRaisedPanel)
    ->link(_connection = new lmEqual(&hint2));
  if (cont)
    link (cont);
}
