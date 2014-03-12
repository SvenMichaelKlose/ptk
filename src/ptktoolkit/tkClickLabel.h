// $Id: tkClickLabel.h,v 1.1.1.1 2000/09/30 13:42:10 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptktoolkit/tkClickLabel.h,v $
//--------------------------------------------------------------------------
// Label that calls a function on click.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#ifndef _ptk_tkClickLabel_H
#define _ptk_tkClickLabel_H

#include "tkLabel.h"

class tkClickLabel : public tkLabel {
 public:
  tkClickLabel(int x=0,int y=0,int w=0,int h=0);
  //virtual ~tkClickLabel() {}

  virtual int receiver(msgbase*);
  virtual void onClick() {}
  virtual void onEnter() {}
  virtual void onLeave() {}
};
#endif
