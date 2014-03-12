// $Id: frame.h,v 1.2 2001/05/04 23:56:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/frame.h,v $
//--------------------------------------------------------------------------
// Base class for every element you want on the console.
//
// (c) 1998-2000 Sven Klose
//--------------------------------------------------------------------------

#ifndef _ptk_frame_H
#define _ptk_frame_H

#include "dim.h"
#include "msgbase.h"
class drawer; //#include "drawer.h"

class frame : public dim {
  bool _clipChilds;
  bool _clipThis;
  bool _clipAble;
  bool _visible;
  bool _resizeable;
  bool _isFocus;
  
 private:
  void do_sublist (dim** cliplist, frame* i, dim* itot, 
			 dim** invalid,
			 int xp, int yp,
			 drawer* dr);
  void nextRefresh (dim*, int px, int py, drawer* dr);
  void defaults ();

  // Extra refresh for off-screen drawers. s.a. PTK_USES_BUFFERED_DRAWERS
#ifdef EMBED_WINDOW_REDRAW
  virtual void drawerRefresh (int x, int y, int w, int h) {
    frame* p = parent ();
    if (p) 
      p->drawerRefresh (x, y, w, h);
  }
#endif

 protected:
  friend class console;

  // Internal drawer handling
  virtual dim* subRefresh (dim* invalid, int xp,int yp,
  			   drawer* dr,
			   bool nofollow = false);
  virtual drawer* getDrawer ();
  drawer* seekDrawer ();
  virtual dim* drawerPosition (); // Position relative to the according drawer

 public:
  // Creating and killing frames:
  frame ();
  frame (dim* size);
  frame (int xpos, int ypos, int width, int height);
  virtual ~frame () {}
  virtual void close ();

  // Methods of 'dim':
  // Coordinates
  virtual void move (int xp, int yp);
  virtual void resize (int xp, int yp);

  // Pointers to neighboured nodes
  virtual inline frame* last () { return (frame*) _last; }
  virtual inline frame* next () { return (frame*) _next; }
  virtual inline frame* child () { return (frame*) _child; }
  virtual inline frame* parent () { return (frame*) _parent; }
 
 // Connecting nodes
  virtual inline frame* link(frame* f) { return (frame*) tree::link(f); }

  // Auto layout:
  inline void resizeable (bool r) { _resizeable=r; } 
  virtual bool isResizeable () { return _resizeable; }
  virtual int minx (); // Minimum width of frame
  virtual int miny (); // Minimum height of frame
  virtual void update ();   // Causes frame to update its childs

  // Clipping information:
  // Clip this frame by its subframes and the draw it
  inline void clipChilds (bool c) { _clipChilds = c; }
  // Clip parent frames by this frame (if they want to be clipped by subframes)
  inline void clipThis (bool c) { _clipThis = c; }
  // Draw this frame and perform clipChilds or clipThis if enabled.
  inline void clipAble (bool c) { _clipAble = c; }
  // True, if frame clips its parent
  inline bool doesClipping () { return _clipThis; }
  // True, if frame is drawable.
  inline bool isClipAble () { return _clipAble; }
  inline bool clippedByChilds () { return _clipChilds; }

  // Drawing and visibility:
  virtual void draw (dim*, int, int, drawer*) {};
  void repaint ();
  void refresh (dim* area);
  virtual void show ();
  virtual void hide ();
  virtual inline void visible (bool v) { _visible = v; }
  inline bool isVisible () { return _visible; }
  dim* visibles ();
  dim* visiblesWithin (dim* withinthis);

  // Messaging:
  // This methods receives all messages for this frame.
  virtual int receiver (msgbase* m);
  virtual inline void setFocus (bool f) { _isFocus = f; } 
  inline bool isFocus () { return _isFocus; }
  void grabKbdFocus (); // Grab the keyboard focus for this frame
  void dropKbdFocus (); // Drop keyboard focus for this frame.
  int prerecv (msgbase* m); // Quite useless
  int postrecv (msgbase* m);

  // Debugging
#ifdef DEBUG
  virtual void dump (ofstream&);
  virtual void dumplist (ofstream&);
  void store_branch (ofstream&);
#endif
};

#endif
