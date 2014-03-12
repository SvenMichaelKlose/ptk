// $Id: drv_image.cc,v 1.1.1.1 2000/09/30 13:42:10 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/x11/drv_image.cc,v $
//--------------------------------------------------------------------------
// image.cc -- Image object.
//
// (c) 1999 Sven Klose
//--------------------------------------------------------------------------

#include "image.h"
#include "drv_display.h"

image::image(int w,int h) : _width(w), _height(h)
{
#warning TODO: Implement MIT shared memory support for images
  Display* disp=x11_display();
  int scr=x11_screen();

  int depth=DefaultDepth(disp,scr);
  _format.size=depth/8;
  if (depth%8) _format.size++;
  _format.Rloss=_format.Gloss=_format.Bloss=0;
  int r=depth%3/2;
  _format.Rshift=depth/3*2+r;
  _format.Gshift=depth/3+r;
  _format.Bshift=0;

  EMSG("(Initalising drawable) ");
  /*  img.width = w;
  img.height = h;
  img.xoffset = w;
  img.format = ZPixmap;
  img.byte_order = MSBFirst;
  img.bitmap_unit = 8;
  img.bitmap_bit_order = MSBFirst;
  img.bitmap_pad = 8;
  img.depth = 24;
  img.bytes_per_line = w*3;
  img.bits_per_pixel = 24;
  img.red_mask = _format.Rmask(); //0xf00;
  img.green_mask = _format.Gmask(); //0xf0;
  img.blue_mask = _format.Bmask(); //0xf;
  */

#ifndef PTK_X11_USE_MITSHM
  bool _mitshm=false;

#else
  _mitshm=XShmQueryExtension(disp);
  if (_mitshm) {
    _shminfo.shmid=shmget(IPC_PRIVATE,h*w*_format.size,IPC_CREAT|0777);
    if (_shminfo.shmid<0) {
      cerr << "image: Can't get MIT shared memory id.\n"; _mitshm=false;
    } else {
      _shminfo.shmaddr=_data=(char*) shmat(_shminfo.shmid,0,0);
      _shminfo.readOnly=false;
      if (_shminfo.shmaddr==(char*)-1) {
	cerr << "image: Can't get MIT shared memory address.\n"; _mitshm=false; shmdt(_shminfo.shmaddr);
      } else {
	XShmAttach(disp,&_shminfo);
	shmctl(_shminfo.shmid, IPC_RMID, 0);
      }
    }
  }
#endif
  if (!_mitshm) _data = new char[w*h*_format.size];

  if (_mitshm) {
#ifdef PTK_X11_USE_MITSHM
    if (!(img=XShmCreateImage(disp,DefaultVisual(disp,scr),depth,ZPixmap,_data,&_shminfo,w,h))) {
      cerr << "Can't initialise drawable.\n";
      exit (-1);
    }
#endif
  } else {
    if (!(img=XCreateImage(disp,DefaultVisual(disp,scr),depth,ZPixmap,0,_data,w,h,depth,0))) {
      cerr << "Can't initialise drawable.\n";
      exit (-1);
    }
  }

  //  img->byte_order = MSBFirst;
  _bytes_per_line=img->bytes_per_line;

  char* i=surface();
  for (int a=h*pitch();a>0;a--) *i++=0;//(char)i;//0;
}

image::~image()
{
#warning FIXME: MIT shared memory not properly freed?
#ifdef PTK_X11_USE_MITSHM
  if (_mitshm) XShmAttach(x11_display(),&_shminfo);
#endif
  delete[] img->data;
}

// $Log: drv_image.cc,v $
// Revision 1.1.1.1  2000/09/30 13:42:10  sven
// ptk is a lightweight cross-platform user interface written in C++.
//
// Revision 1.1.1.1  2000/02/06 01:23:29  sven
// Let's go!
//
// Revision 1.1.1.1  2000/02/04 21:39:17  sven
// Let's go!
//
