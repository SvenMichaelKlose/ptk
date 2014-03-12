// $Id: drv_embed.h,v 1.2 2001/02/03 05:18:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/x11/drv_embed.h,v $
//--------------------------------------------------------------------------
// X11 message wrapper
//
// (c) 2001 Sven Klose
//--------------------------------------------------------------------------

#ifndef _ptk_driver_embed_H
#define _ptk_driver_embed_H

#include "console.h"
#include "wmshell.h"
#include <X11/Xlib.h>

class drv_embed {

  wmshell* getWindow (frame* f, Window w);

 public:

  drv_embed();
  int event_loop(console*);
};

#endif
