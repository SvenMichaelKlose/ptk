// $Id: tkButton.h,v 1.2 2001/05/04 23:56:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptktoolkit/tkButton.h,v $
//--------------------------------------------------------------------------
// Button widget
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#ifndef ptk_toolkit_tkButton
#define ptk_toolkit_tkButton

#include "layout.h"
#include "tkPanel.h"

class tkButton: public border {
  tkPanel* _panel;
  lmEqual* _connection;

 public:
  tkButton(frame* content=0,int x=0,int y=0,int w=0,int h=0);
  virtual ~tkButton() {}

  virtual frame* link (frame*);
  void setColor (color* c);
};

#endif
