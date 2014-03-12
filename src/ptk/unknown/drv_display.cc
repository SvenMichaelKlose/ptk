// $Id: drv_display.cc,v 1.1.1.1 2000/09/30 13:42:10 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/unknown/drv_display.cc,v $
//--------------------------------------------------------------------------
// drv_display.cc -- Display driver for X11
//
// (c) 1998-1999 Sven Klose
//--------------------------------------------------------------------------

#include "drv_display.h"
#include "display.h"

void drv_display::flush() {XFlush(_display);}

drv_display::drv_display()
{
  if (!(_display=XOpenDisplay(NULL))) {
    cerr << "Can't connect to X server. Giving up.\n";
    exit(-1);
  }
  EMSG("(Connected to X server) ");

  _screen_num=DefaultScreen(_display);

#ifdef EMBED_X11
  _charset8x8=new char[4096];
#endif
}

drv_display::~drv_display()
{
  XCloseDisplay(_display);
  EMSG("(Disconnected from X server) ");
}

void drv_display::bell() { bell(100); }

void drv_display::bell(int ms)
{
  XBell(_display,ms);
}

drv_display* drv_display::_instance=0;

drv_display* drv_display::instance()
{
  if (_instance) return _instance;
  return _instance=new drv_display();
}

int x11_screen()
{
  drv_display* drv=(drv_display*) display::instance()->drv();
  return drv->_screen_num;
}

Display* x11_display()
{
  drv_display* drv=(drv_display*) display::instance()->drv();
  return drv->_display;
}

// $Log: drv_display.cc,v $
// Revision 1.1.1.1  2000/09/30 13:42:10  sven
// ptk is a lightweight cross-platform user interface written in C++.
//
// Revision 1.1.1.1  2000/02/06 01:23:29  sven
// Let's go!
//
// Revision 1.1.1.1  2000/02/04 21:39:17  sven
// Let's go!
//
