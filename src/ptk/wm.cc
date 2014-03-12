// $Id: wm.cc,v 1.1.1.1 2000/09/30 13:42:10 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/wm.cc,v $
//--------------------------------------------------------------------------
// Window title for raw environments like DOS.
//
// (c)1998-2000 Sven Klose
//--------------------------------------------------------------------------

#include "wm.h"

#ifdef EMBED_NONE
windowTitle::windowTitle(int w,char* title)
  : frame (0,0,w,12)
{
  body=new plane(0,0,w,12,7);
  body->clipChilds(false);
  body->clipThis(false);
  link(body);
  // title text
  stdtext* s=new stdtext(12,2,title,sysColor(1),sysColor(2));
  s->clipThis(false);
  body->link(s);
  // title bar border
  title_brdr=new border(0,0,w,12,-1);
  s->add(title_brdr);

  // close button
  close_btn=new border(2,2,8,8,_TOP);
  s->add(close_btn);
  // minimize button
  min_btn=new border(w-10,2,8,8,_TOP);
  s->add(min_btn);
  // grow button
  grow_btn=new border(w-20,2,8,8,_TOP);
  s->add(grow_btn);

  clipChilds(false);
  clipThis(false);
}

void windowTitle::resize(int w,int)
{
  body->dim::resize(w,12);
  title_brdr->border::resize(w,12);
  min_btn->dim::move(w-10,2);
  grow_btn->dim::move(w-20,2);
  frame::resize(w,12);
}

int windowTitle::receiver(msgbase* m)
{
  switch (m->cmd) { 
  case _frame_dragged:
    frame* f=parent ();
    f->move (f->x()+m->xrel,f->y()+m->yrel);
    break;
  case _tree_changed :
    refresh (this);
    break;
  default:
    return frame::postrecv (m);
  }
  return _discard;
}
#endif
