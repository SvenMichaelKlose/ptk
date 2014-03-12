// ptk - screen interfaces
//
// Accesses the screen
//
// (c)1998-1999 Sven Klose

#include "config.h"
#include "console.h"
#include "drawer.h"

///////////////////
// X11 interface //
//////////////////

#ifdef EMBED_ANYX11

#include <fstream.h>



void screen::set_output_drawer (void *i)
{
#ifdef EMBED_ANYX11
 if (i)
   {
     _canvas=i;
     xm=((drawer*)_canvas)->width();
     ym=((drawer*)_canvas)->height();
   }
#endif
}

///////////////
// EMBED_X11 //
//////////////
#ifdef EMBED_X11
inline char* screen::getAdr(int x,int y)
{
  return (char*) (y*((drawer*)_canvas)->img.bytes_per_line+x*3+((drawer*)_canvas)->img.data);
}

void screen::rect(dim* d,color* col)	// Rechteck fuellen
{
  char* p=getAdr(d->x(),d->y());
  if (!p) return;
  int h=d->height();
  int w2=d->width();
  if (!w2||!h) return; // Ende, Fl„che besitzt keine Ausdehnung

  int ofs=(xm-w2)*3;
  int w;
  do {
    w=w2;
    do {
      *p++=col->r();
      *p++=col->g();
      *p++=col->b();
    } while(--w);
    p+=ofs;
  } while(--h);
}


void screen::bmp (dim* d, int xp, int yp, int ow, int xm, unsigned short* src)
{
  char* p=getAdr(d->x(),d->y());
  if (!p) return;
  if(!d->width()||
     !d->height()) return; // Ende, Flaeche besitzt keine Ausdehnung

  int dw=d->width();
  int dh=d->height();
  int sofs=(ow-dw);//*2;
  int ofs=(xm-dw)*3;
  int w;
  unsigned short c;
  unsigned short* sp=(d->x()-xp)*2+((d->y()-yp)*ow)+src;
  do {
    w=dw;
    do {
      c=*sp++;
      *p++=(unsigned char) ((c&0xfc00)>>8);
      *p++=(unsigned char) ((c&0x03f0)>>2);
      *p++=(unsigned char) ((c&0x000f)<<4);
    } while(--w);
    p+=ofs;
    sp+=sofs;
  } while(--dh);
}

// Zeichnet Buchstaben
// Parameter: d    = sichtbarer Bereich auf dem Bildschirm
//            x,y  = Position auf dem Bildschirm
//            chr  = ASCII-Code des Zeichens
//            colF = Vordergrundfarbe
//            colB = Hintergrundfarbe
void screen::putchar(dim* d,int xp,int yp,char chr,color* colF,color* colB,font*)
{
  if (!(d->height()&&d->width()))
    return;    // Ende, Bereich besitzt keine Ausdehnung

  char* scrptr=getAdr(d->x(),d->y());
  if (!scrptr) return;

  // Offset innerhalb des Zeichens bestimmen
  xp=d->x()-xp;
  yp=d->y()-yp;
  int ofs=(xm-d->width())*3; // Sprung zur n„chsten Bildschirmzeile
  
  // Zeiger auf erste Zeile des Zeichens
  char* charptr=((chr-128)*8)+charset8x8+yp;
  int yc=d->height(); // H”he des Ausschnitts
  color* c;
  int xc2=d->width();  // Breite des Ausschnitts
  
  do {
    char mask=128>>xp;  // Maske fr ersten Pixel in Zeile
    int xc=xc2;
    char l=*charptr++;  // Aktuelle Zeile auslesen
    c=colF;
    do {
      if(l&mask)      // Farbe des aktuellen Punktes bestimmen...
	{
	  //      else
	  // c=colB;
	  *scrptr++=c->r();    // ...und in den Bildschirm schreiben
	  *scrptr++=c->g();
	  *scrptr++=c->b();
	}
      else
	scrptr+=3;
      mask=mask>>1;   // N„chster Pixel in der Zeile
    } while(--xc);
    scrptr+=ofs;            // Zeiger auf n„chste Zeile
  } while(--yc);
}

void screen::putstring(dim* m,int xp,int yp,char* str,
		       color* foreg,color* backg,font* fnt)
{
  int cnt=strlen(str)-1; // Anzahl der Zeichen
  int a=0;
  dim* d;
  dim* d2;
  dim* d3;

  do {
    d3=new dim(xp+a,yp,8,8); // Sichtbaren Bereich des aktuellen
    // Zeichens bestimmen
    d=m->inside(d3);	// und mit der Hauptmaske clippen
    delete d3;
    d2=d;
    if (d)
      do {    // šbrige Bereiche dieses Buchstabens zeichnen
	putchar(d,xp+a,yp,*str,
		foreg,backg,fnt);
      } while((d=d->next()));
    str++;a+=8;
    if (d2) d2->clearAll();
  } while(cnt--);
}

