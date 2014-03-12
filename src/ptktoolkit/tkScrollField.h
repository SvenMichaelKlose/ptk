// $Id: tkScrollField.h,v 1.2 2001/05/04 23:56:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptktoolkit/tkScrollField.h,v $
//--------------------------------------------------------------------------
// Scrollfield style.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#ifndef _ptk_tkScrollField
#define _ptk_tkScrollField

#include "tkScrollBar.h"
#include "tkScrollFieldBase.h"

class tkScrollField : public tkScrollFieldBase
{
 public:
  tkScrollField (bool havex = true, bool havey = true,
		int x = 0, int y = 0, int w = 0, int h = 0);

#ifdef DEBUG
  virtual void dump (ofstream&);
#endif
};
#endif
