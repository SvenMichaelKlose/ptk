// $Id: tkMenu.cc,v 1.1.1.1 2000/09/30 13:42:10 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptktoolkit/tkMenu.cc,v $
//--------------------------------------------------------------------------
// Menu bar, page and item.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#include "frame.h"
#include "console.h"
#include "tkMenu.h"
#include "tkPanel.h"
#include "tkTextButton.h"

///////////////////
// tkMenuManager //
//////////////////
// NOTE: the menu manager handles opening and closing of tkMenuPopups.
//       When registering a page, the pane where it's on is passed as the owner,
//       *NOT* the tkMenuPage item. If pages don't hide as expected, check
//       if a pointer to a tkMenuPopup or tkMenuBar is used.
//       Somehow this should be solved a better way.
class _popupRec { // Record of a opened page
    public:
    frame* _owner;
    tkMenuPopup* _page;
    _popupRec (frame* o, tkMenuPopup* p) : _owner (o), _page (p) {}
};

using namespace std;
static vector<_popupRec*> _pages;

// Hide all tkMenuPopups that belong to 'owner'
void unregisterPagesOf (frame* owner)
{
  for(vector<_popupRec*>::iterator i = _pages.begin ();i != _pages.end (); i++)
    if ((*i)->_owner == owner) {
      unregisterPagesOf ((*i)->_page);
      (*i)->_page->hide ();
      _pages.erase (i);
      return;
    }
}

// Add a page to the database and show it.
void tkMenuManager::registerPage (frame* owner, tkMenuPopup* page)
{
  unregisterPagesOf (owner);
  _pages.push_back (new _popupRec (owner, page));
  page->resize (page->minx (), page->miny ());
  page->show ();
}

// Hide all tkMenuPopups
void tkMenuManager::unregisterAllPages ()
{
  for(vector<_popupRec*>::iterator i = _pages.begin (); i != _pages.end (); ++i)
    (*i)->_page->hide ();
  _pages.erase (_pages.begin (), _pages.end ());
}

tkMenuManager* tkMenuManager::_instance = 0;
tkMenuManager* tkMenuManager::instance ()
{
  if (!_instance)
    _instance = new tkMenuManager;
  return _instance;
}

tkMenuManager::tkMenuManager() {}
tkMenuManager::~tkMenuManager() { unregisterAllPages (); }

///////////////////////////////////////////////////////////////////////////

///////////////
// tkMenuBar //
//////////////
// Set up a pane with a single-line table on it.
tkMenuBar::tkMenuBar() : toolkit()
{
  console* con=console::instance();
  color* _c[4]; for (int a=3;a>=0;a--) _c[a]=con->sysColor(a);
  lmHint lh(center,center,lmHint::min,lmHint::min);
  frame::link(new lmEqual)
    ->link(new tkRaisedPanel)
    ->link(_tab=new lmTable(&lh,&lh,0,1));
}

void tkMenuBar::onClick() {}
frame* tkMenuBar::link(tkMenuPage* p, lmHint* cdef, lmHint* idef)
{
  p->setOwner(this); p->setAlign(1);
  return _tab->link((frame*)p,cdef,idef); 
}

///////////////////////////////////////////////////////////////////////////

/////////////////
// tkMenuPopup //
////////////////

tkMenuPopup::tkMenuPopup(int x, int y, int w, int h)
  : wmshell (x,y,w,h,0,true)
{
  console* con=console::instance();
  color* _c[4];
  for (int a=3;a>=0;a--) _c[a]=con->sysColor(a);
  lmHint cd(center,center,lmHint::max,lmHint::min);
  frame::link(new lmEqual)
    ->link(new tkRaisedPanel)
    ->link(_tab=new lmTable(&cd,&cd,1,0));
}

frame* tkMenuPopup::link(frame* f) { return _tab->link(f); }
frame* tkMenuPopup::link(frame* f,lmHint* hint) { return _tab->link(f,hint); }

///////////////////////////////////////////////////////////////////////////

////////////////
// tkMenuPage //
///////////////

// Triggers toplevel popup page tkMenuPopup
tkMenuPage::tkMenuPage(char* name) : tkMenuItemBase(name), _align(0)
{
  _page=new tkMenuPopup(0,0,8,8); _page->hide();
  console::instance()->link(_page);
}

// On click, move the according page to the right position and
// let the menu manager know about the new page so it can be closed
// if parent pages are opened.
void tkMenuPage::onClick()
{
  if (!_page) return;
  if (_page->isVisible()) { unregisterPagesOf(_owner); return; }

  dim* _tp=totalPosition();
  if (_align)
    _page->move(_tp->x(),_tp->y()+height());
  else _page->move(_tp->x()+width(),_tp->y());
  delete _tp;
  tkMenuManager::instance()->registerPage(_owner,_page);
}

frame* tkMenuPage::link(frame* f) { return _page->link(f); }
frame* tkMenuPage::link(frame* f,lmHint* hint) { return _page->link(f,hint); }

frame* tkMenuPage::link(tkMenuPage* p)
{
  p->setOwner(_page); return link((frame*)p);
}

///////////////////////////////////////////////////////////////////////////
tkMenuItem::tkMenuItem(char* name,void (*func)(frame*,void*))
  : tkMenuItemBase(name),_func(func),_data(0)
{}

tkMenuItem::tkMenuItem(char* name,void (*func)(frame*,void*),frame* from,void* data)
  : tkMenuItemBase(name),_func(func),_from(from),_data(data)
{}

void tkMenuItem::onClick()
{
  tkMenuManager::instance()->unregisterAllPages();
  if (_func)
    if (_from) (*_func)(_from,_data);
    else (*_func)(this,_data);
}

///////////////////////////////////////////////////////////////////////////
tkMenuItemBase::tkMenuItemBase(char* name) : tkClickLabel()
{
  console* con=console::instance();
  lmHint hint((gravity) 3,center,lmHint::min,lmHint::min);
  frame::link(new lmEqual(&hint))
    -> link(new stdtext(name,con->sysColor(0),con->sysColor(2)));
}

int tkMenuItemBase::minx() { return frame::minx()+8; }
int tkMenuItemBase::miny() { return frame::miny()+4; }

