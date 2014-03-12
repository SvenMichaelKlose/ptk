// $Id: main.cc,v 1.1.1.1 2000/09/30 13:42:09 sven Exp $
// $Source: /home/cvsroot/ptk/programs/tests/main.cc,v $
//--------------------------------------------------------------------------
// ptk tests.
//
// (c)1999-2000 Sven Klose
//--------------------------------------------------------------------------

#include "frame.h"
#include "ptk.h"
#include "console.h"
#include "tkSeparator.h"
#include "tkMenu.h"
#include "tkWindow.h"

#include "test_2x2table.h"
#include "test_stresstable.h"

class MainMenu : public tkWindow {
 public:
  MainMenu();
  static void func_dummy(frame*,void*);
  static void func_quit(frame*,void*);
  static void func_2x2table(frame*,void*);
  static void func_stresstable(frame*,void*);
};

void MainMenu::func_dummy(frame*,void*) {}

void MainMenu::func_2x2table(frame*,void*) { console::instance()->link(new test_2x2table); }

void MainMenu::func_stresstable(frame*,void*) { console::instance()->link(new test_stresstable); }

void MainMenu::func_quit(frame* f,void*)
{
  ptk::shutdown();
}

MainMenu::MainMenu() : tkWindow("ptktests: main menu")
{
  lmHint xmax(center,center,lmHint::max,lmHint::min);
  lmHint rmin(right,center,lmHint::min,lmHint::min);
  lmTable* tab=new lmTable(&xmax,&xmax,1,0);

  tkMenuBar* bar=new tkMenuBar();
  tkMenuPage* mp=new tkMenuPage("Lists & tables");
    mp->link(new tkMenuItem("2x2 button Table",&func_2x2table,this));
    mp->link(new tkSeparator);
    mp->link(new tkMenuItem("Stress test",&func_stresstable,this));
  bar->link(mp);
  bar->tkMenuBar::link(new tkMenuItem("Exit",&func_quit),&xmax,&rmin);

  link(new lmEqual)->link(bar);
  show();
}

int main() { return ptk::startup(new MainMenu); }
