// $Id: tkTextButton.cc,v 1.1.1.1 2000/09/30 13:42:10 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptktoolkit/tkTextButton.cc,v $
//--------------------------------------------------------------------------
// Labeled button style.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#include "frame.h"
#include "tkTextButton.h"
#include "console.h"

tkTextButton::tkTextButton(char *msg,void (*c)(frame*,void*),
			   int x,int y,int w,int h)
  : tkButton(_text=new stdtext(msg,
			 console::instance()->sysColor(0),
			 console::instance()->sysColor(2)),
	     x,y,w,h),
    _call(c),_from(0),_data(0)
{
  setFocus(true);
}

tkTextButton::tkTextButton(char *msg,void (*c)(frame*,void*),frame* from,void* data,
			   int x,int y,int w,int h)
  : tkButton(_text=new stdtext(msg,
			 console::instance()->sysColor(0),
			 console::instance()->sysColor(2)),
	     x,y,w,h),
    _call(c),_from(from),_data(data)
{
  setFocus(true);
}

int tkTextButton::receiver(msgbase* m)
{
  console* con=console::instance();
  if (_call)
    switch(m->cmd)
      {
      case _btn_press:
	setColor(con->sysColor(3)); repaint(); break;
      case _frame_ack:
	if (_from) (*_call)(_from,_data);
	else (*_call)(this,_data);
	  break;
      case _entering:
	setColor(con->sysColor(4)); repaint(); break;
      case _leaving:
	setColor(con->sysColor(2)); repaint(); break;
      }
  return _discard;
}
