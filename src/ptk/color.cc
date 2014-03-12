// $Id: color.cc,v 1.2 2001/02/03 07:12:31 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/color.cc,v $
//--------------------------------------------------------------------------
// Color management
//
// (c)1998-2000 Sven Klose
//--------------------------------------------------------------------------
// Allocate a color object for each color you need.

#include "color.h"
#include "display.h"
#include "drv_color.h"

// Allocate system color
color::color (int r, int g, int b)
{
  set (r, g, b);
  _this = (void*) new drv_color (r, g, b);
  _pixel = ((drv_color*) _this)->pixel ();
}
