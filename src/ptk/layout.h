// $Id: layout.h,v 1.2 2001/05/04 23:56:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/layout.h,v $
//--------------------------------------------------------------------------
// Automatic layout.
//
// (c) 1998-2000 Sven Klose
//--------------------------------------------------------------------------

#ifndef _ptk_layout_H
#define _ptk_layout_H

enum metric {
  pixel,
  perc   = 0x8000 // Take value as percentage of calculated size
};

// Size attributes
enum size {
  min    = 0x1000,    // mininum size
  max    = 0x2000,    // maximum size
  equal  = 0x3000,  // equal to any other dim (defined by layout classes)
  preset = 0x4000  // don't change the current size
};

// Positioning
enum gravity {
  center = 0x1000,
  top    = 0x2000,
  bottom = 0x3000,
  left   = 0x2000,
  right  = 0x3000
};


class lmHint {
 private:
   int pt[4];

 public:
   inline int flag(int a) {return a&0xf000;}
   inline int  val(int a) {return a&0x0fff;}

   inline lmHint() {}
   inline lmHint(lmHint* d)
   {
		pt[0] = d->pt[0];
		pt[1] = d->pt[1];
		pt[2] = d->pt[2];
		pt[3] = d->pt[3];
   }
   inline lmHint(gravity x, gravity y, size w, size h) {
     pt[0]=x; pt[1]=y; pt[2]=w+x; pt[3]=h+y;
   }

   // Getting and setting coordinates
   inline int x() {return pt[0];}
   inline int y() {return pt[1];}
   inline int width() {return pt[2]-pt[0];}
   inline int height(){return pt[3]-pt[1];}
};

////////////
// layout //
///////////
// Caches min. sizes and invokes update on tree changes and or if
// the current size is unknown or is changed
class layout : public frame {
  void defaults();

 protected:
  int getSize(int oldval, int val, int attr, int chmin);
  int getMinSize(int oldval, int attr, int chmin);
  int getPosition(int chmin, int chsize, int size, int attr);
  int getPercent(int val,  int attr);

  int minWidth;     // bevorzugte Mindestgr”áe des Elements
  int minHeight;

 public:
  layout() : frame(0,0,0,0) {defaults();}
  layout(dim* d) : frame(d->x(),d->y(),d->width(),d->height()) {defaults();}
  layout(int x,int y,int w,int h) : frame(val(x),val(y),val(w),val(h))
    {defaults();}
  virtual ~layout() {}

#ifdef DEBUG
  virtual void dump(ofstream&);
#endif
};

////////////
// lmCell //
///////////
class lmCell : public layout {
  lmHint* _cell_def;
  lmHint* _item_def;
 protected:
  friend class lmTable;
  lmHint* getCellDef() { return _cell_def; }
 public:
  lmCell(lmHint* cell_hint, lmHint* child_hint);
  virtual ~lmCell() {}

  virtual void update();
  virtual int minx();
  virtual int miny();

#ifdef DEBUG
  virtual void dump(ofstream&);
#endif
};

/////////////
// lmEqual //
////////////
class lmEqual : public layout
{
 private:
  lmHint* _item_def;
  void defaults();

 public:
  lmEqual();
  lmEqual(lmHint*);
  virtual ~lmEqual() {}

  virtual int minx();
  virtual int miny();
  virtual void update();
#ifdef DEBUG
  virtual void dump(ofstream&);
#endif
};

/////////////
// lmTable //
////////////
class lmTable : public layout {
  lmHint* _cell_def;
  lmHint* _item_def;
  dim* _xref;
  dim* _yref;
  void defaults(lmHint* celldef, lmHint* itemdef);
  lmCell* createCell(lmHint* celldef, lmHint* itemdef);
  int xcnt;
  int ycnt;
  
 protected:
  void getmins(bool do_update);

 public:
  virtual void update();
  lmTable();
  lmTable(int nx, int ny);
  lmTable(lmHint* cell_and_item_def, int nx, int ny);
  lmTable(lmHint* cell_def, lmHint* item_def, int nx, int ny);
  lmTable(lmHint* cell_def, lmHint* item_def, int nx, int ny,dim* total);
  virtual ~lmTable() {}

  virtual frame* link(frame*);
  virtual frame* link(frame* f, lmHint* cell_def, lmHint* item_def);
  virtual frame* link(frame* f, lmHint* cell_and_item_def)
    { return link(f,cell_and_item_def,cell_and_item_def); }

  virtual int minx();
  virtual int miny();

  // Fixed size column widths or row heights
  inline void set_xref(dim* list) { _xref=list; }
  inline void set_yref(dim* list) { _yref=list; };
  
  static lmHint* std();

#ifdef DEBUG
  virtual void dump(ofstream&);
#endif
};

//////////////////////////////
// lmFlow - Flow layout box //
/////////////////////////////

class lmFlow : public frame {
  public:
    lmFlow () : frame () {}
    virtual void update ();
    virtual int minx () {return 0;}
    virtual int miny () {return 0;}

  private:
    void reconnect (frame*) {}
    frame* append_following_content () { return 0; }
    frame* _content;
    int _left_border;
    int _right_border;
    int _top_border;
    int _bottom_border;
    int _minxdist;
    int _minydist;
    lmFlow* _previous_box;
    lmFlow* _next_box;
};
#endif
