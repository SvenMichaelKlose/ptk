// $Id: pencils.h,v 1.1.1.1 2000/09/30 13:42:10 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/pencils.h,v $
//--------------------------------------------------------------------------
// Drawing frames.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#ifndef _ptk_pencils_H
#define _ptk_pencils_H

#include "color.h"
#include "font.h"
#include "image.h"

class plane : public frame {
   color* _color;

 public:
   plane () : frame() {}
   plane (color* c, int x=0, int y=0, int w=0, int h=0);
   plane (dim*, color*);
   virtual ~plane() {}

   virtual void draw (dim*, int, int, drawer*);
   inline void setColor(color* c) {_color=c;}

#ifdef DEBUG
   virtual void dump(ofstream&);
#endif
};

class bitmap : public frame {
   image* _image;
   int _minWidth;
   int _minHeight;
   void defaults(int,int);

 public:
   bitmap(char*,int x=0,int y=0, int w=0, int h=0);
   bitmap(int x=0,int y=0,int w=0,int h=0);
   bitmap(char*,dim*);
   virtual ~bitmap();

   virtual void draw(dim*,int,int,drawer*);
   virtual int minx() {return _minWidth;}
   virtual int miny() {return _minHeight;}
   virtual void resize(int w,int h);
   inline image* get() { return _image; }
};

class stdtext: public frame {
  color* _foreground;
  color* _background;
  char* _textstring;

 public:
  stdtext (char*, color* f, color* b,int x=0,int y=0);
  virtual ~stdtext() {}

  virtual void draw (dim*, int,int,drawer*);
  virtual int width();
  virtual int height();
  virtual int minx();
  virtual int miny();
  static font* stdfont();
  inline char* name() { return _textstring; }
  inline void setString(char* str) { _textstring=str; }

#ifdef DEBUG
  virtual void dump(ofstream&);
#endif
};

class border : public frame {
  void subdraw(dim*,dim*,color*,drawer*);

 protected:
  color* _ct;
  color* _cr;
  color* _cb;
  color* _cl;

 public:
  border(color* ct=0, color* cr=0, color* cb=0, color* cl=0,
	 int x=0,int y=0,int w=0,int h=0);
  virtual ~border() {}

  virtual void resize(int width,int height);
  void setColors(color* ct, color* cr, color* cb, color* cl);
  virtual void draw (dim*,int,int,drawer*);

#ifdef DEBUG
  virtual void dump(ofstream&);
#endif
};

#endif
