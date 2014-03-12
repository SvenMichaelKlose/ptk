// $Id: ptk.h,v 1.1.1.1 2000/09/30 13:42:10 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/ptk.h,v $
//--------------------------------------------------------------------------
// Startup/shutdown.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#ifndef _ptk_ptk_H
#define _ptk_ptk_H

#include "config.h"
#include "frame.h"

class ptk {
  public:
    ptk() {}
    static int startup(frame* intial_frame);
    static void shutdown();
};

#endif
