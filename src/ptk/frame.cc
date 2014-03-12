// $Id: frame.cc,v 1.2 2001/05/04 23:56:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/frame.cc,v $
//--------------------------------------------------------------------------
// Base class for every element you want to place on the console, because
// drawing is done here.
//
// (c) 1998-1999 Sven Klose
//--------------------------------------------------------------------------
// TODO: resize won't work if EMBED_NONE

#include "drawer.h"
#include "frame.h"
#include "console.h"
#include "display.h"
#include "msg.h"
#include "ptk.h"

// Returns visible areas of a frame.
dim*
frame::visibles ()
{
    return visiblesWithin (drawerPosition ());
}

dim*
frame::visiblesWithin (dim* vis)
{
    if (!vis)
	return 0;

    dim* par = 0;
    int xp = 0;
    int yp = 0;

    // Get visible areas of parent frame
    if (!(parent () && (parent ()->parent ())))
	return vis;

#ifndef EMBED_NONE
      //&& (parent ()->parent ()->parent ()) // This sucks.
#endif
    par = parent ()->drawerPosition ();
    if (par) {
        dim psize (par);
        dim* tmp = vis->inside (&psize);
        vis->clearAll ();
        vis = parent ()->visiblesWithin (tmp);
        xp = par->x ();
        yp = par->y ();
        par->clearAll ();
    }

    // Clip visible areas with former frames
    if (vis) {
        frame* p=last ();
        while (p) {
            if (!vis)
                return 0;
            if ((p->_clipThis) && (p->_visible)) {
	        dim par(p->x () + xp, p->y () + yp, p->width (), p->height ());
	        dim* tmp = vis->clip (&par);
	        vis->clearAll ();
	        vis = tmp;
            }
            p = p->last ();
        }
    }
    return vis;
}

void inline
list_clip (dim** list, dim* clp)
{
    if (!*list)
	return;

    dim* tmp = (*list)->clip (clp);
    (*list)->clearAll ();
    *list = tmp;
}

void inline
list_add (dim** list, dim* ad)
{
    if (!*list)
        return;
    (*list)->add (ad);
}

// Draws visible areas of frame
void
frame::nextRefresh (dim* visible, int xp, int yp, drawer* dr)
{
    dim* nvis = 0;

    if (!(visible && dr))
	return;

    dim totalpos (xp + x (), yp + y (), width (), height ());
    if ((nvis = totalpos.inside (visible))) {
        draw (nvis, xp, yp, dr);
        nvis->clearAll ();
    }
}

// Refreshes a frame and its childs.
// 'invalid'   List of areas relative to the drawer that needs refreshing.
// 'validated' List of areas that were refreshed (ignored when passed).
// 'itotal'    Position of the frame relative to the drawer.
//
// Drawn areas a cut out of 'invalid' and added to 'validated'.
//
// _clipChilds If this flag is set, this frame is drawn before its childs.
//             so it can be overdrawn. This can reduce clipping and.
//             drawing in some circumstances.
//             If it's not set, first the childs are drawn and this frame.
//             is clipped be the refreshed areas.
// _clipThis   If this flag is set, this frame clips its followers.
//             If it's not set, the followers are clipped by the areas.
//             refreshed by the childs.
// _clipAble   If this flag is set, the frame is drawn.
//             If it's not set, any drawing is skipped. 
void
frame::do_sublist (
	dim** validated, frame* i, dim* itotal, 
	dim** invalid,
	int xp, int yp, drawer* dr
	)
{
    frame* chld = i->child();

    // !_clipChilds: draw frame before childs.
    if ((!i->_clipChilds || !chld) && i->_clipAble)
        i->nextRefresh (*invalid, xp, yp, dr);

    // If there's a child frame go through its list.
    dim* refreshed = 0;
    if (chld)
        refreshed = chld->subRefresh (*invalid, xp + i->x (), yp + i->y (), dr);

    // _clipChilds: cut refreshed areas off the invalid frame list
    if ((chld) && i->_clipChilds && (i->_clipAble)) {
        if (refreshed)
            list_clip (invalid, refreshed);
        i->nextRefresh (*invalid, xp, yp, dr);
    }

    // If _clipThis is set, clip followers by this frame,
    // if not, followers are already clipped by the refreshed list.
    if (i->_clipThis) {
        dim itot (xp + i->x (), yp+i->y (), i->width (), i->height ());
        list_add (validated, new dim (&itot));
        list_clip (invalid, &itot);
        if (refreshed)
            refreshed->clearAll ();
    } else
        list_add (validated, refreshed);
}

