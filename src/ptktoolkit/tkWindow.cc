// $Id: tkWindow.cc,v 1.1.1.1 2000/09/30 13:42:10 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptktoolkit/tkWindow.cc,v $
//--------------------------------------------------------------------------
// Window container
//
// (c)1998 Sven Klose
//--------------------------------------------------------------------------

#include "frame.h"
#include "tkWindow.h"
#include "tkMenu.h"

tkWindow::tkWindow(char* title) : wmshell (0,0,8,8,title,false) {}

int tkWindow::receiver(msgbase* m)
{
  int r=prerecv(m);
  if (r==_discard) return r;
  switch (m->cmd) {
  case _btn_press:
    tkMenuManager::instance()->unregisterAllPages();
  case _tree_changed:
    return _discard;
  }
  return wmshell::receiver(m);
}

void tkWindow::show()
{
  resize(minx(),miny());
  wmshell::show();
}
