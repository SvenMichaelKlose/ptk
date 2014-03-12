// $Id: tkSeparator.h,v 1.2 2001/05/04 23:56:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptktoolkit/tkSeparator.h,v $
//--------------------------------------------------------------------------
// Separator style.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#ifndef _ptk_toolkit_tkSeparator_H
#define _ptk_toolkit_tkSeparator_H

#include "border.h"

class tkSeparator : public border {
 public:
  tkSeparator();
  virtual ~tkSeparator() {}

  virtual int minx() { return 2; }
  virtual int miny() { return 2; }
};

#endif
