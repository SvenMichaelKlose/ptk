// $Id: tkEditString.cc,v 1.2 2001/05/04 23:56:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptktoolkit/tkEditString.cc,v $
//--------------------------------------------------------------------------
// Edit line for ASCIIZ string. (style)
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#include "frame.h"
#include "console.h"
#include "tkEditString.h"
#include "tkPanel.h"
extern "C" {
  #include <string.h>
}

tkEditString::tkEditString(char* name,char* buffer, int buffersize,
			   bool pwdmode, bool clear)
  : lmTable(0,1)
{
  lmHint lcmin(left, center, (lmHint::size) (min|2), lmHint::min);
  lmHint xymin(center, center, lmHint::min, lmHint::min);
  lmHint xmax(center, center, lmHint::max, lmHint::min);
  console* con=console::instance();
  if (clear) memset(buffer,0,buffersize);
  link(new lmEqual,&xymin,&lcmin)->link(new plane(con->sysColor(2)))
    ->link(new lmEqual(&xymin))->link(new stdtext(name,con->sysColor(0),con->sysColor(2)));
  color* white=con->sysColor(3);
  color* black=con->sysColor(0);
  link(new tkPanel(white,black,black,black,black),&xmax,&lcmin)
    ->link(_inputfield=new tkInputField(buffer,buffersize,pwdmode));
}
