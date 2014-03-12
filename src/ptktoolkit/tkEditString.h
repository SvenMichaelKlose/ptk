// $Id: tkEditString.h,v 1.1.1.1 2000/09/30 13:42:10 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptktoolkit/tkEditString.h,v $
//--------------------------------------------------------------------------
// Edit line for ASCIIZ string. (style)
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#ifndef _ptk_tkEditString_H
#define _ptk_tkEditString_H

#include "layout.h"
#include "tkInputField.h"

class tkEditString : public lmTable {
  tkInputField* _inputfield;

 public:
  tkEditString(char* name,char* buffer, int buffersize, bool pwdmode=false, bool clear=true);

  void reset() { _inputfield->reset(); }
};
#endif
