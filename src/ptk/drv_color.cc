// $Id: drv_color.cc,v 1.1.1.1 2000/09/30 13:42:10 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/x11/drv_color.cc,v $
//--------------------------------------------------------------------------
// Color management
//
// (c)1998-2000 Sven Klose
//--------------------------------------------------------------------------
// Allocate a color object for each color you need.

#include <stdlib.h>
#include "config.h"
#include "drv_color.h"
#include "drv_display.h"

// Allocate system color
drv_color::drv_color(int r,int g,int b)
{
  set(r,g,b);

#ifdef EMBED_X11WRAP
  Display* disp=x11_display();
  int screen_num=x11_screen();
  int depth=DefaultDepth(disp,screen_num);
  XColor clr;
  clr.red=r<<8;
  clr.green=g<<8;
  clr.blue=b<<8;

  // Black'n White display
  if (depth==1) {
#ifdef DEBUG
    cerr << "StaticGray display not supported.\n";
#endif
    exit(-1);
  }
  // TrueColor display
  if (depth>=16) {
    if (!XAllocColor(disp,DefaultColormap(disp,screen_num),&clr)) {
#ifdef DEBUG
      cerr << "Can't allocate color: All colorcells allocated and none matched.\n";
#endif
      exit(-1);
    }
    _pixel=clr.pixel;
  } else {
    unsigned long plane_masks[1];
    unsigned long col[1];
    if (!XAllocColorCells(disp,DefaultColormap(disp,screen_num),False,plane_masks,0,col,1)) {
#ifdef DEBUG
      cerr << "Couldn't allocate r/w color cell.\n";
#endif
      exit(-1);
    }

    _pixel=clr.pixel=col[0];
    clr.flags= DoRed | DoGreen | DoBlue;
    XStoreColor (disp,DefaultColormap(disp,screen_num),&clr);
  }
#endif
}
