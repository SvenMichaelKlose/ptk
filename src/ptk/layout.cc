// $Id: layout.cc,v 1.3 2001/05/04 23:56:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/layout.cc,v $
//--------------------------------------------------------------------------
// Automatic layout
//
// (c) 1998-2000 Sven Klose
//--------------------------------------------------------------------------

#include "frame.h"
#include "layout.h"

////////////
// lmCell //
///////////
// Must be used with tables for cell layout.
lmCell::lmCell (lmHint* c, lmHint* i) :
    layout (c->x (), c->y (), c->width (), c->height ()),
    _cell_def (new lmHint (c)), _item_def (new lmHint (i)) {}

// Updates cell's childs
void lmCell::update ()
{
  frame* c = child ();
  if (!c)
    return;
  if (!c->isVisible ()) {
    hide ();
    return;
  }
  lmHint* i = _item_def;

  int cw;
  int ch;
  if (c->isResizeable ()) {
    cw = getSize (c->width (), width (), i->width (), c->minx ());
    ch = getSize (c->height (), height (), i->height (), c->miny ());
    c->dim::resize (cw, ch);
  } else {
    cw = c->width ();
    ch = c->height ();
  }
  c->dim::move (getPosition (c->width (), cw, width (), i->x ()),
	        getPosition (c->height (), ch, height (), i->y ()));
  frame::update ();
}

// Get min. cell width/height in pixels.
int lmCell::minx()
{
  frame* c = child ();
  if (!c || !isVisible ())
    return 0;
  if (!c->isResizeable ())
    return c->width ();
  return
    getMinSize (c->width (), _item_def->width (), c->minx ());
}

int lmCell::miny()
{
  frame* c = child ();
  if (!c || !isVisible ())
    return 0;
  if (!c->isResizeable ())
    return c->height ();
  return
    getMinSize (c->height (), _item_def->height (), c->miny ());
}

#ifdef DEBUG
void lmCell::dump (ofstream& o) { o << "::lmCell"; layout::dump (o); }
#endif

////////////////////////////////////////////////////////////

/////////////
// lmTable //
////////////

// Get minimum width/height of a cell
inline int _getCellAxis (frame* cell,
			int sizehint,
			int pixelsleft,
			int itemsleft,
			int& shareitemsleft,
			int amin,
			int& share, // shared space for lmHint::max
			int min)
{
  if (sizehint & lmHint::max) {
    int p = 0;
    if (share && shareitemsleft)
      p = share / shareitemsleft;
    else
      p = share;
    share -= p;
    shareitemsleft--;
    return amin + p;
  }

  // Minimum size is the default
  if (sizehint & lmHint::equal)
    return amin;
  else
    return min;
}

// Update table layout
void lmTable::update () {
  EMSG ("> lmTable::update: ");
  getmins (true);
  EMSG ("<\n");
}

// Get min. height of a table row
int getRowsMinY (frame* start, int num, dim* yrefp)
{
  if (yrefp)
    return yrefp->height ();

  int ymin = 0;
  for (; num > 0; num--) {
    int a;
    if (start->isResizeable ())
      a = start->miny ();
    else
      a = start->height ();
    if (ymin < a)
      ymin = a;
    if (!(start = start->next ()))
      break;
  }
  return ymin;
}

// Get min. width of a row.
int getRowsMinX (frame* i, int num, dim* xrefp)
{
  int xmin = 0;
  for(; num > 0; num--) {
    if (!xrefp) {
      if (i->isResizeable ())
	xmin += i->minx ();
      else
	xmin += i->width ();
    } else {
      xmin += xrefp->width ();
      xrefp = xrefp->next ();
    }
    if (!(i = i->next ()))
      break;
  }
  return xmin;
}

