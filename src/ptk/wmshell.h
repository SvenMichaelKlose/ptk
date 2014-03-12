// $Id: wmshell.h,v 1.3 2001/05/04 23:56:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/wmshell.h,v $
//--------------------------------------------------------------------------
// Window manager shell
//
// (c)1998-2000 Sven Klose
//--------------------------------------------------------------------------

#ifndef _ptk_wmshell_H
#define _ptk_wmshell_H

#ifdef EMBED_NONE
#include "pencils.h"
#endif

class window_drawer;

////////////
// Wmshell //
///////////
class wmshell: public frame {
  frame* connection;
  char* _title;
  bool _isPopup;

#ifdef EMBED_NONE
  border* outer_brdr;
  border* inner_brdr;
  windowTitle* title;
#endif
  int _embx;
  int _emby;


#ifdef PTK_USES_WINDOWDRAWER
  window_drawer* _drawer;
#endif

 protected:
#ifndef EMBED_NONE
  virtual dim* drawerPosition ();
#endif
#ifdef PTK_USES_WINDOWDRAWER
  virtual dim* subRefresh (dim* m, int x, int y, drawer* dummy);
#endif
  friend class drv_embed;
  void set_emb_x(int i) { _embx = i; }
  void set_emb_y(int i) { _emby = i; }

 public:
  wmshell (int x, int y, int w, int h,char* title, bool popup);
  virtual ~wmshell ();
  virtual void close();
  virtual frame* link (frame*);
  virtual int receiver (msgbase* m);
  virtual void resize (int w, int h);
  bool isPopup() { return _isPopup; }

#ifdef PTK_BUFFERED_WINDOWDRAWER
  virtual void draw (dim*, int, int, drawer*);
  virtual void drawerRefresh (int x, int y, int w, int h);
#endif

#ifdef EMBED_ANYX11
  virtual void move (int xr, int yr);
  virtual drawer* getDrawer ();
#endif

#ifndef EMBED_NONE
  virtual void show ();
  virtual void hide ();
#endif

#ifdef DEBUG
  virtual void dump (ofstream& o);
#endif
};

#endif
