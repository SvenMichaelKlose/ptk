// $Id: tkInputField.h,v 1.2 2001/05/04 23:56:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptktoolkit/tkInputField.h,v $
//--------------------------------------------------------------------------
// Input field for ASCIIZ string.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#ifndef _ptk_tkInputField_H
#define _ptk_tkInputField_H

#include "font.h"
#include "plane.h"
#include "stdtext.h"

class tkInputField : public plane {
  char* _buffer;
  int _currsize;
  int _maxsize;
  int _currpos;
  frame* _cursor;
  bool _pwd;
  char* _pwdstr;
  stdtext* _txt;
  void setCursor();

 public:
  tkInputField (char* buffer, int maxsize, bool passwordmode,
	     int x = 0, int y = 0, int w = 0, int h = 0);
  virtual ~tkInputField ();

  virtual int receiver (msgbase*);
  
  virtual int minx ()
  {
    return (_maxsize + 1) * _txt->stdfont ()->maxWidth ();
  }
  
  virtual int miny ()
  {
    return _txt->stdfont ()->height ();
  }
  
  void reset();
};
#endif
