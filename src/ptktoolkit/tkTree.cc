// $Id: tkTree.cc,v 1.3 2001/05/04 23:56:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptktoolkit/tkTree.cc,v $
//--------------------------------------------------------------------------
// Tree widget.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#include "frame.h"
#include "console.h"
#include "stdtext.h"
#include "tkPanel.h"
#include "tkTree.h"

#warning TODO: Implement subnodes.
tkTreeText::tkTreeText(char* title,void(*func)(frame*, void* data),frame* from,void* data) : tkClickLabel(), _func(func), _from(from), _data(data)
{
  console* con=console::instance();
  color* black=con->sysColor(0);
  color* white=con->sysColor(3);
  link(new lmEqual)
    ->link(new stdtext(title,black,white));
}

void tkTreeText::onClick()
{
  (*_func)(_from,_data);
}

const char* _minus="-";
const char* _plus="+";

// Open/close sublist
void tkTreeSwitch::func_open(tkTreeSwitch* ts,tkTreeNode* tn)
{
  if (ts->_open) {
    ts->_txt->setString((char*) _minus);
    if (tn->_sub) tn->_sub->hide();
    ts->_open=false;
  } else {
    ts->_txt->setString((char*) _plus);
    if (tn->_sub) tn->_sub->show();
    ts->_open=true;
  }    
}

int tkTreeSwitch::receiver(msgbase* m)
{
  switch(m->cmd) {
  case _frame_ack:
    func_open(this,_from); _from->repaint(); break;
  default:
    return _propagate;
  }
  return _discard;	       
}

tkTreeSwitch::tkTreeSwitch(tkTreeNode* from) : tkRaisedPanel(), _from(from)
{
  console* con=console::instance();
  color* black=con->sysColor(0);
  color* white=con->sysColor(3);
  link(new lmEqual)->link(_txt=new stdtext((char*) _plus,black,white));
  _open=false;
  setFocus(true);
}

lmHint center_center_min_min (lmHint::center,lmHint::center,lmHint::min,lmHint::min);

tkTreeNode::tkTreeNode(char* title,void(*func)(frame*, void* data),
		       frame* from, void* data)
  : lmTable(1,0), _title(title),
    _sub(new lmTable(&center_center_min_min ,1,0))
{
  lmHint xmax(lmHint::center,lmHint::center,lmHint::max,lmHint::max);
  lmHint cmin(lmHint::center,lmHint::center,lmHint::min,lmHint::min);
  lmTable* tab;
  frame::link(tab=new lmTable(0,1));
  tab->link(_sw=new tkTreeSwitch(this));
  tab->link(new tkTreeHGap,&cmin);
  tab->link(new tkTreeText(_title,func,from,data),&xmax);
}

tkTreeNode::~tkTreeNode()
{
  if (_sub) _sub->clear();
}
