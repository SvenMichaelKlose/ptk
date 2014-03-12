// $Id: console.cc,v 1.3 2001/05/04 23:56:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/console.cc,v $
//--------------------------------------------------------------------------
// Main interface between ptk and the rest of the world.
//
// (c) 1998-1999 Sven Klose
//--------------------------------------------------------------------------
// One console is allocated as the root of the frame tree. It reads the input
// devices and sends messages to the frames.
// Get a console using method 'instance'.

#include "frame.h"
#include "console.h"
#include "display.h"
#include "pointer.h"
#include "keyboard.h"
//#include "wmshell.h"

static console* _instance = 0;

screen* console::_scr = 0;
pointer* console::_ptr = 0;
keyboard* console::_kbd = 0;

static color* _sysColor[5] = {0, 0, 0, 0, 0};

int console::message_loop ()
{
#ifdef EMBED_NONE
  ptr->poll ();      // Just poll the pointer
  process_message (child ());
  return;
#else
  embed emb;
  return emb.event_loop (this);
#endif
}

// Process internal message and send it to the right frame.
int console::process_message (frame* root_frame, long time)
{
  EMSG (">>> process_message:\n");

  int moved = 0;
  int clicked = 0;

  if ((_curr_x != _old_x) || (_curr_y != _old_y))
    moved = 1;
  if ((_old_btn == 1) && (_buttonstate != _old_btn))
    clicked=1;
  
#ifdef EXCESSIVE_LOGGING
  cout << "_old_btn: " << _old_btn << " _buttonstate: " << _buttonstate << " moved: " << moved << " clicked: " << clicked << "\n";
#endif

  int xr = _curr_x;
  int xrel = _curr_x - _old_x;
  int yr = _curr_y;
  int yrel = _curr_y - _old_y;
  frame* t = seek_frame (root_frame, &xr, &yr);
  msg m (0, 0, 0, xr, yr, 0);

  // Do _enter, _leave
  if ((_active_window != t) && !_dragging) {
      EMSG ("_leaving: ");
      m.cmd = _leaving;
      m.distribute (_active_window);
      EMSG ("_entering: ");
      m.cmd = _entering;
      _active_window = t;
      m.distribute (t);
    }

  // Do _btn_press, _btn_release/_drop if dragged
  if (_buttonstate != _old_btn) {
    if (_old_btn == 1) {
      m.cmd = _btn_release;
      m.distribute (t);
      _last_presstime = time - _last_presstime;
    } else {
      m.cmd = _btn_press;
      _last_presstime = time;
      m.distribute (t);
      if (_dragging) {
	EMSG ("_drop:");
	_dragging = false;
	m.cmd = _drop;
	m.distribute (t);
      }
    }
  }
  
  int done=0;
  
  // Do _frame_dragged if we're dragging
  if (_dragging) {
    EMSG ("_frame_dragged: ");
    m.cmd = _frame_dragged;
    m.xrel = xrel;
    m.yrel = yrel;
    m.distribute (_dragged_frame);
    if (_buttonstate & 1)
      done = 1; // avoid _frame_ack message
  }

  if (!done) {
    // Do _move and initial _frame_dragged
    if (moved) {
      EMSG ("_move: ");
      m.cmd = _move;
      m.distribute (t);
      if (_buttonstate && 1) {
	EMSG ("_frame_dragged (init): ");
	m.cmd = _frame_dragged;
	m.xrel = xrel;
	m.yrel = yrel;
	m.distribute (t);
	_dragged_frame = t;
	_dragging = true;
      }
    }

    // If button was released clear dragging mode and check if it
    // was a proper click.
#warning TODO: Implement double click, long duration button press and different buttons.
    if (clicked) {
      if (_dragging)
        _dragging = false;
      if (_last_presstime < 500) {
	// Assume probable first click of a double if it's short enough
	if (_last_presstime < 250)
	  _dbl_click_first_release = time;
	EMSG ("_frame_ack: ");
	m.cmd = _frame_ack;
	m.distribute (t);
      }
    }
  }

  // Do _enter, _leave
  if ((_active_window != t) && (!_dragging)) {
    if (_active_window) {
      EMSG ("_leaving: ");
      m.cmd = _leaving;
      m.distribute (_active_window);
    } else
      goto end;
    EMSG ("_entering: ");
    m.cmd = _entering;
    _active_window = t;
    m.distribute (t);
  }

 end:
#ifdef EMBED_NONE
  dim d (_old_x, _old_y, 16, 16); // Restore former pointer background.
  _root->refresh (&d);
#endif

  _old_x = _curr_x;
  _old_y = _curr_y;
  _old_btn = _buttonstate;

#ifdef EMBED_NONE
  scr -> draw_pointer (xc, yc, ptr_img);
#endif
  EMSG ("<<< process_message\n");
  return 0;
}
 
