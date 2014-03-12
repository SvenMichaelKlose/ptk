///////////////////////////////////////////////////////////
// ptk - embed.h                                         //
//                                                       //
// Event wrapper needed when using ptk on X11 or Windoze //
//                                                       //
// (c)1998-1999 S.Klose                                  //
//////////////////////////////////////////////////////////
#ifndef _ptk_driver_embed_H
#define _ptk_driver_embed_H

#include "console.h"
#include "wmshell.h"
#include <X11/Xlib.h>

class embed {
  wmshell* getWindow(frame* f,Window w);
 public:
  embed() {}
  int event_loop(console*);
};

#endif
