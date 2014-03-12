// $Id: db_list.cc,v 1.1.1.1 2000/09/30 13:42:09 sven Exp $
// $Source: /home/cvsroot/ptk/programs/db/db_list.cc,v $
//--------------------------------------------------------------------------
// Database selector dialog.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#include "frame.h"
#include "console.h"
#include "db_editor.h"
#include "db_list.h"
#include "ptk.h"
#include "tkAlert.h"
#include "tkPanel.h"
#include "tkScrollField.h"
#include "tkSeparator.h"
#include "tkTextButton.h"

void db_list::func_connect_db(frame* fr,void* database_name)
{
  console* con=console::instance();
  db_editor* f=(db_editor*) fr;
  if (!f->db()->select_db((char*)database_name)) {
    con->link(new tkAlert("Fehler!","Kann Datenbank nicht auswaehlen:"));
  } else {
    con->link(new db_editor(f->db()));
    f->close();
  }
}

void db_list::func_quit(frame*,void*) { ptk::shutdown(); }

db_list::db_list(dbi* db) : tkWindow ("radb: Datenbankauswahl"),_db(db)
{
  // Setup frame
  lmHint xmax(center,center,max,min);
  lmTable* tab;
  link(new lmEqual)
    ->link(new tkRaisedPanel)
    ->link(tab=new lmTable(&xmax,1,0));
  lmTable* tabt;
  tab->link(new tkRaisedPanel)
    ->link(new tkScrollField)->link(tabt=new lmTable(&xmax,1,0));

  // Create a button for each database
  _dbases=_db->seek_databases();
  for(vector<dbEntry*>::iterator i=_dbases->begin(); i!=_dbases->end(); i++) {
    void* dbnam=(*i)->data();
    tabt->link(new tkTextButton((char*)dbnam, &func_connect_db, this, dbnam));
    break;
  }

  tab->link(new tkRaisedPanel,&xmax)
    ->link(new tkTextButton("Abbruch",&func_quit,this));
  show();
#ifdef DEBUG
  ofstream o("db_list.dump",ios::out); store_branch(o); o.close();
#endif
}

db_list::~db_list()
{
  delete _dbases;
}
