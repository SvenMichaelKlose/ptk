// $Id: db_entry_list.cc,v 1.1.1.1 2000/09/30 13:42:09 sven Exp $
// $Source: /home/cvsroot/ptk/programs/db/db_entry_list.cc,v $
//--------------------------------------------------------------------------
// Table row editor,
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#include "frame.h"
#include "db_entry_list.h"
#include "db_entry_view.h"

#include "tkTextButton.h"

void db_entry_list::reload()
{
}

// Fill row  with field views
db_entry_list::db_entry_list (vector<dbEntry*>* entries)
  : lmTable (&lmHint (center, center, max, max), 0, 1), _entries (entries)
{
  for (vector<dbEntry*>::iterator i = _entries->begin ();
      i != _entries->end(); i++)
    link (new db_entry_view (*i));
  clipAble (false);
  clipThis (false);
  clipChilds (false);
}
