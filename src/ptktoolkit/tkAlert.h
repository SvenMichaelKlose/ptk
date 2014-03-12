// $Id: tkAlert.h,v 1.2 2001/05/04 23:56:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptktoolkit/tkAlert.h,v $
//--------------------------------------------------------------------------
// Alert dialog
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#ifndef _ptk_toolkit_tkAlert
#define _ptk_toolkit_tkAlert

#include "tkWindow.h"

class tkAlert : public tkWindow {
  static void func_close (frame*, void*);

 public:
  tkAlert (char* title, char* msg);
  virtual ~tkAlert () {}
};
#endif
