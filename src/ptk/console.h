// $Id: console.h,v 1.4 2001/05/04 23:56:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/console.h,v $
//--------------------------------------------------------------------------
// Console
//
// (c)1998-1999 Sven Klose
//--------------------------------------------------------------------------
// This is a singleton class. Use console::instance() instead of console().

#ifndef _ptk_console_H
#define _ptk_console_H

#include "color.h"
#include "msg.h"

#ifndef EMBED_NONE
class console;
#include "embed.h"
#endif

class screen;
class pointer;
class keyboard;
class wmshell;

class console : public frame {
 private:
  frame* _root;
  frame* _active_window;
  frame* _kbdFocus;

  int _old_x; // Former state of pointer
  int _old_y;
  int _old_btn;

  bool _dragging; // current state of pointer
  int _curr_x;
  int _curr_y;
  int _buttonstate;
  frame* _dragged_frame;
  int _nr_sent_messages;    // Total number of sent messages

  long _last_presstime;
  long _dbl_click_first_release;
  static screen* _scr;
  static pointer* _ptr;
  static keyboard* _kbd;

  frame* seek_frame (frame* seekstart, int* xr, int* yr);
  int poll_pointer (msg*);
  console ();

  friend class drv_embed;
  int process_message (frame* root, long time);  // Processes waiting messages and creates new ones

friend class ptk;
  int message_loop ();

  friend class wmshell;
  void deactivate (wmshell*);

 protected:
  virtual drawer* getDrawer () {return 0;}

 public:
  static console* instance ();
  virtual ~console ();
  virtual void close ();

  virtual int receiver (msgbase* m);
  virtual frame* link (frame* f);

  int setKbdFocus (frame* f);
  virtual void setFocus (bool);

  color* sysColor (int);
  void bell ();
};
#endif
