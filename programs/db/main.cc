// $Id: main.cc,v 1.1.1.1 2000/09/30 13:42:09 sven Exp $
// $Source: /home/cvsroot/ptk/programs/db/main.cc,v $
//--------------------------------------------------------------------------
// ptkdb startup.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#include "frame.h"
#include "login.h"
#include "ptk.h"

int main()
{
  return ptk::startup(new login);
}
