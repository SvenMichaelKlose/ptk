// $Id: drv_windowdrawer.cc,v 1.1.1.1 2000/09/30 13:42:10 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/x11/drv_windowdrawer.cc,v $
//--------------------------------------------------------------------------
// windowdrawer.cc -- Window drawer to embed ptk into X11 window managers
//
// (c) 1998-1999 Sven Klose
//--------------------------------------------------------------------------
// Does a window manager shell as well.

#include "ptk.h"
#include "color.h"
#include "display.h"
#include "image.h"
#include "windowdrawer.h"

#include "drv_display.h"

#ifndef EMBED_ANYX11
#error EMBED_ANYX11 not set!
#endif

window_drawer::window_drawer(char* name,int xp,int yp, int w, int h,
			     bool popup,color* bgcolor)
  : drawer(name,xp,yp,w,h)
{
  Display* disp=x11_display();
  int screen_num=x11_screen();

  // Set up _display
  border_width=0;//4;
  window_name=icon_name=name;

  // seek visual
#ifdef EMBED_X11
  XVisualInfo vTemplate;
  XVisualInfo *visualList;
  int visualsMatched;

  vTemplate.screen=screen_num;
  vTemplate.depth=24;
  //vTemplate.class=DirectColor;
  visualList=XGetVisualInfo (disp,
			     VisualScreenMask |
			     VisualDepthMask, //|
			     //VisualClassMask,
			     &vTemplate, &visualsMatched
			     );
  EMSG("(");EMSG(visualsMatched);EMSG(" visuals matched) ");
  if (!visualsMatched)
    {
      cerr << "No visuals matched.\n";
      exit(-1);
    }
#endif

  // Create window
  unsigned long wincolor; int value_mask=0;
  XSetWindowAttributes xattr;
  if (bgcolor) {
    wincolor=bgcolor->pixel(); 
    value_mask|=CWBackPixel | CWBorderPixel; // | CWEventMask;
    xattr.background_pixel=wincolor;
  }
  if (popup) {
    xattr.override_redirect=true;
    value_mask|=CWOverrideRedirect;
  }
  _win = XCreateWindow (disp, RootWindow(disp,screen_num),
			xp,yp,w,h,border_width,
			DefaultDepth(disp,screen_num),
			NULL,NULL,value_mask,&xattr
			);

  if (!(size_hints=XAllocSizeHints())) {
    cerr << "Out of memory while trying to allocate size hints.\n";
    exit(-1);
  }

  if (!(wm_hints=XAllocWMHints())) {
    cerr << "Out of memory while trying to allocate wm hints.\n";
    exit(-1);
  }

  if (!(class_hints=XAllocClassHint())) {
    cerr << "Out of memory while trying to allocate class hints.\n";
    exit(-1);
  }

  size_hints->flags = PSize | PMinSize;
  if (popup)
    size_hints->flags |= PPosition;
  size_hints->min_width = w;
  size_hints->min_height = h;

  if (XStringListToTextProperty(&window_name, 1,&windowName)==0) {
    cerr << "Structure allocation for windowName failed.\n";
    exit(-1);
  }
  if (XStringListToTextProperty(&icon_name, 1,&iconName)==0) {
    cerr << "Structure allocation for windowName failed.\n";
    exit(-1);
  }
 
  class_hints -> res_name=name;
  class_hints -> res_class=name;

  int argc=0;
  char** argv=0;

  XSetWMProperties(disp,_win,&windowName,&iconName,argv,argc,
		   size_hints, wm_hints, class_hints);
  XSelectInput(disp, _win,
  	       ExposureMask
	       | ButtonPressMask | ButtonReleaseMask | PointerMotionMask
	       | KeyPressMask | KeyReleaseMask
	       | StructureNotifyMask);
  valuemask=0;
  _gc = XCreateGC(disp,_win,valuemask,&values);

#ifdef EMBED_X11
  EMSG("(Initalising drawable) ");
  img.width = w;
  img.height = h;
  img.xoffset = w;
  img.format = ZPixmap;
  img.data = new char[w*h*3];
  img.byte_order = LSBFirst;
  img.bitmap_unit = 8;
  img.bitmap_bit_order = MSBFirst;
  img.bitmap_pad = 8;
  img.depth = 24;
  img.bytes_per_line = w*3;
  img.bits_per_pixel = 24;
  img.red_mask = 0xf00;
  img.green_mask = 0xf0;
  img.blue_mask = 0xf;

  if (!XInitImage(&img)) {
    cerr << "Can't initialise drawable.\n";
    exit (-1);
  }

  unsigned char *p = (unsigned char*) img.data;
  for (int a=w*h*3;a>0;a--) *p++=196;
#endif
}

