// $Id: msgbase.h,v 1.1.1.1 2000/09/30 13:42:10 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/msgbase.h,v $
//--------------------------------------------------------------------------
// Internal message data.
//
// (c)1998-1999 Sven Klose
//--------------------------------------------------------------------------

#ifndef _MSGBASE_H
#define _MSGBASE_H

class msgbase {
 public:
  int   id;
  int   xrel;
  int   yrel;      // Frameposition
  int   cmd;       // Befehlscode
  void* data;	   // Dateninfo
  long  val;
  long  timestamp;
  msgbase(int a,int b,void* c,int x,int y, int v) : id(a),xrel(x),yrel(y),cmd(b),data(c),val(v) {}
};

enum msgResult {
  _propagate,
  _discard
};

enum cmd_frame {
  _frame_ack,     // Frame was clicked
                  // val: button
                  // (long) data: clicktype
  _btn_press,     // 
  _btn_release,   // 

  _move,           // Pointer has moved
  _keypress,	   // Key was pressed
  _keyrelease,     // Key was released
  _kbd_focus_in,
  _kbd_focus_out,

  _entering,       // Pointer entered frame
  _leaving,        // Pointer left frame

  _tree_changed,   // child nodes changed

  _frame_dragged,  // Frame was dragged
  _drop            // Frame was dropped
};

#endif
