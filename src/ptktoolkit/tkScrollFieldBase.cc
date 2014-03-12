// $Id: tkScrollFieldBase.cc,v 1.3 2001/10/11 03:26:18 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptktoolkit/tkScrollFieldBase.cc,v $
//--------------------------------------------------------------------------
// Abstract scrollfield.
//
// (c)1999-2000 Sven Klose
//--------------------------------------------------------------------------

#include "frame.h"
#include "tkScrollFieldBase.h"
#include "tkPanel.h"
#include "console.h"
#include "plane.h"

#warning lmTable does not get smaller than the minimum size needed to display its contents.

void tkScrollFieldBase::_update (frame* viewport)
{
  if (!viewport) {
    hide ();
    return;
  }
  lmTable::update ();

  // Resize viewport to its min. size needed.
  int vw = viewport->minx ();
  int vh = viewport->miny ();
  viewport->dim::resize (vw, vh);
  viewport->resizeable (false);
#ifdef DEBUG
  cout << "viewport: " << vw << "," << vh << "\n";
#endif

  // Get origin and sizes of connected scrollbars
  int w = _barx->width ();
  int h = _bary->height ();
#ifdef DEBUG
  cout << "bars: " << w << "," << h << "\n";
#endif
  if ((vw) && (vh)) {
    if (_bary)
      _bary->setSize (10000, h * 10000 / vh);
    if (_barx)
      _barx->setSize (w * 10000 / vw, 10000);
  }
}

void tkScrollFieldBase::update ()
{
  // Update viewport which'll always will have its minimum size.
  frame* v = _view->child ();
  _update (v); 
  _barx->update ();
  _bary->update ();
}

// The scrollfield is 2x2 cell table with viewport, 2 scrollbars and a
// dummy plane.
tkScrollFieldBase::tkScrollFieldBase (tkScrollBarBase* barx,
				      tkScrollBarBase* bary,
				      int x, int y, int w, int h) 
  : lmTable (2, 2),
    _barx (barx), _bary (bary),
    _former_view (0)
{
  lmHint xymax (center, center, lmHint::max, lmHint::max);
  lmHint xmax (center, center, lmHint::max, lmHint::min);
  lmHint ymax (center, center, lmHint::min, lmHint::max);
  console* con = console::instance ();

  lmTable::link (_view = new plane (con->sysColor (3)), &xymax);
  lmTable::link (_bary, &ymax);
  lmTable::link (_barx, &xmax);
  plane* p;
  lmTable::link (p = new plane (con->sysColor (2)));
  p->dim::resize (16, 16);
  p->frame::resizeable (false);
  p->frame::clipThis (true);
  _view->clipChilds (true);
  clipAble (false);
}

frame* tkScrollFieldBase::link (frame* f)
{
  if (_view->child ())
    _view->child ()->clearAll ();
  return _view->link (f);
}

void tkScrollFieldBase::_setView (int xp, int yp)
{
  frame* v = _view->child ();
  if (xp)
    xp = v->width() * xp / 10000;
  if (yp)
    yp = v->height()* yp / 10000;
  v->move(-xp, -yp);
}

int tkScrollFieldBase::receiver (msgbase *)
{
  _setView (_barx->getx (), _bary->gety ());
  return _discard;
}
