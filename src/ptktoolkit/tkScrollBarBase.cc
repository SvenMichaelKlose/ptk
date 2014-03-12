// $Id: tkScrollBarBase.cc,v 1.3 2001/10/11 03:26:18 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptktoolkit/tkScrollBarBase.cc,v $
//--------------------------------------------------------------------------
// Abstract scrollbar
//
// (c)1999-2000 Sven Klose
//--------------------------------------------------------------------------

#include "tkScrollBarBase.h"
#include "msg.h"

/////////////////////
// tkScrollBarBase //
////////////////////

// Get thumb position on a single axis
void _getThumbAxis(int& pos,int barsize,int relsize)
{
  if (pos < 0)
    pos = 0;
  else {
    int thumbsize = barsize * relsize / 10000;
    if (thumbsize > barsize)
      pos = 0;
    else
      if ((thumbsize + pos) > barsize)
	pos = barsize - thumbsize;
  }
}

// Get thumb's position
void tkScrollBarBase::_setThumbPos (int px, int py)
{
  int ox = _posx;
  int oy = _posy;
  _posx = px;
  _getThumbAxis (_posx, width (), _thumbsizex);
  _posy = py;
  _getThumbAxis (_posy, height (), _thumbsizey);
  if ((ox != _posx) || (oy != _posy))
    _setThumb ();
}

// Get thumb's width
int tkScrollBarBase::_thumbWidth ()
{
  int w = width () * _thumbsizex / 10000 - 1;
#ifdef DEBUG
  cout << "\n" << w << " - " << _thumbsizex << "\n";
#endif
  if (w > width ())
    return width () - 1;
  return w;
}

// Get thumb's height
inline int tkScrollBarBase::_thumbHeight ()
{
  int h = height () * _thumbsizey / 10000;
  if (h > height ())
    return height ();
  return h;
}

// Set relative thumb size
void tkScrollBarBase::setSize(unsigned sizex,unsigned sizey)
{
  _thumbsizex = sizex;
  _thumbsizey = sizey;
}

// Set thumb's position/size
void tkScrollBarBase::_setThumb ()
{
  _thumb->resizeable (true);
  _thumb->resize (_thumbWidth (), _thumbHeight ());
  _thumb->resizeable (false);
  _thumb->move (_posx, _posy);
  if (parent()) {
    msg m (_tree_changed);
    m.send (parent ());
    _thumb->update ();
    _thumb->repaint ();
  }
}

void tkScrollBarBase::update ()
{
  frame::update ();
  _setThumbPos (_posx, _posy);
}

int tkScrollBarBase::getx() { return _posx*10000/width(); }

int tkScrollBarBase::gety() { return _posy*10000/height(); }

tkScrollBarBase::tkScrollBarBase(unsigned sizex,unsigned sizey,
				 int x,int y,int w,int h)
  : frame (x, y, w, h),
    _thumbsizex (sizex), _thumbsizey (sizey),
    _dragged (false), _clicked (false),
    _posx (0), _posy (0),
    _bgr(0), _thumb (0)
{
  clipAble (true);
  clipChilds (false);
  clipThis (true);
  setFocus (true);
}

// Delete old style objects and attach new ones.
void tkScrollBarBase::defineStyle (frame* newbgr, frame* newthumb)
{
  if (_thumb)
    _thumb->clearAll ();
  if (_bgr)
    _bgr->clearAll ();
  _bgr = newbgr;
  _thumb = newthumb;
}

int tkScrollBarBase::receiver(msgbase* m)
{
  switch (m->cmd) {
  case _frame_dragged:
    _dragged = true;
    _setThumbPos (_thumb->x () + m->xrel, _thumb->y () + m->yrel);
    break;
  case _entering:
    _clicked = _dragged = false; onEnter ();
    break;
  case _leaving:
    _clicked = _dragged = false; onLeave ();
    break;
  case _btn_press:
    _clicked = true;
    break;
  case _frame_ack:
    if ((!_dragged) && _clicked)
      _setThumbPos (m->xrel - (width () * _thumbsizex / 10000 / 2) - x (),
		    m->yrel - (height () * _thumbsizey / 10000 / 2) - y ());
    _dragged = _clicked = false;
    break;
  default:
    return _propagate;
  }
  return _discard;
}
