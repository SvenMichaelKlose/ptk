// $Id: pointer.cc,v 1.1.1.1 2000/09/30 13:42:10 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/pointer.cc,v $
//--------------------------------------------------------------------------
// Pointer interface.
//
// (c)1998-1999 Sven Klose
//--------------------------------------------------------------------------

#include "frame.h"
#include "pointer.h"

pointer::pointer()
{
  EMSG("Initialising pointer ");

#ifdef DJGPP
  __dpmi_regs r;
  r.x.ax=0x0;
  __dpmi_simulate_real_mode_interrupt(0x33,&r);
  if(r.x.ax==0) {
    cerr << "No mouse driver installed\n";
    exit(-1);
  }
  /*
    // Maus initialisieren
    __dpmi_regs r;
    __dpmi_allocate_real_mode_callback(&mousehandler,&r,&mh);
    r.x.ax=0;
    __dpmi_simulate_real_mode_interrupt(0x33,&r);
    r.x.es=mh.segment;
    r.x.dx=mh.offset16;
    r.x.ax=0x0c;
    r.x.cx=0x3f;
    __dpmi_simulate_real_mode_interrupt(0x33,&r);
  */
  ptr_img=
    "1111111111110000"
    "1111111111100000"
    "1111111111000000"
    "1111111110000000"
    "1111111100000000"
    "1111111100000000"
    "1111111110000000"
    "1111111111000000"
    "1111001111100000"
    "1110000111110000"
    "1100000011111000"
    "1000000001111100"
    "0000000000111000"
    "0000000000010000"
    "0000000000000000";
#endif
}

pointer::~pointer()
{
#ifdef DJGPP
  __dpmi_free_real_mode_callback(&mh);
#endif
}

int pointer::poll(msg* m)
{
#ifdef DJGPP
  __dpmi_regs r;

  r.x.ax=0x3;
  __dpmi_simulate_real_mode_interrupt(0x33,&r);

  m->xrel = r.x.cx/2;
  m->yrel = r.x.dx;
  int a=r.x.bx&3;
  return a;
#endif
#ifdef EMBED_ANYX11
  return 0;
#endif
}