//// Repaints list of invalid frames
// xp,yp: Position of parent frame
// Returns a list of refreshed areas.
dim* frame::subRefresh (
	dim* invalid,
	int xp, int yp,
	drawer* dr, bool nofollow
	)
{
    if (!invalid)
        return 0;

    // Only use areas inside the parent or just make a copy
    if (parent ()) {
        if (parent ()->getDrawer ())
            xp = yp = 0;
        dim p (parent ());
        p.move (xp, yp);
        invalid = invalid->inside (&p);
    } else
        invalid = invalid->duplicate ();

    // Now step through the current list and draw any frames
    // that overlap an invalid area.
    frame* i = this;
    dim* refreshed = 0;
    do {
        if (!invalid)
            return refreshed;
        if (!i->_visible)
            continue;
        dim* v = invalid;

        dim itotal;
        if (!i->getDrawer ())
            itotal.moveresize (
                xp + i->x (), yp + i->y (), i->width (), i->height ()
          );
        else
            itotal.moveresize (0, 0, i->width (), i->height ());

        do {
            if (itotal.hits (v)) {
	        // Check if frame owns a drawer and use it instead of ours.
	        if (i->getDrawer ())
	            do_sublist (&refreshed, i, &itotal, &invalid,0 ,0 ,i->getDrawer ());
	        else
	            do_sublist (&refreshed, i, &itotal, &invalid, xp, yp, dr);
	        if (!invalid)
	            return refreshed; 
	        break;
            }
        } while((v = v->next ()));
        if (nofollow)
            break;
    } while ((i = i->next ()));

    if (invalid)
       invalid->clearAll ();
    return refreshed;
}

// Refresh area relative to this frame
// All drawing is done here.
void frame::refresh (dim* a)
{
    drawer* dr = 0;
    dim* d = drawerPosition ();

    frame* f = this;
    for(;;) {
        if (!f->_visible) { // Break if not visible (difference to seekDrawer)
            delete d;
            return;
        }
        if (f->getDrawer ()) {
            dr = f->getDrawer ();
            break;
        }
        if ( !(f = f->parent ()) )
            break;
    }

    dim* r=f->subRefresh (a, d->x (), d->y (), dr, true);
    if (r)
      	r->clearAll ();
    delete d;

  // Buffered drawers must be copied to the screen
#ifdef PTK_USES_BUFFERED_DRAWERS
    dr->draw (d);
#endif
}

// Repaint a frame
void frame::repaint ()
{
    if (_visible) {
        dim* d;
        if ((d = visibles ())) {
            refresh (d);
            d->clearAll ();
        }
    }
}

// Resize a frame and update display
void frame::resize (int w, int h)
{
    if ((w < 0) || (h < 0)) return;
        dim::resize (w, h);
}

// Move frame and update the display
void frame::move (int xp, int yp)
{
    if (!_visible) {
        dim::move (xp, yp);
        return;
    }
    drawer* dr = seekDrawer ();
    if (!dr) return;
    int xd = x () - xp;
    int yd = y () - yp;

  // Get src visibles
    dim* src = visibles ();

    // Get dest visibles
    int xr = xp - x ();
    int yr = yp - y ();
    dim* tsrc = src->duplicate ();
    dim* i = tsrc;
    while (i) {
        i->dim::move (i->x () + xr, i->y () + yr);
        i = i->next ();
    }
    dim::move (xp, yp);
    dim* dest = visiblesWithin (tsrc);
    dim* ref = src->clip (dest); // Get areas to refresh
    dim* copy = 0;
    if (dest) {
        // Move dest visibles to src position
        i = dest;
        do {
            i->move (i->x () - xr, i->y () - yr);
        } while((i = i->next ()));
    
        // Get all dest inside src which are the areas that can be moved by a
        // graphics accelerator.
        copy = dest->inside (src);
    }
  
    if (dest)
        dest->clearAll ();
    if (src)
        src->clearAll ();

    // Copy the areas one by one
    if (copy) {
        // Sort so copies don't overlap.
        i = copy = xysort (copy, -xd, -yd);
        do {
            // seekDrawer might be no good idea in here, the right window should
            // be determined instead. use this::getDrawer() instead before
            // entering the loop.
            dr->copy (i->x (), i->y (),
	             i->x () + xr, i->y () + yr,
	             i->width (), i->height ());
        } while ((i = i->next ()));
        copy->clearAll ();
    }
    // Refresh moved areas
    if (ref) {
        refresh (ref);
        ref->clearAll ();
    }
}

