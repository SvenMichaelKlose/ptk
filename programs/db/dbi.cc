// $Id: dbi.cc,v 1.1.1.1 2000/09/30 13:42:09 sven Exp $
// $Source: /home/cvsroot/ptk/programs/db/dbi.cc,v $
//--------------------------------------------------------------------------
// Database interface for mysql.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#include "dbi.h"

#include "frame.h"
#include "console.h"
#include "tkAlert.h"

// Select current database
bool dbi::select_db (char* db)
{
  if (mysql_select_db (_server, db)) {
    alert ();
    return false;
  }
  _db = db;
  return true;
}

// Create a database
dbi* dbi::create (char *name)
{
  if (mysql_create_db (_server,name)) {
    alert ();
    return this;
  }
  return 0;
}

// Get list of databases
vector<dbEntry*>* dbi::seek_databases ()
{
  // Do query
  MYSQL_RES* result = mysql_list_dbs (_server, "%");
  if (!result) {
    console::instance ()
      ->link (new tkAlert ("Fehler!","Keine Datenbank verfuegbar."));
    return 0;
  }

  // Create dbEntry vector containing database names
  vector<dbEntry*>* databases = new vector<dbEntry*>;
  for (unsigned a = mysql_num_rows (result); a > 0; a--) {
    MYSQL_ROW row = mysql_fetch_row (result);
    dbField* fld = new dbField (0, 0, 0, 0, 0);
    databases->push_back (new dbEntry (fld, *row, 16)); //strlen(*row)+1));
  }

  mysql_free_result (result);

  return databases;
}

// Create a dbEntry vector containing the table names of the
// currently selected database
vector<dbEntry*>* dbi::seek_tables ()
{
  MYSQL_RES* result = mysql_list_tables (_server, "%");
  if (!result) {
    console::instance ()->
      link (new tkAlert ("Fehler!", "Es wurden keine Formulare definiert"));
    return 0;
  }
  vector<dbEntry*>* entries = new vector<dbEntry*>;
  for (unsigned a = mysql_num_rows (result); a>0; a--) {
    MYSQL_ROW row = mysql_fetch_row (result);
    dbField* fld = new dbField (0, 0, 0, 0, 0);
    entries->push_back (new dbEntry (fld, *row, 16)); //strlen (*row)+1));
  }
  mysql_free_result (result);
  return entries;
}

// Do Query for tables and create a dbForm for each
vector<dbForm*>* dbi::seek_forms (char* selection)
{
  // Do query
  if (mysql_query (_server,selection)) {
    console::instance ()->link (new tkAlert ("dbi", "Selection failed."));
    return 0;
  }
  vector<dbForm*>* forms = new vector<dbForm*>;
  MYSQL_RES* result = mysql_store_result (_server);
  if (!result) {
    console::instance ()->link (new tkAlert ("dbi", "Can't get result."));
    return 0;
  }
  
  // Copy field types into _curent_fields
  if (_current_fields)
    delete _current_fields;
  _current_fields = new vector<dbField*>;
  for (unsigned a = 0; a < mysql_num_fields (result); a++) {
    MYSQL_FIELD mf = result->fields[a];
    _current_fields->push_back (
      new dbField (0, clone (mf.name), mf.type, mf.length, mf.max_length <17 ? mf.max_length : 16));
  }

  // Convert rows to dbForms
  for (unsigned a = mysql_num_rows (result); a > 0; a--) {
    MYSQL_ROW row = mysql_fetch_row (result);
    if (!row) {
      console::instance ()->link (new tkAlert ("dbi", "Can't get row."));
      return 0;
    }
    dbForm* f = new dbForm (this);
    vector<dbField*>::iterator fi = _current_fields->begin ();
    for (unsigned b = 0; b < mysql_num_fields (result); b++)
      f->add (new dbEntry (*fi++, row[b], 16)); //strlen(row[b])+1));
    forms->push_back (f);
  }
  mysql_free_result (result);

  return forms;
}

dbi::dbi (char* host, char* user = 0, char* pwd = 0)
  : _host (0), _current_fields (0)
{
  _server = mysql_connect (0, host, user, pwd);
  if (!_server) {
    console::instance ()->link(
      new tkAlert("Fehler!",
                  "Es konnte keine Verbindung zum Server hergestellt werden.")
     );
    return;
  }
  _host = host;
}

dbi::~dbi ()
{
  mysql_close (_server);
}

void dbi::alert ()
{
  console::instance ()
    ->link (new tkAlert ("Fehler!", _server->net.last_error));
}

//////////////////////////////////////////////////////////////////////////

dbEntry::dbEntry (dbField* field, void* data, size_t datasize)
  : _field (field), _datasize (datasize)
{
  if (datasize > 0 && datasize < 256) {
    _data = malloc (datasize);
    memcpy (_data, data, datasize);
  } else {
    _data = malloc(1);
    *((char*) _data) = 0;
  }
}

dbEntry::~dbEntry () { delete[] _data; }

//////////////////////////////////////////////////////////////////////////

dbForm::dbForm (dbi* db) : _db (db)
{
  _entries = new vector<dbEntry*>;
}

dbForm::~dbForm ()
{
  _entries->erase (_entries->begin (), _entries->end ());
  delete _entries;
}
