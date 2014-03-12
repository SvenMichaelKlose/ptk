// $Id: wmshell.cc,v 1.3 2001/05/04 23:56:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/wmshell.cc,v $
//--------------------------------------------------------------------------
// Native shell. Keep it running.
//
// (c) 1998-1999 Sven Klose
//--------------------------------------------------------------------------
// This belongs to the ptk window manager.

#include "frame.h"
#include "windowdrawer.h"
#include "wmshell.h"
#include "ptk.h"
#include "console.h"
#ifdef EMBED_NONE
#include "layout.h"
#endif

wmshell::wmshell (int xp, int yp, int w, int h, char* name, bool popup)
  : frame (xp, yp, w, h), _title (name), _isPopup (popup)
{
  EHEAD ("wmshell::wmshell");
#warning TODO: Handle close button
  //#warning FIXME: need new native wmshell.
#ifdef EMBED_NONE
  h += 12;

  title = new windowTitle (w, name); // Title bar
  dim::link (title);

  connection = new plane (1, title->height (), w - 2, h, sysColor (2));
  connection->clipThis (false);
  title->add (connection);

  // Window border
  outer_brdr = new border (0, title->height (), w, h - title->height (), _TOP);
  connection->add (outer_brdr);
  inner_brdr=new border(2, 2 + title->height (), w - 4, h - 4 - title->height (), _DOWN);
  connection->add (inner_brdr);
#endif

#ifndef EMBED_NONE
  _drawer = new window_drawer (name, xp, yp, w, h, _isPopup);
  clipAble (false);
#endif
  clipThis (true);
  clipChilds (true);
  EOK;
}

void wmshell::resize (int w, int h)
{
#ifdef EMBED_NONE
  h -= 12;
  if (h < 32)
    h = 32;
  connection->resize (w - 3, h - 14);
  outer_brdr->resize (w, h - 12);
  inner_brdr->resize (w - 4, h - 16);
  title->resize (w, h);
  bottombar->dim::move (0, h);
  bottombar->resize (w, 12);
  frame::resize (w, h + 12);
#else
  visible (false);
  _drawer->resize (w, h);
  dim::resize (w, h);
  frame::update ();
  visible (true);
#endif
}

frame* wmshell::link (frame* d)
{
#ifdef EMBED_NONE
  connection->dim::link (d);
#else
  dim::link (d);
#endif
  return d;
}

int wmshell::receiver (msgbase* m)
{
  switch (m->cmd) {
  case _frame_dragged :
    //move(_embx+m->xrel,_emby+m->yrel);
    break;
#ifdef EMBED_NONE
  case _frame_ack:
    if (!top ())
      refresh (this);
    break;
#endif
  case _tree_changed:
    break;
  }
  return _discard;
}

wmshell::~wmshell ()
{
  EHEAD("Closing wmshell");
  #ifdef EMBED_ANYX11
  delete _drawer;
  #endif
  EOK;
}

void wmshell::close ()
{
  hide();
  console::instance ()->deactivate (this);
  delete _drawer;
  frame::close ();
}

drawer* wmshell::getDrawer () { return _drawer; }

/////////////////////////////////////////////////////////////////////////

#ifndef EMBED_NONE
void wmshell::move (int x, int y)
{
  _drawer->move (x, y);
  dim::move (x, y);
}

dim* wmshell::drawerPosition () { return new dim (0, 0, width (), height ()); }

void wmshell::show ()
{
  _drawer->show ();
  frame::visible (true);
  dim::move (_drawer->x (), _drawer->y ());
}

void wmshell::hide ()
{
  _drawer->hide ();
  frame::visible (false);
}
#endif

/////////////////////////////////////////////////////////////////////////

#ifdef PTK_USES_WINDOWDRAWER
dim* wmshell::subRefresh (dim* m, int x, int y, drawer*)
{
#ifdef EXCESSIVE_LOGGING
  cout << "wmshell::subrefrsh\n";
#endif
  dim* d = frame::subRefresh (m, 0, 0, _drawer); //m,x,y,...
  draw (m, 0, 0, _drawer);
  return d;
}
#endif

/////////////////////////////////////////////////////////////////////////

#ifdef PTK_BUFFERED_WINDOWDRAWER
void wmshell::draw (dim* d, int, int, drawer* dr) { _drawer->draw (d); }

void wmshell::drawerRefresh (int x, int y, int w, int h)
{
  dim d(x, y, w, h);
#ifndef NOLOG
  cout << "wmshell::drawerRefresh: " << x << " " << y 
       << " " << w << " " << h << "\n";
#endif
  draw (&d, 0, 0, _drawer);
}
#endif

#ifdef DEBUG
void wmshell::dump (ofstream& o)
{
  o << "::wmshell";
  frame::dump (o);
  o << " " << x () << " " << y ()
    << " " << width () << " " << height ()
    << " \"" << _title << "\"";
}
#endif
