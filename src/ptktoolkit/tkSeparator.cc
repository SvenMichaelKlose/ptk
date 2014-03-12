// $Id: tkSeparator.cc,v 1.2 2001/02/03 07:31:00 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptktoolkit/tkSeparator.cc,v $
//--------------------------------------------------------------------------
// Separator style.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#include "frame.h"
#include "console.h"
#include "tkSeparator.h"

tkSeparator::tkSeparator()
  : border(console::instance()->sysColor(1),
	   console::instance()->sysColor(3),
	   console::instance()->sysColor(3),
	   console::instance()->sysColor(1))
{}
