// $Id: dim.cc,v 1.2 2001/02/03 07:12:31 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/dim.cc,v $
//--------------------------------------------------------------------------
// Handles rectangular areas in a list tree.
//
// (c) 1997-1999 Sven Klose
//--------------------------------------------------------------------------

#warning Remove hint enum from dim or layout!

#include"dim.h"

//#define DEBUG_DIM_CLIP
//#define DEBUG_DIM_INSIDE
//#define DEBUG_DIM_RESIZE

// Returns true if d hits this.
bool dim::hits (dim* d)
{
    if (!d)
        return false;
    if (
        (d->pt[0] <= pt[2]) &&
        (d->pt[2] >= pt[0]) &&
        (d->pt[1] <= pt[3]) &&
        (d->pt[3] >= pt[1])
       )
        return true;
    return false;
}

bool dim::isWithin (dim* c)
{
    if (!c)
        return false;
    if ((c->pt[0] <= pt[0]) &&
         (pt[2] <= c->pt[2]) &&
         (c->pt[1] <= pt[1]) &&
         (pt[3] <= c->pt[3]))
        return true;
    return false;
}  

void dim::move (int a, int b)
{
    pt[2] = a + pt[2] - pt[0];
    pt[3] = b + pt[3] - pt[1];
    pt[0] = a;
    pt[1] = b;
}

void dim::resize (int w, int h)
{
    if (w < 0) {
        w *= -1;
#ifdef DEBUG_DIM_RESIZE
        cerr << "dim::resize: Negative width! Corrected signedness.\n";
#endif
    }
    if (h<0) {
        h *= -1;
#ifdef DEBUG_DIM_RESIZE
        cerr << "dim::resize: Negative height! Corrected signedness.\n";
#endif
    }
    pt[2] = w + pt[0];
    pt[3] = h + pt[1];
}

void dim::moveresize (int a, int b, int c, int d)
{
    move (a, b);
    resize (c, d);
}

void dim::setdim (dim* d)
{
    move (d->x (), d->y ());
    resize (d->width (), d->height ());
}

//////////////
// Clipping //
/////////////

dim* dim::split (int edge, dim* c)
{
    dim* n = new dim (this);
    int a = c->pt[edge];
    pt[edge] = a;
    n->pt[(edge + 2) & 3] = a;
    return n;
}
  
// This methods check if an edge of a rectangle 'c' overlaps 'this'
// rectangle. If so, it is split and the visible part is returned.
// If not, 0 is returned.
// There're different methods for each side.
dim* dim::split0 (dim* c)
{
    if ((pt[1] <= c->pt[1]) &&
        (c->pt[1] <= pt[3]) &&
        (pt[0] <= c->pt[2]) &&
        (pt[2] >= c->pt[0]))
        return split (1, c);
    return 0;
}

dim* dim::split1 (dim* c)
{
    if ((pt[2] >= c->pt[2]) &&
        (c->pt[2] >= pt[0]) &&
        (pt[1] <= c->pt[3]) &&
        (pt[3] >= c->pt[1]))
      return split (2, c);
    return 0;
}

dim* dim::split2 (dim* c)
{
    if ((pt[3] >= c->pt[3]) &&
        (c->pt[3] >= pt[1]) &&
        (pt[2] >= c->pt[0]) &&
        (pt[0] <= c->pt[2]))
      return split (3, c);
    return 0;
}

dim* dim::split3(dim* c)
{
    if ((pt[0] <= c->pt[0]) &&
        (c->pt[0] <= pt[2]) &&
        (pt[3] >= c->pt[1]) &&
        (pt[1] <= c->pt[3]))
      return split (0, c);
    return 0;
}

dim* dim::clipBase (dim* c)
{
    // A case which the split function can't detect is if 'this' completely
    // lies inside 'c', so we need an extra check.
    if (isWithin (c))
        return 0;
  
    dim* n = split0 (c);
    n->add (split2 (c));
    n->add (split1 (c));
    n->add (split3 (c));
//    if (dimisempty (n))
 //       n = new dim (this); // Areas don't overlap, return original.
    return cleanup (n);
}

// Calls split functions which will truncate the areas.
// (oh dear)
dim* dim::insideBase (dim* c)
{
    if (!this) return 0;

    dim* n = new dim (this);
    if ((c->pt[0] <= pt[0]) &&
        (pt[2] <= c->pt[2]) &&
        (c->pt[1] <= pt[1]) &&
        (pt[3] <= c->pt[3]))
        return n; // This area is already within c.

    if (
        ((c->pt[0] < pt[0]) &&
         (c->pt[2] < pt[0]))
        ||
        ((c->pt[2] > pt[2]) &&
         (c->pt[0] > pt[2]))
        ||
        ((c->pt[1] < pt[1]) &&
         (c->pt[3] < pt[1]))
        ||
        ((c->pt[3] > pt[3]) &&
         (c->pt[1] > pt[3]))
        ) {
        n->clear ();
        return 0;
    }

    dim* a; // = new dim ();
    a = n->split0 (c);
    if (a) a->clearAll ();
    a = n->split2 (c);
    if (a) a->clearAll ();
    a = n->split1 (c);
    if (a) a->clearAll ();
    a = n->split3 (c);
    if (a) a->clearAll ();

    return cleanup (n);
}

