// $Id: tkInputField.cc,v 1.2 2001/05/04 23:56:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptktoolkit/tkInputField.cc,v $
//--------------------------------------------------------------------------
// Input field for ASCIIZ string.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#include "frame.h"
#include "console.h"
#include "layout.h"
#include "tkInputField.h"
#include <X11/keysym.h>
extern "C" {
  #include <string.h>
}

#warning TODO: tkInputField: scrolling

tkInputField::tkInputField (char* buffer, int max, bool pwdmode,
		       int x, int y, int w, int h)
  : plane (console::instance ()->sysColor (3), x, y, w, h),
    _buffer (buffer), _maxsize (max), _pwd (pwdmode)
{
  _currpos = _currsize = strlen (_buffer);
  console* con = console::instance ();
  char* buf;
  if (_pwd)
    buf = _pwdstr = new char[_maxsize + 1];
  else
    buf = _buffer;
  _txt = new stdtext (buf, con->sysColor (0), con->sysColor (3));
  _txt->clipThis (false);
  _txt->clipChilds (false);
  _cursor = new plane (con->sysColor (1), 0, 0, 2, 16);
  _cursor->clipThis (false);
  _cursor->clipChilds (false);
  link (_cursor);
  link (new lmEqual)->link (_txt);
  clipChilds (false);
  clipThis (true);
  setCursor ();
}

tkInputField::~tkInputField ()
{
  if (_pwd)
    delete[] _pwdstr;
}

int tkInputField::receiver (msgbase* m)
{
  console* con=console::instance ();
  switch (m->cmd) {
  case _keypress: {
    int k = m->val;
    if (((k >= XK_KP_Space)
	 && (k <= XK_KP_9))
	|| ((k >= XK_space)
	    && (k <= XK_asciitilde))) {
      if(_currsize < _maxsize) {
	if (_currsize == _currpos) {
	  _buffer[_currpos] = (char) k;
	  _buffer[_currpos + 1] = 0;
	} else {
	  memcpy(_buffer + _currpos + 1,
		 _buffer + _currpos,
		 _currsize - _currpos + 1);
	  _buffer[_currpos] = (char) k;
	}
	_currsize++;
	_currpos++;
      } else
        con->bell ();
    } else {
      switch (k) {
      case XK_Left:
	if (_currpos)
	  _currpos--;
	else
	  con->bell();
	break;
      case XK_Right:
	if (_currpos < _currsize)
	  _currpos++;
	else
	  con->bell();
	break;
      case XK_BackSpace:
	if (_currpos) {
	  if(_currpos < _currsize) {
	    strncpy (_buffer + _currpos - 1,
		    _buffer + _currpos,
		    _currsize - _currpos);
	  }
	  _currpos--;
	  _currsize--;
	  _buffer[_currsize] = 0;
	} else
	  con->bell ();
	break;
      case XK_Begin:
	_currpos = 0;
	break;
      case XK_End:
	_currpos = _currsize;
	break;
      }
    }
    if (_pwd) {
      int a = _currsize;
      char* s = _pwdstr;
      while (a--)
        *s++ = 0x2a;
      *s=0;
    }
    setCursor ();
    repaint ();
  }
  break;
  case _frame_ack: {
    int cw;
    int ppos = 0;
    _currpos = _currsize;
    for(int a = 0; a < _currsize; a++) {
      cw = _txt->stdfont ()->widthOf (_buffer[a]);
      if (m->xrel < (ppos + cw / 2)) {
	_currpos = a;
	a = _currsize;
      }
      ppos += cw;
    }
    setCursor ();
    grabKbdFocus ();
  }
  break;
  // Toolkit stuff:
  case _kbd_focus_in:
    ((plane*) _cursor)->setColor (console::instance ()->sysColor (0));
    repaint ();
    break;
  case _kbd_focus_out:
    ((plane*) _cursor)->setColor (console::instance ()->sysColor (1));
    repaint ();
    break;
  default:
      return _propagate;
  }
  return _discard;
}   

void tkInputField::setCursor ()
{
  unsigned pos = 0;
  for(int a = 0; a < _currpos; a++)
    pos += _txt->stdfont ()->widthOf (_buffer[a]);
  _cursor->dim::move (pos, 0);
}

void tkInputField::reset ()
{
  _buffer[0] = _currsize = _currpos = 0;
  setCursor ();
}
