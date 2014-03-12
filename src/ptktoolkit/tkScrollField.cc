// $Id: tkScrollField.cc,v 1.1.1.1 2000/09/30 13:42:10 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptktoolkit/tkScrollField.cc,v $
//--------------------------------------------------------------------------
// Scrollfield style.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#include "frame.h"
#include "tkScrollField.h"
#include "console.h"

tkScrollField::tkScrollField(bool havex, bool havey,
			     int x,int y,int w,int h)
  : tkScrollFieldBase(havex ? new tkScrollBar : 0,
		      havey ? new tkScrollBar : 0,x,y,w,h)
{}

#ifdef DEBUG
void tkScrollField::dump(ofstream& o)
{
  o << "::tkScrollField";
  lmTable::dump(o);
}
#endif