// Returns this areas inside mask
dim* dim::inside (dim* mask)
{
#ifdef DEBUG_DIM_INSIDE
    cout << "dim::inside(dim* mask) - this:\n";
    tree::dumplist ();
    cout << "dim::inside(dim* mask) - mask:\n";
    mask->tree::dumplist ();
#endif
    if (!mask)
        return 0;
    dim* clipped = new dim ();
    dim* toclip;

    do {
        toclip = this; // Iterate over all in cpy
        do {
	    clipped->add (toclip->insideBase (mask));
        } while ((toclip = toclip->next ()));
    } while ((mask = mask->next ()));

#ifdef DEBUG_DIM_INSIDE
    cout << "dim::inside() returns:\n";
    clipped->tree::dumplist ();
#endif

    return cleanup (clipped);
}

dim* dim::clip (dim* mask)
{
    if (!mask)
        return duplicate ();
#ifdef DEBUG_DIM_CLIP
    cout << "dim::clip(dim* mask) - this:\n";
    tree::dumplist ();
    cout << "dim::clip(dim* mask) - mask:\n";
    mask->tree::dumplist ();
#endif

    dim* clipped = new dim ();
    dim* cpy = this->duplicate ();

    do {
        dim* toclip = cpy;
        clipped->clearAll();
        do {
//            if (toclip->isempty ())
 //               break; //??? check
	    clipped->add (toclip->clipBase (mask));
        } while ((toclip = toclip->next ()));

        cpy->clearAll ();
        cpy = clipped;
    } while ((mask = mask->next ()));
#ifdef DEBUG_DIM_CLIP
    cout << "dim::clip() returns:\n";
    cpy->tree::dumplist ();
#endif
    return cleanup (cpy);
}

// Inverts and returns this mask
dim* dim::hole ()
{
    int xp = x ();
    int yp = y ();
    int w = width ();
    int h = height ();

    dim* n = new dim ();
    // 320,200 -> width(),height()
    if (yp >= 0)
        n->add (new dim (0, 0, 32000, yp));
    if (xp >= 0)
        n->add (new dim (0, yp, xp, h));
    if ((xp + w) < 32000)
    	n->add (new dim (xp + w, yp, 32000 - xp - w, h));
    if ((yp + h) < 20000)
     	n->add (new dim (0, yp + h, 32000, 20000 - yp - h));
    return n;
}

// Creates a copy of this list.
// Does NOT care about childs.
dim* dim::duplicate ()
{
//    if (!this)
 //       return 0;
    dim* d = this;
    dim* n = new dim (d);
    while (( d = d->next ()))
        n->add (new dim (d));
    return n;
}

// Delete frames of no size and invalid frames
dim* dim::cleanup (dim* n)
{
    if (!n)
        return 0;

    dim* i;
    dim* i2 = n;
    do {
        i = i2;
        i2 = i2->next ();
        if ((i->pt[0] >= i->pt[2]) ||
	    (i->pt[1] >= i->pt[3])) {
            if (i == n)
                n=i2;
            i->clear ();
        }
    } while (i2);

    return n;
}

bool _xytestless (dim* i, dim* j, int (dim::* cmp) ())
{
    return ((i->*cmp) () < (j->*cmp) ());
}

bool _xytestgreater (dim* i, dim* j, int (dim::* cmp) ())
{
    return ((i->*cmp) () > (j->*cmp) ());
}

dim* _xysort3 (dim* list,
		      bool (*cmp) (dim*, dim*, int (dim::*) ()),
		      int (dim::* pos) ())
{
    dim* i = list;
    dim* j;
    dim* tmp;
    while ((j = i->next ())) {
        do {
            if ((*cmp) (i, j, pos) == 0)
		continue;
            i->xchg (j);
            tmp = j;
            j = i;
            i = tmp;
            if (list == j)
                list = i;
        } while ((j = j->next ()));

        if (!(i = i->next ()))
            break;
    }
    return list;
}

dim* _xysort2 (dim* list, int dir, int (dim::* cmp) ())
{
    if (dir < 0)
        return _xysort3 (list, &_xytestless, cmp);
    else
        return _xysort3 (list, &_xytestgreater, cmp);
}

dim* dim::xysort (dim* list, int xdir, int ydir)
{
    return _xysort2 (_xysort2 (list, xdir, &dim::x), ydir, &dim::y);
}

dim* dim::totalPosition ()
{
    dim* d;

    if (parent ())
        d = parent ()->totalPosition ();
    else
        d = new dim (this);

    d->moveresize (d->x () + x (), d->y () + y (), width (), height ());

    return d;
}

#ifdef DEBUG
void dim::dump (ofstream& o)
{
    tree::dump (o);
    o << " " << pt[0] << " " << pt[1]<< " " << pt[2] << " " << pt[3];
}
#endif
