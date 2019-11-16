// $Id: main.cc,v 1.2 2001/05/04 23:56:09 sven Exp $
// $Source: /home/cvsroot/ptk/programs/scribble/main.cc,v $
//--------------------------------------------------------------------------
// ptkscribble demo.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#include "frame.h"
#include "ptk.h"
#include "bitmap.h"
#include "layout.h"
#include "tkScrollField.h"
#include "tkWindow.h"

class scribble : public tkWindow {
  int _xp;
  int _yp;
 public:
  scribble ();
  virtual int receiver (msgbase*);
};

void func_main_quit (frame*, void*)
{
  ptk::shutdown ();
}

static bitmap* _bmp;

scribble::scribble () : tkWindow ("bitmap test")
{
  link (new lmEqual)
    ->link (new tkScrollField)
    ->link (_bmp = new bitmap (0, 0, 640, 400));
  show ();
}

int scribble::receiver (msgbase* m)
{
  switch (m->cmd) {
  case _btn_press:
    _xp=m->xrel; 
    _yp=m->yrel; 
    break;
  case _frame_dragged:
    _xp+=m->xrel; 
    _yp+=m->yrel;
    if ((_xp >= 0 && _xp < _bmp->width ())
	&& (_yp >= 0 && _yp < _bmp->height ())) {
      _bmp->get ()->set_pixel (_xp, _yp, 192, 192, 255,
      			       _bmp->get ()->format ()->size);
      dim * d = new dim (_xp, _yp, 1, 1);
      _bmp->refresh (d);
      delete d;
    }
  }
  return _discard;
}

int main()
{
  return ptk::startup (new scribble);
}
