// $Id: test_stresstable.h,v 1.1.1.1 2000/09/30 13:42:09 sven Exp $
// $Source: /home/cvsroot/ptk/programs/tests/test_stresstable.h,v $
//--------------------------------------------------------------------------
// Stress test with nested tables.
//
// (c)2000 Sven Klose
//--------------------------------------------------------------------------

class test_stresstable : public tkWindow
{
 public:
  test_stresstable();
  virtual int receiver(msgbase*) { return _discard; }
  static void myfunc(frame*, void*);
  frame* mktable(int);
};