console::console () : frame (0, 0, 32000, 32000),
  _root (0), _active_window (this), _kbdFocus (0),
  _old_x (0), _old_y (0), _old_btn (0),
  _dragging (0), //_drag_x(0), _drag_y(0),
  _curr_x (0), _curr_y (0),
  _buttonstate (0), _dragged_frame (0),
  _nr_sent_messages (0), _dbl_click_first_release (0)
{
  EHEAD ("Initialising console");

  _ptr = new pointer ();
  _kbd = new keyboard ();

  _sysColor[0] = new color (0, 0, 0);
  _sysColor[1] = new color (128, 128, 128);
  _sysColor[2] = new color (210, 210, 210);
  _sysColor[3] = new color (255, 255, 255);
  _sysColor[4] = new color (240, 240, 240);

  msg m (0, 0, 0, 0, 0, 0);
  _old_btn = _ptr->poll (&m);
  _old_x = m.xrel;
  _old_y = m.yrel;
 EOK;
}

console::~console () {}

void console::close ()
{
  EHEAD ("Shutting down console");
  delete _ptr;
  delete _kbd;
  clearAll ();
#ifdef EXCESSIVE_LOGGING
  cout << _nr_sent_messages << " messages sent.\n";
#endif
  EOK;
}

// Discard messages sent to us,
int console::receiver (msgbase*) {return _discard;}

frame* console::link (frame* f)
{
  frame::link (f);
#ifdef EMBED_NONE
  f->repaint(0);
#endif
  return f;
}

color* console::sysColor (int i) {return _sysColor[i];}

// Seek frame at position xr,yr in frame t
frame* console::seek_frame (frame* t, int* xr, int* yr)
{
  EMSG ("> frame seek:\n");
  for (;;) {
    if (t && t->isVisible ()) {
#ifdef EXCESSIVE_LOGGING
      int a = t->x () + t->width ();
      int b = t->y () + t->height ();
      cout << "frame/ptr: "
	   << t->x () << " " << t->y () << " "
	   << a << " " << b << " / "
	   << xr << " " << yr << "\n";
#endif
      int tx;
      int ty;
      if (t->getDrawer ()) {
	tx = ty = 0;
      } else {
	tx = t->x ();
	ty = t->y ();
      }
      if ((tx <= *xr)
          && (ty <= *yr)
	  && ((tx + t->width ()) > *xr)
	  && ((ty + t->height ()) > *yr)) { // Pointer lies within frame 't'
	if (!t->child() || t->isFocus())
	  break;
	*xr -= tx;
	*yr -= ty;
	t=t->child ();
	continue;
      }
    }
    
    if (t->next ())
      // ...or try the next one.
      t = t->next ();
    else {
      // Get parent node, if no frame matched in this list.
      t = t->parent ();
      break;
    }
  }
  EMSG ("< frame seek\n");
  return t;
}

int console::setKbdFocus (frame* f)
{
  if (_kbdFocus) {
    msg m(0, _kbd_focus_out, 0, 0, 0, 0);
    m.distribute (_kbdFocus);
  }
  _kbdFocus = f;
  msg m(0, _kbd_focus_in, 0, 0, 0, 0);
  m.distribute (_kbdFocus);
  return -1;
}

void console::bell ()
{
  display::instance ()->bell ();
}

console* console::instance ()
{
  if (!_instance)
    _instance = new console ();
  return _instance;
}

void console::deactivate (wmshell* w)
{
  if (_active_window)
    _active_window = 0;
}

void console::setFocus (bool)
{
#ifdef DEBUG
  cerr << "console::setFocus is no good idea.\n";
#endif
}
