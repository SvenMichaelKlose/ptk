// $Id: tree.h,v 1.2 2001/05/04 23:56:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/tree.h,v $
//--------------------------------------------------------------------------
// Linked list tree.
//
// (c) 1997-1999 Sven Klose
//--------------------------------------------------------------------------

#ifndef _ptk_tree_H
#define _ptk_tree_H

#include "config.h"

class tree {
 protected:
  tree*    _last;
  tree*    _next;
  tree*	   _child;
  tree*    _parent;

 public:
  tree();

  // Getting parent, child and siblings
  virtual inline tree* last() {return _last;}
  virtual inline tree* next() {return _next;}
  virtual inline tree* child() {return _child;}
  virtual inline tree* parent() {return _parent;}

  // Connecting nodes
  virtual void  add (tree*); // Append node to end of this list.
  virtual tree* link (tree*); // Link child to this node.
  virtual tree* cut () { return 0; } // Cut this node from former sibling. Parent is lost.

  // Deleting nodes and branches
  virtual void  clearAll(); // Kill this and next nodes and all childs.
  virtual void  clear(); // Kill this node.

  // Moving nodes
  bool top(); // Move this node to the top of the current list.
  void xchg(tree*); // Exchange two nodes.

  // List related stuff
  tree* start(); // First node in the current list.
  int items(); // # of nodes in this list

#ifdef DEBUG
  void dump(ofstream&);
#endif
};

#endif
