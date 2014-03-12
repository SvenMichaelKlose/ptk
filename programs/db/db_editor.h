// $Id: db_editor.h,v 1.1.1.1 2000/09/30 13:42:09 sven Exp $
// $Source: /home/cvsroot/ptk/programs/db/db_editor.h,v $
//--------------------------------------------------------------------------
// Database editor window.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#ifndef _radb_db_editor_h
#define _radb_db_editor_h

#include "dbi.h"
#include "db_table_list.h"
#include "db_row_list.h"

#include "layout.h"
#include "tkScrollField.h"
#include "tkWindow.h"

class db_editor : public tkWindow
{
  dbi* _db;
  db_table_list* _tab_tables;
  frame* _rowlist;
  char _searchstr[40];

  static int alert_nologin();
  static void func_quit(frame*,void*);
  static void func_connect_dbserv(frame*,void*);
  static void func_create_db(frame*,void*);
  static void func_logout(frame*,void*);
  static void func_dummy(frame* f,void*) {}
  static void func_new_login(frame* f,void*);
  static void func_select_table(frame* fr, void* data);
 public:
  db_editor(dbi*);
  dbi* db() {return _db;}
};

#endif