// Delete frame and update the screen
void frame::close ()
{
#ifdef EMBED_NONE
    dim d(x (), y (), width (), height ());
    visible (false);
    parent ()->refresh (&d);
#endif
    clearAll ();
}

// We don't know our minimum sizes, so we ask our child.
// Overload this functions when creating own frames.
// Mostly texts and images have a mininum size.
int frame::minx ()
{
  if (child ())
    if (_resizeable)
      return child ()->minx ();
    else return child ()->width ();
  return 0;
}

int frame::miny ()
{
  if (child ())
    if (_resizeable)
      return child ()->miny ();
    else return child ()->height ();
  return 0;
}

// Updates frame contents.
// This should be overloaded when doing automatic layout.
void frame::update ()
{
  frame* f = child ();
  while (f) {
    f->update ();
    f = f->next ();
  }
}

// Make frame visible
void frame::show ()
{
  visible (true);
#ifndef EMBED_NONE
  repaint (); // We must paint it on our own
#endif
}

// Make frame and subframes invisible
void frame::hide ()
{
  visible (false);
#ifndef EMBED_NONE
  refresh (drawerPosition ()); // Recover background
#endif
}

// Drawer handling
drawer* frame::getDrawer() { return 0; }

// Return position relative to our drawer
dim* frame::drawerPosition ()
{
  if (parent ()) {
      dim* _d = parent ()->drawerPosition ();
      _d->move (_d->x () + x (), _d->y () + y ());
      _d->resize (width (), height ());
      return _d;
    }
  return new dim (0, 0, width (), height ());
}

drawer* frame::seekDrawer ()
{
  frame* f = this;
  drawer* dr;
  while (!(dr = f->getDrawer ()))
    if (!(f = f->parent ()))
      break;
  return dr;
}

void frame::defaults ()
{
  // Frame draws nothing, clips frame with subframes and next with this.
  // Frame is visible, resizable, doesn't catch messages sent to childs.
  _clipAble = _visible = _resizeable = true;
  _clipChilds = _clipThis = _isFocus = false;
}

// Default message receiver
int frame::receiver (msgbase* m)
{
  int r = prerecv (m);
  if (r == _propagate) return postrecv (m);
  return r;
}

int frame::prerecv (msgbase*) { return _propagate; }
int frame::postrecv (msgbase*) { return _propagate; }

// Constructors
frame::frame () : dim () { defaults (); }
frame::frame (dim* g) : dim (g) { defaults (); }
frame::frame (int a, int b, int c, int d) : dim (a, b, c, d) { defaults (); }

// Grabbing and dropping the keyboard focus
void frame::grabKbdFocus () { console::instance()->setKbdFocus (this); }
void frame::dropKbdFocus () { console::instance()->setKbdFocus (0); }

// Debugging
#ifdef DEBUG
void
frame::dump (ofstream& o)
{
  o << "::frame";
  dim::dump(o);
  o << " ";
  if (_clipChilds) o <<"C";
  if (_clipThis) o << "T";
  if (_clipAble) o << "A";
  if (_visible) o << "V";
  if (_resizeable) o << "R";
  if (_isFocus) o << "F";
}

void
frame::dumplist (ofstream& o)
{
  if (!this) {
    o << "# nothing\n";
  } else {
    frame* i = this;
    do {
      i->dump (o); o << "\n";
      if (i->child ())
        i->child ()->dumplist (o);
    } while ((i = i->next()));
  }
}

// Dump branch into a stream
void
frame::store_branch (ofstream& o)
{
  o << "# ptk frame branch\n"
    << "# ptk (c)1997-1999 Sven Klose\n\n";
  dumplist(o);
  o << "# end of file\n";
}
#endif
