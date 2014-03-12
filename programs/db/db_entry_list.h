// $Id: db_entry_list.h,v 1.1.1.1 2000/09/30 13:42:09 sven Exp $
// $Source: /home/cvsroot/ptk/programs/db/db_entry_list.h,v $
//--------------------------------------------------------------------------
// Table row editor.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#ifndef _radb_db_entry_list_H
#define _radb_db_entry_list_H

#include "dbi.h"
#include "layout.h"

class db_entry_list : public lmTable
{
  vector<dbEntry*>* _entries;;
 public:
  db_entry_list(vector<dbEntry*>* entries);
  void reload();
};

#endif