#endif

//--------8<----------8<--------8<---------8<----

//////////////////////////////////////
// VGA mode 13h screen under DJGPP  //
/////////////////////////////////////


void screen::rect(dim* d,int col)	// Rechteck fllen
{
#ifdef DJGPP
  unsigned long p=getAdr(d->x(),d->y());
#else
  uchar far* p=MK_FPTR(0xa000,getAdr(d->x(),d->y()));
#endif
  int h=d->height();
  int w2=d->width();
  if(!w2||!h) return; // Ende, Fl„che besitzt keine Ausdehnung
  
  int ofs=xm-w2;
  int w;
  do {
    w=w2;
    do {
#ifdef DJGPP
      _farpokeb(scrsel,p++,col);
#else
      *p++=col:
#endif
    } while(--w);
    p+=ofs;
  } while(--h);
  //getch();
}

void screen::bmp(dim* d,int xp,int yp,int ow,UCHAR* src)
{
  unsigned long p=getAdr(d->x(),d->y());
  if(!d->width()||!d->height()) return; // Ende, Fl„che besitzt keine Ausdehnung
  
  int w2=d->width();
  int h=d->height();
  int sofs=ow-w2;
  int ofs=xm-w2;
  int w;
  UCHAR* sp=d->x()-xp+((d->y()-yp)*ow)+src;
  do {
    w=w2;
    do {
      _farpokeb(scrsel,p++,*sp++);
    } while(--w);
    p+=ofs;
    sp+=sofs;
  } while(--h);
}

void screen::draw_pointer(int x,int y,unsigned char* pointer)
{
  if ((x<0)||(x>300)||(y<0)||(y>180)) return;
  unsigned long p=getAdr(x,y);
  int a=16;
  int b;
  do {
    b=16;
    do {
      if (*pointer==0x31)
	_farpokeb(scrsel,p,*pointer);
      pointer++;
      p++;
    } while (--b);
    p+=320-16;
  } while (--a);
}

void screen::save_pointer(int x,int y,unsigned char* buff)
{
  unsigned long p=getAdr(x,y);
  int a=16;
  int b;
  do {
    b=16;
    do {
      *buff++=_farpeekb(scrsel,p++);
    } while (--b);
    p+=320-16;
  } while (--a);
}

void screen::restore_pointer(int x,int y,unsigned char* buff)
{
  unsigned long p=getAdr(x,y);
  int a=16;
  int b;
  do {
    b=16;
    do {
      _farpokeb(scrsel,p++,*buff++);
    } while (--b);
    p+=320-16;
  } while (--a);
}

inline unsigned long screen::getAdr(int x,int y)
{
  return(y*320+x);
}

// Zeichnet Buchstaben
// Parameter: d    = sichtbarer Bereich auf dem Bildschirm
//            x,y  = Position auf dem Bildschirm
//            chr  = ASCII-Code des Zeichens
//            colF = Vordergrundfarbe
//            colB = Hintergrundfarbe
void screen::putchar(dim* d,int xp,int yp,uchar chr,int colF,int colB)
{
  if(!(d->height()&&d->width())) return;    // Ende, Bereich besitzt keine Ausdehnung

  unsigned long scrptr=getAdr(d->x(),d->y());
  
  // Offset innerhalb des Zeichens bestimmen
  xp=d->x()-xp;
  yp=d->y()-yp;
  int ofs=xm-d->width(); // Sprung zur n„chsten Bildschirmzeile
  
  // Zeiger auf erste Zeile des Zeichens
  unsigned long charptr=((chr-128)*8)+charset8x8ofs+yp;
  int yc=d->height(); // H”he des Ausschnitts
  int c;
  int xc2=d->width();  // Breite des Ausschnitts
  
  do {
    uchar mask=128>>xp;  // Maske fr ersten Pixel in Zeile
    int xc=xc2;
    uchar l=_farpeekb(charset8x8sel,charptr++);  // Aktuelle Zeile auslesen
    do {
      if(l&mask)      // Farbe des aktuellen Punktes bestimmen...
	c=colF;
      else
	c=colB;
      _farpokeb(scrsel,scrptr++,c);  // ...und in den Bildschirm
      // schreiben
      /*	         _farpokeb(scrsel,dest++,
			 ((*src>>4)&0xc0)|
			 ((*src>>2)&0x38)|
			 ((*src++>>1)&0x07)
			 );*/
      mask=mask>>1;   // N„chster Pixel in der Zeile
    } while(--xc);
    scrptr+=ofs;            // Zeiger auf n„chste Zeile
  } while(--yc);
  //getch();
}
#endif


































































































































