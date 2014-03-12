// $Id: tkTextButton.h,v 1.2 2001/05/04 23:56:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptktoolkit/tkTextButton.h,v $
//--------------------------------------------------------------------------
// Labeled button style.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#ifndef _ptk_tkTextButton
#define _ptk_tkTextButton

#include "wmshell.h"
#include "tkButton.h"
#include "stdtext.h"

class tkTextButton : public tkButton
{
  void (*_call)(frame*,void*);
  frame* _from;
  void* _data;
  stdtext* _text;
 public:
  tkTextButton(char *msg,void (*c)(frame*,void*),
	       int x=0,int y=0,int w=0,int h=0);
  tkTextButton(char *msg,void (*c)(frame*,void*),frame* from,void* data=0,
	       int x=0,int y=0,int w=0,int h=0);

  virtual int receiver(msgbase* m);
  char* name() { return _text->name(); }
};
#endif
