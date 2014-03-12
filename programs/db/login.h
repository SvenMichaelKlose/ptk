// $Id: login.h,v 1.1.1.1 2000/09/30 13:42:09 sven Exp $
// $Source: /home/cvsroot/ptk/programs/db/login.h,v $
//--------------------------------------------------------------------------
// Database login dialog.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#ifndef _radb_login_H
#define _radb_login_H

#include "tkWindow.h"
#include "dbi.h"

class login : public tkWindow
{
  dbi* _db;
  static void func_login(frame*,void*);

 public:
  char* _host;
  char* _user;
  char* _pwd;
  login();
};

#endif
