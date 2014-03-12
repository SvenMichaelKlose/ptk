// $Id: tree.cc,v 1.2 2001/05/04 23:56:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/tree.cc,v $
//--------------------------------------------------------------------------
// Linked list tree.
//
// (c) 1997-1999 Sven Klose
//--------------------------------------------------------------------------

#warning TODO: Make this a template class.

#include"tree.h"

int tree::items()
{
  int n=1; tree* t=start();
  while ((t=t->_next)) n++;
  return n;
}

tree::tree()
{
  _next=_last=_child=_parent=0;
}

// Baumfunktionen
void tree::add(tree* e)
{
  if(e) {
    tree* t=this; while (t->_next) t=t->_next;
    e->_last=t; e->_parent=t->_parent; t->_next=e;
  }
}

tree* tree::link(tree* e)
{
  tree* ret=e;
  if (!e) return e;
  if(!_child) {
    while(e->_last) e=e->_last;
    _child=e;
    do { e->_parent=this; } while((e=e->_next));
  } else {
    _child->add(e); return e;
  }
  return ret;
}

void tree::clearAll()
{
  tree* e=this;
  if (_parent&&(!_last))
    _parent->_child=0;
  do {
    tree* n=e; e=e->_next;
    /*    if (n->_child)
	  n->_child->clearAll();*/
    n->clear(); //delete n;
  } while (e);
}

void tree::clear()
{
  if (_child) _child->clearAll();
  if (_last) _last->_next=_next;
  if (_next) _next->_last=_last;
  if (_parent&&(!_last)) _parent->_child=_next;
  delete this;
}

bool tree::top()
{
  if (!_last) return true;
  tree* t=start();
  t->_last=this;
  _last->_next=_next;
  if (_next) _next->_last=_last;
  _last=0;
  _next=t;
  if (_parent) _parent->_child=this;
  return false;
}

/*
  tree* l=_last;
  tree* n=_next;
  tree* tn=t->_next;
  
  _last=0;
  if (tn!=this) {
  _next=tn;
  t->_last=l;
  }
  else
  {
  _next=t;
  t->_last=this;
  }
  
  t->_next=n;

  if (tn!=this) {
  tn->_last=this;
  l->_next=t;
  }
  if (n)
  n->_last=t;
*/

void tree::xchg(tree* n)
{
  // buggy
  tree* t=this;
  if (n->_next==t)  {
    tree* tmp=t; t=n; n=tmp;
  }

  tree* tl=t->_last; tree* tn=t->_next;
  tree* nl=n->_last; tree* nn=n->_next;

  if (tl) tl->_next=n;
  if (nn) nn->_last=t;

  if (t->_next==n) {
    nl=n; tn=t;
  } else {
    if (tn) tn->_last=n;
    if (nl) nl->_next=t;
  }

  t->_last=nl; t->_next=nn;
  n->_last=tl; n->_next=tn;  
}

tree* tree::start()
{
  tree* p=this;
  while (p->_last) p=p->_last;
  return p;
}

#ifdef DEBUG
void tree::dump(ofstream& o)
{
  o << " " << this
    << " " << _last << " " << _next 
    << " " << _parent << " " << _child;
}
#endif
