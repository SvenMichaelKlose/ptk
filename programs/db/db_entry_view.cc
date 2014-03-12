// $Id: db_entry_view.cc,v 1.1.1.1 2000/09/30 13:42:09 sven Exp $
// $Source: /home/cvsroot/ptk/programs/db/db_entry_view.cc,v $
//--------------------------------------------------------------------------
// Row entry editor.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#include "frame.h"
#include "db_entry_view.h"
#include "console.h"
#include "layout.h"

db_entry_view::db_entry_view (dbEntry* entry) : tkPanel (), _entry (entry)
{
  yellow = new color (255, 240, 224);
  console* con = console::instance ();
  color* black = con->sysColor (0);
  color* white = con->sysColor (3);
  setColors (white, black, black, black, black);
  lmHint lcmin (
    (gravity) (left | 4), center, (size) (min | 8), (size) (min | 4)
  );
  link (new lmEqual (&lcmin))
    ->link (new stdtext ((char*) _entry->data (), black, white));
  clipThis (false);
  clipChilds (false);
  setFocus (true);
  plane* p = getPlane ();
  p->clipAble (false);
  p->clipChilds (false);
  p->clipThis (true);
  clipChilds (false);
}

int db_entry_view::receiver (msgbase* m)
{
  plane* _pln = getPlane ();
  switch (m->cmd) {
  case _entering:
    _pln->clipAble (true);
    _pln->setColor (yellow);
    repaint ();
    break;
  case _leaving:
    _pln->setColor (console::instance ()->sysColor (3));
    repaint ();
    _pln->clipAble (false);
    break;
  default:
    return _propagate;
  }
  return _discard;
}
