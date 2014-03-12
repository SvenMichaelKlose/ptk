// $Id: db_table_list.h,v 1.1.1.1 2000/09/30 13:42:09 sven Exp $
// $Source: /home/cvsroot/ptk/programs/db/db_table_list.h,v $
//--------------------------------------------------------------------------
// Table selector tree.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#ifndef _radb_db_table_list_H
#define _radb_db_table_list_H

#include "dbi.h"
#include "layout.h"

class db_table_list : public lmTable
{
  dbi* _db;
  vector<dbEntry*>* _tables;
  void (*_func)(frame*, void*);
  frame* _from;
    public:
  db_table_list(dbi* db,void(*func)(frame*, void*), frame *from);
  void reload();
};

#endif
