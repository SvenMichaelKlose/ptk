// $Id: pointer.h,v 1.1.1.1 2000/09/30 13:42:10 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/pointer.h,v $
//--------------------------------------------------------------------------
// Pointer interface.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#ifndef _ptk_pointer_H
#define _ptk_pointer_H

#include "msg.h"

#ifdef DJGPP
  #include <dpmi.h>
#endif

class pointer {
#ifdef DJGPP
  __dpmi_regs mhr;
  __dpmi_raddr mh;
#endif
  unsigned char* ptr_img;  // Bitmap for the cursor

public:
  pointer();
  ~pointer();

  int poll(msg*);
};
#endif
