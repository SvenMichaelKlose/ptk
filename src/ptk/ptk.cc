// $Id: ptk.cc,v 1.2 2001/05/04 23:56:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/ptk.cc,v $
//--------------------------------------------------------------------------
// Startup/shutdown functions.
//
// Sven Klose
//--------------------------------------------------------------------------

#include <stdlib.h>
#include "ptk.h"
#include "console.h"

int ptk::startup (frame* initframe)
{
#ifndef EMBED_NONE
  console* con=console::instance();
#endif

#ifdef EMBED_NONE
  plane* p=new plane(0,0,320,200,new color(0,0,0)); // black background
  p->link(initframe);
  con->link(p);
#else
  con->link(initframe);
#endif

  int retcode = con->message_loop();
  con->close();
  return retcode;
}

void ptk::shutdown()
{
  console::instance()->close(); exit(0);
}
