// $Id: display.h,v 1.1.1.1 2000/09/30 13:42:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/display.h,v $
//--------------------------------------------------------------------------
// Connection to display
//
// (c)1998-1999 Sven Klose
//--------------------------------------------------------------------------

#ifndef _ptk_display_H
#define _ptk_display_H

class display {
friend class color;
friend class drawer;
friend class embed;
friend class font;
friend class image;
friend class window_drawer;

static display* _instance;

 public:
 display();
 virtual ~display();

 static display* instance();
 virtual void bell();
 virtual void bell(int ms);
 virtual void flush();
 void* drv();
};

#endif
