// $Id: test_2x2table.h,v 1.1.1.1 2000/09/30 13:42:09 sven Exp $
// $Source: /home/cvsroot/ptk/programs/tests/test_2x2table.h,v $
//--------------------------------------------------------------------------
// Table test with 2x2 cells.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#ifndef _test_2x2_table_h
#define _test_2x2_table_h

class test_2x2table : public tkWindow
{
 public:
  test_2x2table();
  virtual int receiver(msgbase*) { return _discard; }
  static void myfunc(frame*, void*);
};

#endif
