// $Id: tkTree.h,v 1.2 2001/05/04 23:56:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptktoolkit/tkTree.h,v $
//--------------------------------------------------------------------------
// Tree widget.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#ifndef _ptk_tkTree_H
#define _ptk_tkTree_H

#include "layout.h"
#include "tkClickLabel.h"
#include "tkPanel.h"

class stdtext;

class tkTreeHGap : public tkLoweredPanel {
 public:
  tkTreeHGap() : tkLoweredPanel() {}
  virtual int minx() { return 8; }
};

class tkTreeVGap : public tkLoweredPanel {
 public:
  tkTreeVGap() : tkLoweredPanel() {}
  virtual int miny() { return 8; }
};

class tkTreeText : public tkClickLabel {
  void (*_func)(frame*, void* data);
  frame* _from;
  void* _data;

 public:
  tkTreeText(char* title,void(*func)(frame*, void* data),frame* from,void* data);
  virtual void onClick();
};
  
class tkTreeNode;
class tkTreeSwitch;
class tkTreeNode : public lmTable {  
  char* _title;
  friend class tkTreeSwitch;
  lmTable* _sub;
  tkTreeSwitch* _sw;
 public:
  tkTreeNode(char* title,void(*func)(frame*, void* data),frame* from, void* data);
  virtual ~tkTreeNode();
  virtual frame* link(tkTreeNode* f) { return _sub->link(f); }
};

class tkTreeSwitch : public tkRaisedPanel {
  stdtext* _txt;
  tkTreeNode* _from;
  bool _open;
  static void func_open(tkTreeSwitch*,tkTreeNode*);

 public:
  tkTreeSwitch(tkTreeNode*);
  virtual int receiver(msgbase* m);
};

#endif
