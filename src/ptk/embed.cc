// $Id: embed.cc,v 1.2 2001/02/03 05:18:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/embed.cc,v $
//--------------------------------------------------------------------------
// Wrapper for third party window managers.
//
// (c)1999-2001 Sven Klose
//--------------------------------------------------------------------------

#include "frame.h"
#include "embed.h"
#include "drv_embed.h"

embed::embed ()
{
  this->_driver = (void*) new drv_embed ();
}

int embed::event_loop (console* con)
{
  return ((drv_embed*) this->_driver)->event_loop (con);
}
