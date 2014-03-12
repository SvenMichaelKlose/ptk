// $Id: db_entry_view.h,v 1.1.1.1 2000/09/30 13:42:09 sven Exp $
// $Source: /home/cvsroot/ptk/programs/db/db_entry_view.h,v $
//--------------------------------------------------------------------------
// Row entry editor.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#ifndef _radb_db_entry_view_H
#define _radb_db_entry_view_H

#include "dbi.h"

#include "layout.h"
#include "pencils.h"
#include "tkPanel.h"

class db_entry_view : public tkPanel {
  dbEntry* _entry;
  color* yellow;
 public:
  db_entry_view(dbEntry*);
  ~db_entry_view() { delete yellow; }
  virtual int receiver(msgbase*);
};
#endif
