// $Id: db_row_list.cc,v 1.1.1.1 2000/09/30 13:42:09 sven Exp $
// $Source: /home/cvsroot/ptk/programs/db/db_row_list.cc,v $
//--------------------------------------------------------------------------
// List of row editor with labeled border.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#include "frame.h"
#include "db_row_list.h"
#include "db_entry_list.h"

#include "tkTextButton.h"

db_row_list::db_row_list (dbi* _db, vector<dbForm*>* forms)
  : tkScrollField (), _forms (forms)
{
  lmHint hint (center, center, max, min);
  lmTable* tab;
  link (tab = new lmTable (&hint, 1, 2));
  _maintab = new lmTable (&hint,1,0);

  tkScrollFieldBase* topscr;
  lmTable* toptab;
  tab->link(topscr = new tkScrollField (false, false))
    ->link (toptab = new lmTable (&hint, 0, 1));
  topscr->clipAble (false);
  topscr->clipThis (true);

  tkTextButton* tb;
  for (vector<dbField*>::iterator i = _db->current_fields ()->begin ();
      i != _db->current_fields ()->end ();
      i++) {
    toptab->link (tb = new tkTextButton ((*i)->name (), 0, 0));
    int maxlen = (*i)->maxlength ();
    if (maxlen > 16)
      maxlen = 16;
    int m = stdtext::stdfont ()->maxWidth() * maxlen + 8 + 4;
    if (tb->minx () > m)
      m = tb->minx ();
    tb->resize (m, tb->miny ());
    tb->resizeable (false);
    tb->clipThis (false);
  }

  tkScrollFieldBase* mainscr;
  tab->link (mainscr = new tkScrollField (false, false))
    ->link (_maintab);
  mainscr->clipAble (false);
  mainscr->clipThis (false);

  lmTable* tmp;
  for (vector<dbForm*>::iterator i = _forms->begin();
       i != _forms->end ();
       i++) {
    _maintab->link (tmp = new db_entry_list ((*i)->entries ()));
    tmp->set_xref (toptab->child ());
  }
  clipChilds (false);

#ifdef DEBUG
  ofstream o("db_row.dump",ios::out); store_branch(o); o.close();
#endif
}

#ifdef DEBUG
void db_row_list::dump (ofstream& o)
{
  o << "::db_row_list";
  tkScrollField::dump (o);
}
#endif

void db_row_list::update ()
{
  tkScrollField::update ();
}
