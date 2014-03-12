// $Id: db_list.h,v 1.1.1.1 2000/09/30 13:42:09 sven Exp $
// $Source: /home/cvsroot/ptk/programs/db/db_list.h,v $
//--------------------------------------------------------------------------
// Database selector dialog.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#ifndef _radb_db_list_H
#define _radb_db_list_H

#include "tkWindow.h"
#include "dbi.h"

class db_list : public tkWindow
{
  dbi* _db;
  vector<dbEntry*>* _dbases;
  static void func_connect_db(frame*,void* database_name);
  static void func_quit(frame*,void*);

 public:
  db_list(dbi*);
  ~db_list();
  dbi* db() {  return _db; }
};

#endif
