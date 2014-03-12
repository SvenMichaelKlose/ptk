// $Id: login.cc,v 1.1.1.1 2000/09/30 13:42:09 sven Exp $
// $Source: /home/cvsroot/ptk/programs/db/login.cc,v $
//--------------------------------------------------------------------------
// Database login dialog.
//
// (c)1999-2000 Sven Klose
//--------------------------------------------------------------------------

#include "frame.h"
#include "db_list.h"
#include "login.h"
#include "console.h"
#include "ptk.h"
#include "tkAlert.h"
#include "tkEditString.h"
#include "tkSeparator.h"
#include "tkPanel.h"
#include "tkTextButton.h"

void login::func_login(frame* fr,void*)
{
  // Connect to database
  login* f=(login*)fr;
  f->_db=new dbi(f->_host,f->_user,f->_pwd);
  if (!f->_db->host()) {
    console::instance()->link(new tkAlert("Fehler!","Login unbekannt."));
    return;
  }

  // Create database list and close the login window
  dbi* db=f->_db;
  console::instance()->link(new db_list(db));
  f->close(); return;
}

login::login() : tkWindow ("radb: Login"),
  _host(new char[31]),_user(new char[17]), _pwd(new char[17])
{
  // Clean input fields
  lmHint xmax(center,center,max,min);
  lmHint lcmin(left,center,min,min);
  lmTable* tab;
  link(new lmEqual)
    ->link(new tkRaisedPanel)
    ->link(tab=new lmTable(&xmax,1,0));
  lmTable* tabt;
  tab->link(new tkRaisedPanel)->link(tabt=new lmTable(&xmax,&lcmin,1,0));
  tabt->link(new tkRaisedPanel)->link(new lmEqual)
    ->link(new tkEditString("Host: ",_host,30));
  tabt->link(new tkSeparator,&xmax);
  tabt->link(new tkRaisedPanel)->link(new lmEqual)
    ->link(new tkEditString("Username: ",_user,16));
  tabt->link(new tkSeparator,&xmax);
  tabt->link(new tkRaisedPanel)->link(new lmEqual)
    ->link(new tkEditString("Passwort: ",_pwd,16,true));
  tab->link(new tkRaisedPanel)
    ->link(new lmEqual)->link(new tkLoweredPanel)
    ->link(new tkTextButton("Login",&func_login,this));
  show();

#ifdef DEBUG
  ofstream o("login.dump",ios::out); store_branch(o); o.close();
#endif
}

