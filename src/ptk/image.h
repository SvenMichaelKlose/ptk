// $Id: image.h,v 1.2 2001/05/04 23:56:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/image.h,v $
//--------------------------------------------------------------------------
// Image manipulation.
//
// (c)1999 Sven Klose
//--------------------------------------------------------------------------

#ifndef _ptk_core_image_H
#define _ptk_core_image_H

#include <string.h>

#include "dim.h"
#include "color.h"
#include "font.h"

//#include "drawer.h"

class image;

class pixel {
 public:
  int depth;
  int size;
  int Rloss;
  int Gloss;
  int Bloss;
  int Rshift;
  int Gshift;
  int Bshift;

  inline pixel () {}
  inline pixel (int d, int s,
	        int rl, int gl, int bl,
	        int rs, int gs, int bs)
    : depth (d), size (s),
      Rloss (rl), Gloss (gl), Bloss (bl),
      Rshift (rs), Gshift (gs), Bshift (bs) {}

  inline long Rmask () { return -1 >> Rloss << Rshift; }
  inline long Gmask () { return -1 >> Gloss << Gshift; }
  inline long Bmask () { return -1 >> Bloss << Bshift; }

  // Return value of color.
  inline long value (int r, int g, int b)
  {
    return (r >> Rloss << Rshift) |
      	   (g >> Gloss << Gshift) |
      	   (b >> Bloss << Bshift);
  }
};

class image {
  friend class window_drawer;
  void* img;
  pixel _format;
  int _width;
  int _height;
  char* _data;
  int _bytes_per_line;

 public:
  image (int width, int height);
  virtual ~image ();

  inline int width () { return _width; }
  inline int height () { return _height; }
  inline char* surface () { return _data; }
  inline pixel* format () { return &_format; }
  inline int pitch () { return _bytes_per_line; }
  inline char* surface (int x,int y, int bpp)
  {
    return surface () + (pitch () * y) + (bpp * x);
  }

  // Generic drawing functions //
  static inline void gen_set_pixel (char* i, long pixel, int bpp)
    {
      switch(bpp) {
      case 1:
	*i = (char) pixel;
	break;
      case 2:
	*(int*) i = (int) pixel;
	break;
      case 3:
	*(int*) i = (int) pixel;
	pixel >>= 16;
	i += 2;
	*i++ = (char) pixel;
	break;
      case 4:
	*(long*)i=pixel; break;
      }
    }

  static inline long gen_get_pixel (char* i, int bpp)
    {
      switch(bpp) {
      case 1:
	return (long) (char) *i;
      case 2:
	return (long) *(int*)i;
      case 3:
	return i[0] << 16 | i[1] << 8 | i[2];
      case 4:
	return *(long*)i;
      default:
	return 0;
      }
    }

  static inline void gen_set_pixel (image* i, int x, int y,
			            int r, int g, int b,
			       	    int bpp)
    {
      gen_set_pixel (i->surface (x, y, bpp),
      		     i->format ()->value (r, g, b),
		     bpp);
    }

  static inline void gen_set_pixel (char* i, color* c, int bpp)
    {
      gen_set_pixel (i, c->r (), c->g (), c->b (), bpp);
    }

  static inline void gen_set_pixel (char* i, int r, int g, int b, int bpp)
    {
      gen_set_pixel (i, r, g, b, bpp);
    }

  static inline void gen_rect (image* i, dim* d, color* c, int bpp = 0)
    {
      if (bpp == 0)
        bpp = i->format ()->size;
      
      char* p = i->surface (d->x (),d->y (), bpp);
      if (p == 0)
        return;
      int h = d->height ();
      int w2 = d-> width ();
      if (w2 == 0 || h == 0)
        return;

      int ofs = (i->pitch () - w2) * bpp;
      int w;
      do {
	w = w2;
	do {
	  gen_set_pixel (p, c, bpp);
	  p += bpp;
	} while (--w);
	p += ofs;
      } while (--h);
    }

  static inline void gen_blit (image* src, dim* d,
			       image* i, int xp, int yp,
			  	int srcbpp = 0, int bpp = 0)
    {
      if (bpp == 0)
        bpp = i->format ()->size;
      if (srcbpp == 0)
        srcbpp = src->format ()->size;

      char* p = i->surface (d->x (),d->y (),bpp);
      if (p == 0)
        return;
      if(d->width () == 0|| d->height() == 0)
        return;

      long is = i->pitch ();
      long sps = src->pitch ();
      int dw = d->width ();
      int dh = d->height ();
      int sofs = (sps - dw) * srcbpp;
      int ofs = (is - dw) * bpp;
      int w;
      char* sp = src->surface (d->x ()-xp, d->y () - yp, srcbpp);
      do {
	w = dw;
	do {
	    gen_set_pixel (p, gen_get_pixel (sp, srcbpp), bpp);
	  p += is;
	  sp += sps;
	} while (--w);
	p += ofs;
	sp += sofs;
      } while (--dh);
    }

  // Zeichnet Buchstaben
  // Parameter: d    = sichtbarer Bereich auf dem Bildschirm
  //            x,y  = Position auf dem Bildschirm
  //            chr  = ASCII-Code des Zeichens
  //            colF = Vordergrundfarbe
  //            colB = Hintergrundfarbe
  static inline void gen_putchar (image* i, dim* d,
				  int xp, int yp, char chr,
				  color* colF, color* colB,
				  font*, char* charset, int bpp)
    {
      if (!(d->height () && d->width ()))
        return;
      char* scrptr;
      if (!(scrptr = i->surface (d->x (), d->y (), bpp)))
        return;
      xp = d->x() - xp;
      yp = d->y() - yp;
      int ofs = (i->width () - d->width ()) * bpp;
      char* charptr = ((chr - 128) * 8) + charset + yp;
      int yc = d->height ();
      int xc2 = d->width ();
  
      long ps = i->format ()->size;
      do {
	char mask = 128 >> xp;
	int xc = xc2;
	char l = *charptr++;
	do {
	  if(l & mask)
	    gen_set_pixel (scrptr, colF, bpp);
	  scrptr += ps;
	  mask = mask >> 1;
	} while (--xc);
	scrptr += ofs;
      } while (--yc);
    }

  static inline void gen_putstring (image* i,dim* m,int xp,int yp,
				    char* str,
				    color* foreg, color* backg,
				    font* fnt,char* charset,
				    int bpp)
    {
      int cnt = strlen (str) - 1;
      int a = 0;
      dim* d;
      dim* d2;
      dim* d3;

      do {
	d3 = new dim (xp + a, yp, 8, 8);
	d=m->inside (d3);
	delete d3;
	d2 = d;
	if (d)
	  do {
	    gen_putchar (i, d, xp + a, yp, *str, foreg, backg, fnt, charset,bpp);
	  } while ((d = d->next ()));
	str++;
	a += 8;
	if (d2)
	  d2->clearAll ();
      } while (cnt--);
    }

  inline void set_pixel (int x, int y, int r, int g, int b, int bpp)
    {
      gen_set_pixel(surface(x,y,bpp),_format.value(r,g,b),bpp);
    }
};
#endif
