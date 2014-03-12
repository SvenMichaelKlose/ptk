// $Id: db_row_list.h,v 1.1.1.1 2000/09/30 13:42:09 sven Exp $
// $Source: /home/cvsroot/ptk/programs/db/db_row_list.h,v $
//--------------------------------------------------------------------------
// List of row editors with labeled border,
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#ifndef _radb_db_row_list_H
#define _radb_db_row_list_H

#include "dbi.h"
#include "layout.h"
#include "tkScrollField.h"

class db_row_list : public tkScrollField
{
  vector<dbForm*>* _forms;
  lmTable* _maintab;

 public:
  db_row_list(dbi*, vector<dbForm*>*);
  virtual void update();
#ifdef DEBUG
  virtual void dump(ofstream& o);
#endif
};

#endif
