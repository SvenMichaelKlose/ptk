// $Id: embed.h,v 1.3 2001/05/04 23:56:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/embed.h,v $
//--------------------------------------------------------------------------
// Wrapper for third party window managers.
//
// (c)1999-2001 Sven Klose
//--------------------------------------------------------------------------

#ifndef _ptk_embed_H
#define _ptk_embed_H

#include "console.h"

class embed {

  void* _driver;

 public:

  embed();
  int event_loop (console*);
};

#endif
