// $Id: display.cc,v 1.1.1.1 2000/09/30 13:42:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/display.cc,v $
//--------------------------------------------------------------------------
// Virtual screen
//
// (c) 1998-1999 Sven Klose
//--------------------------------------------------------------------------

#include "display.h"
#include "drv_display.h"

static drv_display* _this=0;

void display::flush() {_this->flush();}

display::display()
{
  _this=new drv_display();
}

display::~display()
{
  delete _this;
}

void display::bell() { bell(100); }

void display::bell(int ms)
{
  _this->bell(ms);
}

display* display::_instance=0;

display* display::instance()
{
  if (_instance) return _instance;
  return _instance = new display();
}

void* display::drv()
{
  return (void*) _this;
}
