// $Id: tkWindow.h,v 1.1.1.1 2000/09/30 13:42:10 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptktoolkit/tkWindow.h,v $
//--------------------------------------------------------------------------
// Window container
//
// (c)1998 Sven Klose
//--------------------------------------------------------------------------

#ifndef _ptk_toolkit_tkWindow_H
#define _ptk_toolkit_tkWindow_H

#include "wmshell.h"

class tkWindow : public wmshell
{
 public:
  tkWindow(char* title);
  virtual ~tkWindow() {}

  virtual void show();
  virtual int receiver(msgbase* m);
};

#endif