window_drawer::~window_drawer()
{
  Display* disp=x11_display();
  XFreeGC(disp,_gc);
  XDestroyWindow (disp,_win);
  XFree (wm_hints);
  XFree (size_hints);
  XFree (class_hints);
}

void window_drawer::draw(dim* d)
{
  if (!d) return;
#ifdef EMBED_X11
  int xd;
  int yd;
  int w;
  int h;
  Display* disp=x11_display();

  do {
    xd=d->x();
    yd=d->y();
    w=d->width();
    h=d->height();
#ifdef EXCESSIVE_LOGGING
    cout << "window_drawer::draw: " << xd << " " << yd
	 << " " << w << " " << h << "\n";
#endif
    XPutImage (disp,_win,_gc,&img,xd,yd,xd,yd,w,h);
  } while ((d=d->next()));
#endif
#ifdef EMBED_X11WRAP
  d=d;
#endif
}

///////////////////
// EMBED_X11WRAP //
//////////////////
#ifdef EMBED_X11WRAP
void window_drawer::rect(dim* d,color* col)
{
  Display* disp=x11_display();

  XSetForeground(disp,_gc,col->pixel());
  XFillRectangle(disp,_win,_gc,d->x(),d->y(),d->width(),d->height());
}

void window_drawer::bmp (dim* d, int xt, int yt, int ow, int xm,image* src)
{
  XPutImage (x11_display(), _win, _gc,
	     src->img,
	     d->x()-xt,  d->y()-yt,
	     d->x(),     d->y(),
	     d->width(), d->height()
	     );
}

void window_drawer::copy(int xs,int ys,int xd,int yd,int w,int h)
{
  XCopyArea(x11_display(),_win,_win,_gc,xs,ys,w,h,xd,yd);
}

void window_drawer::putchar(dim* d,int xp,int yp,char chr,color* colF,color* colB,font*)
{
}

#warning TODO: putstring(): plain/transparent mode
void window_drawer::putstring(dim* m,int xp,int yp,char* str,
		       color* foreg,color* backg,font* fnt)
{
  Display* disp=x11_display();
  //  XClipRect(m->x(),m->y(),m->width(),m->height());
  XSetFont(disp,_gc,fnt->fontInfo()->fid);
  XSetForeground(disp,_gc,foreg->pixel());
  while(m) {
    _XRegion* reg=XCreateRegion();
    XRectangle rec={m->x(),m->y(),m->width(),m->height()};
    XUnionRectWithRegion(&rec,reg,reg);
    XSetRegion(disp,_gc,reg);
    XDrawString(disp,_win,_gc,xp,yp,str,strlen(str));
    XDestroyRegion(reg);
    m=m->next();
  }
  _XRegion* reg=XCreateRegion();
  XRectangle rec={0,0,32000,32000};
  XUnionRectWithRegion(&rec,reg,reg);
  XSetRegion(disp,_gc,reg);
  XDestroyRegion(reg);
}

#endif

void window_drawer::move(int x, int y)
{
  XMoveWindow(x11_display(),_win,x,y);
}

void window_drawer::resize(int w,int h)
{
  XResizeWindow(x11_display(),_win,w,h);
}

// End of EMBED_ANYX11

void window_drawer::show() { XMapWindow(x11_display(),_win); }
void window_drawer::hide() { XUnmapWindow(x11_display(),_win); }

int window_drawer::x()
{
  XWindowAttributes attr;
  XGetWindowAttributes(x11_display(),_win,&attr);
  return attr.x;
}

int window_drawer::y()
{
  XWindowAttributes attr;
  XGetWindowAttributes(x11_display(),_win,&attr);
  return attr.y;
}

  /*
  img.width = w;
  img.height = h;
  img.xoffset = w;
  img.format = XYPixmap; //ZPixmap;
  img.data = new char[w*h*3];
  img.byte_order = MSBFirst;
  img.bitmap_unit = 8;
  img.bitmap_bit_order = LSBFirst;
  img.bitmap_pad = 8;
  img.depth = 24;
  img.bytes_per_line = w/8; //w*3;
*/
  /*
  int c=8;
  for (int a=480;a>0;a--) {
    int b;
    //    for (b=640;b>0;b--) *p++=0xff;
    for (b=640;b>0;b--)
      {
	*p++=(unsigned char) (b/3);
	*p++=(unsigned char) (255-b/3);
	*p++=(unsigned char) (a/2);
      }
    //  if (!--c) {c=8;draw(0,0,0);}
  }
*/

// $Log: drv_windowdrawer.cc,v $
// Revision 1.1.1.1  2000/09/30 13:42:10  sven
// ptk is a lightweight cross-platform user interface written in C++.
//
// Revision 1.1.1.1  2000/02/06 01:23:29  sven
// Let's go!
//
// Revision 1.1.1.1  2000/02/04 21:39:17  sven
// Let's go!
//
