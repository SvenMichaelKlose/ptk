// $Id: test_stresstable.cc,v 1.2 2001/05/04 23:56:09 sven Exp $
// $Source: /home/cvsroot/ptk/programs/tests/test_stresstable.cc,v $
//--------------------------------------------------------------------------
// Stress with nested tables
//
// (c)2000 Sven Klose
//--------------------------------------------------------------------------

#include <math.h>
#include <unistd.h>

#include "frame.h"
#include "console.h"
#include "ptk.h"

#include "tkScrollField.h"
#include "tkTextButton.h"
#include "tkWindow.h"

#include "test_stresstable.h"

void test_stresstable::myfunc(frame*f,void*) { f->close(); }

frame* test_stresstable::mktable(int level)
{
  if (level == 6)
    return new tkTextButton ("Phew!", &myfunc, this);
  char* l;
  switch (level % 5) {
  case 0:
    l = ":)"; break;
  case 1:
    l = ":^)"; break;
  case 2:
    l = "-*-"; break;
  case 3:
    l = "(:)"; break;
  case 4:
    l = "()"; break;
  case 5:
    l = "(x)"; break;
  }
  lmHint xmax (lmHint::center, lmHint::center, lmHint::max, lmHint::max);
  lmTable* t = new lmTable (&xmax, &xmax, 6, 0);
  for (int y = 35; y > 0; y--)
    if (y == 34)//-level)
      t->link (mktable (level + 1));
    else
      t->link (new tkTextButton (l, &myfunc, this));
  tkRaisedPanel* p = new tkRaisedPanel;
  p->link (new lmEqual (&xmax))->link (t);
  return p;
}

test_stresstable::test_stresstable () : tkWindow("ptktests: table stress")
{
  link(new lmEqual)
    //->link(new tkScrollField)
    ->link(mktable(0));
  update();
  show();

#ifdef DEBUG
  ofstream o("test_stresstable.dump",ios::out); store_branch(o); o.close();
#endif
}
