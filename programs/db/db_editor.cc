// $Id: db_editor.cc,v 1.1.1.1 2000/09/30 13:42:09 sven Exp $
// $Source: /home/cvsroot/ptk/programs/db/db_editor.cc,v $
//--------------------------------------------------------------------------
// Database editor window
//
// (c)1999-2000 Sven Klose
//--------------------------------------------------------------------------

#include "frame.h"
#include "db_editor.h"
#include "db_row_list.h"
#include "db_table_list.h"
#include "login.h"

#include "console.h"
#include "ptk.h"
#include "tkAlert.h"
#include "tkEditString.h"
#include "tkMenu.h"
#include "tkPanel.h"
#include "tkScrollBar.h"
#include "tkScrollField.h"
#include "tkSeparator.h"
#include "tkTextButton.h"

void db_editor::func_quit (frame* f, void*) { ptk::shutdown(); }

// Connect to database server
void db_editor::func_connect_dbserv (frame* fr, void*)
{
  db_editor* f = (db_editor*) fr;
  dbi* db = new dbi ("localhost", 0, 0);
  if (!db->host ()) {
    console::instance()
      ->link (new tkAlert (
        "Fehler!","Konnte keine Verbindung zum Datenbankserver aufnehmen."
	));
    delete db; return;
  }
  if (f->_db) delete f->_db;
  f->_db = db;
  console::instance ()
    ->link (new tkAlert (
      "Eisberg umschifft","Verbindung zur Datenbank ist hergestellt."
      ));
}

// Create new database
void db_editor::func_create_db (frame* fr, void*)
{
  db_editor* f = (db_editor*) fr;
  if (!f->_db && !alert_nologin ())
    return;
  f->_db->create ("radb_std_database");
}

// Logout from database server and quit
void db_editor::func_logout (frame* fr, void*)
{
  db_editor* f = (db_editor*) fr;
  if (!f->_db && !alert_nologin()) return;
  delete f->_db;
  f->_db = 0;
  func_quit (fr, 0);
}

// Open new login window
void db_editor::func_new_login (frame*, void*)
{
  console::instance ()->link (new login);
}

// Called by db_table_list
//
// data: pointer to database name
void db_editor::func_select_table (frame* fr, void* data)
{
  db_editor* f = (db_editor*) fr;
  frame* tp = f->_rowlist;
  char sel[256];
  char* tmp = "SELECT * FROM ";
  strcpy (sel, tmp);
  //strcpy(sel+strlen(tmp),(char*)data);
  strcat (sel, (char*) data);
  strcat (sel, ";");
  if (tp->child ())
    tp->child ()->clearAll ();
  tp->link (new db_row_list (f->_db, f->_db->seek_forms (sel)));
  f->update ();
  f->repaint ();
}

// Database window
db_editor::db_editor (dbi* db)
  : tkWindow ("radb: main menu"), _db (db)
{
  lmHint xymax (center, center, max, max);
  lmHint xmax (center, center, max, min);
  lmHint ymax (center, center, min, max);
  lmHint rmin (right, center, min, min);
  lmHint lxmax (left, center, max, min);
  lmHint ccmin (center, center, min, min);

  // Menubar
  tkMenuBar* bar = new tkMenuBar ();

  tkMenuPage* mp_db = new tkMenuPage ("Datenbank");
  mp_db->link (new tkMenuItem ("Oeffnen...", &func_connect_dbserv, this));
  mp_db->link (new tkMenuItem ("Neue erstellen...", &func_create_db, this));
  mp_db->link (new tkSeparator);
  mp_db->link (new tkMenuItem ("Logout", &func_logout, this));

  bar->link (mp_db);
  bar->tkMenuBar::link (new tkMenuItem ("Hilfe", &func_dummy), &xmax, &rmin);

  // Toolbar
  lmTable* mtab = new lmTable (0, 1);
  mtab->link (new tkTextButton ("Neues Login...", &func_new_login));
  mtab->link (new tkTextButton ("Suchen...", &func_dummy));

  // table treeview
  _tab_tables = new db_table_list (_db, &func_select_table, this);

  // Row list
  lmTable* rtab = new lmTable (&xmax, 1, 0);
  lmTable* rttab = new lmTable (&xmax, 0, 1);

  rtab->link (new tkRaisedPanel)->link (rttab); // Search field
  tkEditString* eds = new tkEditString ("Suche:", (char*)&_searchstr, 40);
  eds->reset ();
  rttab->link (eds, &xmax, &ccmin);

  rtab->link (new tkRaisedPanel, &xymax)
    ->link (new tkLoweredPanel)->link (_rowlist = new lmEqual);
  // select first table in database
  vector<dbEntry*>* dbs = _db->seek_tables ();
  vector<dbEntry*>::iterator dbsi = dbs->begin ();
  func_select_table (this, (*dbsi)->data ());
  dbs->erase (dbsi,dbs->end());
  delete dbs;

  lmTable* ptab = new lmTable (&xymax, 0, 1);
  ptab->link (new tkRaisedPanel)
    ->link (new tkScrollField)
    ->link (_tab_tables);
  ptab->link (rtab);

  lmTable* tab = new lmTable (&xymax, &xmax, 1, 0);
  tab->link (bar, &xmax);
  tab->link (new tkRaisedPanel, &lxmax)->link (mtab);
  tab->link (new tkRaisedPanel, &xymax)->link (ptab);
  link (new lmEqual)->link (tab);
  _tab_tables->reload ();
  show ();

#ifdef DEBUG
  ofstream o("db_editor.dump",ios::out); store_branch(o); o.close();
#endif
}

int db_editor::alert_nologin ()
{
  console::instance()
    ->link(new tkAlert(
      "Problem","Es besteht keine Verbindung zu einem Datenbankserver."
      ));
  return 0;
}
