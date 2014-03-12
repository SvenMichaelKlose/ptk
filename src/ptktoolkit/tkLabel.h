// $Id: tkLabel.h,v 1.2 2001/05/04 23:56:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptktoolkit/tkLabel.h,v $
//--------------------------------------------------------------------------
// Text label.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#ifndef _ptk_tkLabel_H
#define _ptk_tkLabel_H

#include "plane.h"
#include "toolkit.h"

class tkLabel : public plane {
 public:
  tkLabel(int x=0,int y=0,int w=0,int h=0);
};
#endif