// Get minimum table size and optionally update table layout.
void lmTable::getmins(bool do_update)
{
  frame* d;
  if (!(d = child ()))
    return;
  int cnt = d->items ();

  // Get # of items on each axis
  if ((xcnt) && (!ycnt))
    ycnt = cnt / xcnt;
  if ((ycnt) && (!xcnt))
    xcnt = cnt / ycnt;

  // get:
  // minWidth/minHeight = total minimum size of table
  // ymaxleft = Number of rows where lmHint::max is used on the y-axis
  //            by any item.
  int ymaxleft = minWidth = minHeight = 0;
  dim* yrefp = _yref;
  for (int _y = ycnt; _y > 0; _y--) {
    dim* xrefp = _xref;
    minHeight += getRowsMinY (d, xcnt, yrefp); // Get min. row height
    int mx = getRowsMinX (d, xcnt, xrefp);

    // Use max. space for row if a single item wants max height.
    frame* d2 = d;
    for (int tmp = xcnt; tmp > 0; tmp--) {
      lmHint* def;
      if ((def = ((lmCell*) d2)->getCellDef ()))
	if (flag (def->height ()) & lmHint::max) {
	  ymaxleft++;
	  break;
	}
      d2 = d2->next ();
    }

    // Move pointer to next line.
    for (int _x = xcnt; _x > 0; _x--)
      if (!(d = d->next ()))
        break;

    if (minWidth < mx)
      minWidth = mx;
    if (!d)
      break;
    if (yrefp)
      yrefp = yrefp->next ();
  }

  if (!do_update)
    return; // The min. size is all we wanted to know.

  /*************************************************************************/

  // Set up new layout
  int newy = 0;	// Position of current cell.
  int sizeX = flag (_cell_def->width ()); // Default cell attributes.
  int sizeY = flag (_cell_def->height ());

  // Counters that will be updated for each new row.
  int yleft = dim::height ();	// Pixels left on Y axis.
  int yitemsleft = ycnt;	// Number of rows left.
  int yshare = yleft - minHeight;

  d = child ();
  do {
    // Intialize run through row
    int ymin = getRowsMinY (d, xcnt, yrefp); // Minimum row height.
    int newx = 0;	// Position of current cell.
    int last_h = 0;	// Height of last cell.
    int xleft = dim::width (); // Pixels on X axis left in current row.
    dim* xrefp = _xref;	// Pointer to reference widths to use.
    int xshare = xleft - getRowsMinX (d, xcnt, xrefp);
			// Pixels left on X axis that can be shared by cells
			// that want the maximum width.

    // Count cells that want maximum space in this row.
    int xmaxleft = 0;	// Number of cells left that want maximum space.
    frame* d2 = d;
    for (int tmp = xcnt; tmp > 0; tmp--) {
      lmHint* def;
      if ((def = ((lmCell*) d2)->getCellDef ()))
	if (flag(def->width ()) & lmHint::max)
	  xmaxleft++;
      d2 = d2->next ();
    }

    bool ymaxdec = false;
    for (int xitemsleft = xcnt; xitemsleft > 0; xitemsleft--) {
#ifdef EXCESSIVE_LOGGING
      cout << newx << " x " << newy << " - ";
#endif
      d->dim::move (newx, newy);

      int neww;	// Size of current cell.
      int newh;

      // Get cell's size if it's resizeable.
      if (d->isResizeable ()) {
	// Use size list if available
	if (xrefp) {
	  neww = xrefp->width ();
	  xrefp = xrefp->next ();
	}

	int xmin = d->minx ();
	lmHint* def;
	int tmp = ymaxleft;
	int ysh = yshare;

	if ((def = ((lmCell*) d)->getCellDef ())) {
	  int sx = flag (def->width ()); // Use cell's attribs.
	  int sy = flag (def->height ());
	  if (!xrefp)
	    neww = _getCellAxis (
	      d, sx, xleft, xitemsleft, xmaxleft, xmin, xshare, d->minx ()
	    );
	  newh = _getCellAxis (
	      d, sy, yleft, yitemsleft, tmp, ymin, ysh, d->miny ()
	    );
	} else {
	  // Use default attribs.
	  if (!xrefp)
	    neww = _getCellAxis (
	      d, sizeX, xleft, xitemsleft, xmaxleft, xmin, xshare, d->minx ()
	    );
	  newh = _getCellAxis (
	      d, sizeY, yleft, yitemsleft, tmp, ymin, ysh, d->miny ()
	    );
	}

	if (tmp != ymaxleft)
	  ymaxdec = true;

        d->dim::resize (neww, newh);
      } else {
	neww = d->width (); // Cell is not resizeable.
	newh = d->height ();
      }
      if (last_h < newh)
        last_h = newh;
    
#ifdef EXCESSIVE_LOGGING
      cout << neww << " x " << newh << " - " << d->isResizeable () << "\n";
#endif

      d->update ();

      // Initialize next cell
      newx += neww;
      xleft -= neww;
      if (!(--cnt))
        return;
      d = d->next ();
    }

    // Initialize next row
    if (yrefp)
      yrefp = yrefp->next ();
    if ((sizeY & equal) == equal)
      last_h = ymin;
    if (yshare && ymaxleft)
      yshare -= yshare / ymaxleft;
    yitemsleft--;
    if (ymaxdec)
      ymaxleft--;
    yleft -= last_h;
    newy += last_h;
  } while (yitemsleft > 0);
}

