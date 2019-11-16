// $Id: tkAlert.cc,v 1.2 2001/05/04 23:56:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptktoolkit/tkAlert.cc,v $
//--------------------------------------------------------------------------
// Alert dialog
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#include "frame.h"
#include "tkAlert.h"
#include "tkLabel.h"
#include "tkPanel.h"
#include "tkTextButton.h"
#include "console.h"
#include "layout.h"

void tkAlert::func_close (frame* f,void*)
{
  f->close ();
}

tkAlert::tkAlert (char* title, char* msg) : tkWindow (title)
{
  lmHint xmax (lmHint::center, lmHint::center, lmHint::max, lmHint::min);
  lmHint cmin (lmHint::center, lmHint::center, lmHint::min, lmHint::min);
  console* con = console::instance ();
  lmTable* tab = new lmTable (&xmax, &cmin, 1, 0);
  link (new lmEqual)->link (new tkRaisedPanel)->link (tab);
  tab->link (new tkRaisedPanel)
    ->link (new tkLabel)->link(new stdtext (msg,con->sysColor (0),con->sysColor (2)));
  tab->link (new tkTextButton ("Ok", &func_close, this), &xmax);
  show ();
}
  
