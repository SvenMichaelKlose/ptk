// $Id: tkPanel.h,v 1.2 2001/05/04 23:56:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptktoolkit/tkPanel.h,v $
//--------------------------------------------------------------------------
// Panels. (style)
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#ifndef _ptk_toolkit_tkPanel_H
#define _ptk_toolkit_tkPanel_H

#include "layout.h"
#include "border.h"

class plane;

class tkPanel : public border {
  color* _cp;
  plane* _plane;
  lmEqual* _connection;
 public:
  tkPanel (color* cp=0, color* ct=0, color* cr=0, color* cb=0, color* cl=0,
	int x=0, int y=0, int w=0, int h=0);
  virtual ~tkPanel() {}

  virtual frame* link(frame*);
  void setColor(color* cplane);
  void setColors(color* cp, color* ct, color* cr, color* cb, color* cl);
  plane* getPlane() { return _plane; }
#ifdef DEBUG
  virtual void dump(ofstream&);
  virtual void dumplist(ofstream&);
#endif
};

class tkLoweredPanel : public tkPanel {
 public:
  tkLoweredPanel();
  virtual ~tkLoweredPanel() {}
};

class tkRaisedPanel : public tkPanel {
 public:
  tkRaisedPanel();
  virtual ~tkRaisedPanel() {}
};

#endif