void lmTable::defaults (lmHint* cell_def, lmHint* item_def)
{
  _xref = _yref = 0;
  if (cell_def)
    _cell_def = new lmHint (cell_def);
  else
    _cell_def = new lmHint (center, center, size::min, size::min);
  if (item_def)
    _item_def = new lmHint (item_def);
  else
    _item_def = new lmHint (center, center, size::min, size::min);

  if (!(xcnt | ycnt)) {
#ifdef DEBUG
    cerr << "lmTable: Need # of items on x and/or y axis! "
      "Setting default xc=1.\n";
#endif
    xcnt = 1;
  }
}

// Various table constructors.
lmTable::lmTable (int xsize, int ysize)
    : layout (), xcnt (xsize), ycnt (ysize)
{ defaults (0, 0); } 

lmTable::lmTable (lmHint* cell_and_item_def, int nx, int ny)
  : layout (), xcnt (nx), ycnt (ny)
{ defaults (cell_and_item_def, cell_and_item_def); }

lmTable::lmTable (lmHint* cell_def, lmHint* item_def, int nx, int ny)
  : layout (), xcnt (nx), ycnt (ny)
{ defaults (cell_def, item_def); }

lmTable::lmTable (lmHint* cell_def, lmHint* item_def, int nx, int ny,
		  dim* total)
  : layout (total), xcnt (nx), ycnt (ny)
{ defaults (cell_def, item_def); }

// Create default cell for table
lmCell* lmTable::createCell (lmHint* c, lmHint* i)
{
  lmHint sc (center, center, size::min, size::min);
  return new lmCell (c ? c : &sc, i ? i : &sc);
}

// Link item via cell using specified layout hints
frame* lmTable::link (frame* f, lmHint* cell_def, lmHint* item_def)
{
  return frame::link (createCell (cell_def, item_def))->link (f);
}

// Link via cell set to default layout hints passed to lmTable
frame* lmTable::link (frame* f)
{
  return frame::link (new lmCell (_cell_def, _item_def))->link (f);
}

#ifdef DEBUG
void lmTable::dump (ofstream& o)
{
  o << "::lmTable"; layout::dump (o);
}
#endif

int lmTable::minx ()
{
  if (!minWidth)
    getmins (false);
  return minWidth;
}

int lmTable::miny ()
{
  if (!minHeight)
    getmins (false);
  return minHeight;
}

////////////////////////////////////////////////////////////

/////////////
// lmEqual //
////////////
// Makes child the same size as the parent.
void lmEqual::update ()
{
  int w;
  int h;
  int xp = 0;
  int yp = 0;
  frame* c = child ();
  if (c && parent ()) {
    int pw = parent ()->dim::width ();
    int ph = parent ()->dim::height ();
    if (_item_def) {
      if (c->isResizeable ()) {
	w = getSize (c->width (), pw, _item_def->width (), c->minx ());
	h = getSize (c->height (), ph, _item_def->height (), c->miny ());
      } else {
	w = c->width ();
	h = c->height ();
      }
      xp = getPosition (c->width (), w, pw, _item_def->x ());
      yp = getPosition (c->height (), h, ph, _item_def->y ());
    } else {
      w = pw;
      h = ph;
    }
#ifdef EXCESSIVE_LOGGING
    cout << "lmEqual::update: resizing to " << w << " x " << h << "\n";
#endif
    dim::move (xp, yp);
    dim::resize (w, h);
    c->dim::move (0, 0);
    if (c->isResizeable ())
      c->dim::resize (w, h);
  }
  frame::update (); // Update child frames as well
}

