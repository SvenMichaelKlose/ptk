// $Id: stdtext.h,v 1.1 2001/05/05 00:00:58 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/stdtext.h,v $
//--------------------------------------------------------------------------
// Drawing frames.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#ifndef _ptk_stdtext_H
#define _ptk_stdtext_H

class color;
class font;

class stdtext: public frame {
  color* _foreground;
  color* _background;
  char* _textstring;

 public:
  stdtext (char*, color* f, color* b, int x = 0, int y = 0);
  virtual ~stdtext() {}

  virtual void draw (dim*, int, int, drawer*);
  virtual int width ();
  virtual int height ();
  virtual int minx ();
  virtual int miny ();
  static font* stdfont ();
  inline char* name () { return _textstring; }
  inline void setString (char* str) { _textstring=str; }

#ifdef DEBUG
  virtual void dump (ofstream&);
#endif
};

#endif
