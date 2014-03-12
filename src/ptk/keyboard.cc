// $Id: keyboard.cc,v 1.2 2001/05/04 23:56:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/keyboard.cc,v $
//--------------------------------------------------------------------------
// Keyboard interface.
//
// (c) 1998 Sven Klose
//--------------------------------------------------------------------------

#include "keyboard.h"
#include "config.h"

keyboard::keyboard ()
{
  EMSG ("Initializing keyboard interface");
  EOK;
}

keyboard::~keyboard ()
{
  EMSG ("Freeing keyboard interface");
  EOK;
}
