// $Id: tkMenu.h,v 1.1.1.1 2000/09/30 13:42:10 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptktoolkit/tkMenu.h,v $
//--------------------------------------------------------------------------
// Menu bar, page and item.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#ifndef _ptk_toolkit_tkMenu_H
#define _ptk_toolkit_tkMenu_H

#include <vector>
#include "layout.h"
#include "wmshell.h"
#include "toolkit.h"
#include "tkClickLabel.h"

class tkMenuPopup : public wmshell {
 lmTable* _tab;
 public:
  tkMenuPopup (int x = 0, int y = 0, int w = 0, int h = 0);

  virtual frame* link (frame*);
  virtual frame* link (frame*, lmHint* hint);
};

class tkMenuBar;
class tkMenuItemBase : public tkClickLabel {
 public:
  tkMenuItemBase (char* name);

  virtual int minx ();
  virtual int miny ();
};

class tkMenuItem : public tkMenuItemBase {
  void (*_func) (frame*, void*);
  frame* _from;
  void* _data;
 public:
  tkMenuItem (char* name, void (*func) (frame*, void*));
  tkMenuItem (char* name, void (*func) (frame*, void*),
  	      frame* from, void* data = 0);

  virtual void onClick ();
};

class tkMenuPage : public tkMenuItemBase {
  tkMenuPopup* _page;
  frame* _owner;
  int _align;
  
 protected:
  friend class tkMenuBar;
  void setAlign (int a) { _align = a; }
  void setOwner (frame* f) {_owner = f;}

 public:
  tkMenuPage (char* name);

  virtual void onClick ();
  virtual frame* link (frame*);
  virtual frame* link (frame*, lmHint* hint);
  virtual frame* link (tkMenuPage*);
};

class tkMenuBar : public toolkit {
  lmTable* _tab;
 public:
  tkMenuBar ();
  tkMenuBar (dim* a) : toolkit (a) {}
  tkMenuBar (int x, int y, int w, int h) : toolkit(x, y, w, h) {}

  virtual void onClick ();
  virtual frame* link (frame* f) { return _tab->link (f); }
  virtual frame* link (frame* f, lmHint* cidef)
    { return _tab->link (f, cidef); }
  virtual frame* link(frame* f, lmHint* cdef, lmHint* idef)
    { return _tab->link(f, cdef, idef); }
  virtual frame* link(tkMenuPage*, lmHint* cdef = 0, lmHint* idef = 0);
};

class tkMenuManager;
class tkMenuManager {
  static tkMenuManager* _instance;
 public:
  tkMenuManager ();
  ~tkMenuManager ();
  static tkMenuManager* instance ();
  void registerPage (frame* owner, tkMenuPopup*);
  void unregisterAllPages ();
};

#endif
