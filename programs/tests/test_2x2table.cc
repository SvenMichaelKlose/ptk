// $Id: test_2x2table.cc,v 1.1.1.1 2000/09/30 13:42:09 sven Exp $
// $Source: /home/cvsroot/ptk/programs/tests/test_2x2table.cc,v $
//--------------------------------------------------------------------------
// Table test with 2x2 cells.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#include <math.h>
#include <unistd.h>

#include "frame.h"
#include "console.h"
#include "ptk.h"

#include "tkScrollField.h"
#include "tkTextButton.h"
#include "tkWindow.h"
#include "test_2x2table.h"

void test_2x2table::myfunc(frame*f,void*) { f->close(); }

test_2x2table::test_2x2table() : tkWindow("ptktests: 2x2 table with buttons")
{
  lmHint xmax(lmHint::center,lmHint::center,lmHint::max,lmHint::min);
  lmHint max(lmHint::center,lmHint::center,lmHint::max,lmHint::max);
  lmTable* tab_btn; 
  link(new lmEqual)
    -> link(tab_btn=new lmTable(&max,2,2));
  tab_btn->link(new tkTextButton("1st",&myfunc,this));
  tab_btn->link(new tkTextButton("2nd",&myfunc,this));
  tab_btn->link(new tkTextButton("3rd",&myfunc,this));
  tab_btn->link(new tkTextButton("4th",&myfunc,this));
  show();

#ifdef DEBUG
  ofstream o("test.dump",ios::out); store_branch(o); o.close();
#endif
}

