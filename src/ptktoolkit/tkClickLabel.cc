// $Id: tkClickLabel.cc,v 1.1.1.1 2000/09/30 13:42:10 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptktoolkit/tkClickLabel.cc,v $
//--------------------------------------------------------------------------
// Label that invokes callback on click.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#include "frame.h"
#include "tkClickLabel.h"
#include "console.h"

tkClickLabel::tkClickLabel(int x,int y,int w,int h)
  : tkLabel(x,y,w,h) {}

int tkClickLabel::receiver(msgbase* m)
{
  console* con=console::instance();
  switch(m->cmd) {
  case _btn_press:
    setColor(con->sysColor(3)); repaint(); break;
  case _frame_ack:
    onClick(); break;
  case _entering:
    setColor(con->sysColor(4)); repaint(); onEnter(); break;
  case _leaving:
    setColor(con->sysColor(2)); repaint(); onLeave(); break;
  }
  return _discard;
}
