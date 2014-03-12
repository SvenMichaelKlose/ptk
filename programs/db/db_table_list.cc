// $Id: db_table_list.cc,v 1.1.1.1 2000/09/30 13:42:09 sven Exp $
// $Source: /home/cvsroot/ptk/programs/db/db_table_list.cc,v $
//--------------------------------------------------------------------------
// Table selector tree.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#include "frame.h"
#include "db_list.h"
#include "db_table_list.h"
#include "tkTree.h"

void func_dummy(frame*, void*) {}

void db_table_list::reload()
{
  // Kill old table info+view and make new ones
  if (_tables) delete _tables;
  if (child()) child()->clearAll();
  _tables=_db->seek_tables();
  for(vector<dbEntry*>::iterator i=_tables->begin(); i!=_tables->end(); i++) {
    void* tabnam=(*i)->data();
    link(new tkTreeNode((char*)tabnam,_func,_from,tabnam));
  }
  update();
  repaint();
}

db_table_list::db_table_list(dbi* db,void(*func)(frame*, void* data),frame* from)
  : lmTable(&lmHint(center,center,min,min),1,0),
    _db(db), _tables(0),
    _func(func), _from(from)
{}
