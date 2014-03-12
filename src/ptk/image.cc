// $Id: image.cc,v 1.2 2001/05/04 23:56:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/image.cc,v $
//--------------------------------------------------------------------------
// Image manipulation.
//
// (c) 1999 Sven Klose
//--------------------------------------------------------------------------

#include "config.h"
#include "image.h"
#include "drv_display.h"
#include <stdlib.h>
#include <string.h>

#ifdef EMBED_ANYX11
#include <X11/Xlib.h>
#ifdef PTK_X11_USE_MITSHM
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>
#endif
#endif

// This belongs inside class decl.
#ifdef EMBED_ANYX11
#ifdef PTK_X11_USE_MITSHM
  XShmSegmentInfo _shminfo; 
  bool _mitshm;
#endif
#endif

#warning TODO: Implement MIT shared memory support for images
image::image (int w, int h) : _width (w), _height (h)
{
  Display* disp = x11_display ();
  int scr = x11_screen ();

  int depth = DefaultDepth (disp, scr);
  _format.size = depth / 8;
  if (depth % 8)
    _format.size++;
  _format.Rloss = _format.Gloss = _format.Bloss = 0;
  int r = depth % 3 / 2;
  _format.Rshift = depth / 3 * 2 + r;
  _format.Gshift = depth / 3 + r;
  _format.Bshift = 0;

  EMSG ("(Initalising drawable) ");

#ifndef PTK_X11_USE_MITSHM
  bool _mitshm = false;

#else
  _mitshm = XShmQueryExtension (disp);
  if (_mitshm) {
    _shminfo.shmid = shmget (IPC_PRIVATE, h * w * _format.size, IPC_CREAT | 0777);
    if (_shminfo.shmid < 0) {
#ifdef DEBUG
      cerr << "image: Can't get MIT shared memory id.\n";
      _mitshm = false;
#endif
    } else {
      _shminfo.shmaddr = _data = (char*) shmat (_shminfo.shmid, 0, 0);
      _shminfo.readOnly = false;
      if (_shminfo.shmaddr == (char*) -1) {
#ifdef DEBUG
	cerr << "image: Can't get MIT shared memory address.\n"; _mitshm=false; shmdt(_shminfo.shmaddr);
#endif
      } else {
	XShmAttach (disp, &_shminfo);
	shmctl(_shminfo.shmid, IPC_RMID, 0);
      }
    }
  }
#endif
  if (!_mitshm)
    _data = new char[w*h*_format.size];

  if (_mitshm) {
#ifdef PTK_X11_USE_MITSHM
    if (!(img = XShmCreateImage (disp,DefaultVisual (disp,scr), depth, ZPixmap, _data, &_shminfo, w, h))) {
#ifdef DEBUG
      cerr << "Can't initialise drawable.\n";
#endif
      exit (-1);
    }
#endif
  } else {
    if (!(img = XCreateImage (disp, DefaultVisual (disp, scr), depth, ZPixmap, 0, _data, w, h, depth, _format.size))) {
#ifdef DEBUG
      cerr << "Can't initialise drawable.\n";
#endif
      exit (-1);
    }
  }

  //  img->byte_order = MSBFirst;
  _bytes_per_line = ((XImage*)img)->bytes_per_line;

  memset (surface (), 0, h * pitch ());
}

image::~image()
{
#warning FIXME: MIT shared memory not properly freed?
#ifdef PTK_X11_USE_MITSHM
  if (_mitshm)
    XShmAttach(x11_display(),&_shminfo);
#endif

  // Free image bitmap.
  delete[] ((XImage*)img)->data;
}
