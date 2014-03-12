// $Id: dbi.h,v 1.1.1.1 2000/09/30 13:42:09 sven Exp $
// $Source: /home/cvsroot/ptk/programs/db/dbi.h,v $
//--------------------------------------------------------------------------
// Database interface for mysql.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#ifndef _dbi_H
#define _dbi_H

#include <string.h>
#include <vector.h>
#include <mysql/mysql.h>

class dbForm;

// Field info
class dbField {
  char*  _name;
  int    _type;
  size_t _length;
  size_t _maxlength;

 public:
  dbField (dbForm*, char* name, int type, size_t length, size_t maxlength)
    : _name(name), _type(type), _length(length), _maxlength(maxlength) {}

  inline int type () 		{ return _type; }
  inline size_t length () 	{ return _length; }
  inline size_t maxlength () 	{ return _maxlength; }
  inline char* name () 		{ return _name; }
};

// Field data
class dbEntry {
  dbField* _field;
  void*    _data;
  size_t   _datasize;

 public:
  dbEntry (dbField* field, void* data, size_t datasize);
  ~dbEntry ();

  inline dbField* field () { return _field; }
  inline void* data () { return _data; }
  inline size_t size () { return _datasize; }

  inline int type () { return _field->type(); }
  inline size_t length () { return _field->length(); }
  inline size_t maxlength () { return _field->maxlength(); }
};

// A row
class dbi;
class dbForm {
  dbi* _db;
  vector<dbEntry*>* _entries;
 public:
  dbForm (dbi*);
  ~dbForm ();

  vector<dbEntry*>* entries () { return _entries; }
  void add (dbEntry* e) { _entries->push_back(e); }
  int num_fields () { return _entries->size(); }
  dbi* db () { return _db; }
};

// Database connection
class dbi {
  MYSQL* _server;
  char*  _host;
  char*  _db;
  vector<dbField*>* _current_fields;
  void alert ();
  char* clone (char* str)
    {
      char* ret = new char[strlen (str) + 1];
      strcpy (ret, str);
      return ret;
    }

 public:
  dbi (char* host, char* user = 0, char* pwd = 0);
  ~dbi ();
  
  bool select_db (char*);
  char* host () { return _host; }
  dbi* create (char* dbname);
  vector<dbForm*>*  seek_forms (char* selection);
  vector<dbEntry*>* seek_tables ();
  vector<dbEntry*>* seek_databases ();
  vector<dbField*>* current_fields () { return _current_fields; }
};

#endif