int lmEqual::minx ()
{
  if (_item_def && child ())
    return
      getMinSize (child ()->width (), _item_def->width (), child ()->minx ());
  else
    return frame::minx ();
}
int lmEqual::miny()
{
  if (_item_def && child ())
    return
      getMinSize (child ()->height (), _item_def->height (), child ()->miny ());
  else
    return frame::miny ();
}

lmEqual::lmEqual () : layout (), _item_def (0) {}
lmEqual::lmEqual (lmHint* i) : layout (), _item_def (new lmHint (i)) {}

#ifdef DEBUG
void lmEqual::dump (ofstream& o)
{
  o << "::lmEqual"; layout::dump (o);
}
#endif

////////////////////////////////////////////////////////////

////////////
// layout //
///////////

int layout::getPosition (int cmin, int ccurrent, int avail, int poshint)
{
  int v = 0;
  switch (flag (poshint) & (perc - 1))
    {
    case center:
      v = (avail - ccurrent) / 2;
      return v + getPercent (avail, poshint);
    case 0:
    case gravity::top: //case left:
      return getPercent (avail, poshint);
    case bottom: //case right:
      v = avail - ccurrent;
      return v - getPercent (avail, poshint);
    default:
      // No hint available, it's a fixed value.
      return getPercent (avail, poshint);
    }
}

// Get size value from current child size, the child's min. size,
// the maximum available space
int layout::getSize (int ccurrent, int avail, int sizehint, int cmin)
{
  switch (flag (sizehint) & (perc - 1))
    {
    case 0:
    case min:
      return cmin;
    case max:
      return avail - getPercent (avail, sizehint);
    case equal:
      return avail - getPercent (avail, sizehint);
    case preset:
      return ccurrent;
    default:
    return avail - getPercent (avail, sizehint);
    }
}


// Get min. size from current size or min. size depending on hint
int layout::getMinSize (int ccurrent, int sizehint, int cmin)
{
  if ((flag (sizehint) & (perc - 1)) == preset)
    return ccurrent;
  return cmin + getPercent (cmin, sizehint);
}

int layout::getPercent (int v, int hint)
{
  if (hint & perc)
    return v = v * val (hint) / 100;
  return val (hint);
}

void layout::defaults ()
{
//  clipThis (false);
//  clipChilds (false);
  clipAble (false);
  minWidth = minHeight = 0;
}

#ifdef DEBUG
void layout::dump (ofstream& o)
{
  o << "::layout";
  frame::dump (o);
  o << " " << minWidth << " " << minHeight;
}
#endif

void lmFlow::update ()
{
  // If there's a previous box, let it update first because it'll tell us the
  // starting point in the content list using our reconnect() method.
  if (_previous_box != 0) {
    _previous_box->update ();
    return; // We'll get called again by the previous box.
  } else {
    // Reconnect append all content into our box if we're the first.
    append_following_content ();
  }

  int w = width () - _right_border;
  int h = height () - _bottom_border;
  int x = _left_border;
  int y = _top_border;
  int bottom = y;
  frame* p = _content;
  while (y < h) {
    p->update ();
    int pw = p->width () + _minxdist;
    if (x + pw >= w) {
      // Move to next line.
      x = _left_border;
      y = bottom;
    }
    int ph = p->height () + _minydist;
    if (y + ph >= h)
      break; // Box is full.
    if (y + ph > bottom)
      bottom = y + ph;
    p->move (x, y);
    x += pw;
  }
  if (_next_box != 0) {
    _next_box->reconnect ((frame*) p->tree::cut ());
    _next_box->update ();
  }
}
