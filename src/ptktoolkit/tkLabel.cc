// $Id: tkLabel.cc,v 1.2 2001/02/03 07:31:00 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptktoolkit/tkLabel.cc,v $
//--------------------------------------------------------------------------
// Text label.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#include "frame.h"
#include "console.h"
#include "tkLabel.h"

tkLabel::tkLabel (int x, int y, int w, int h)
  : plane (console::instance ()->sysColor (2), x, y, w, h)
{
  clipChilds (false);
  setFocus(true);
}
