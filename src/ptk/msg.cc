// $Id: msg.cc,v 1.1.1.1 2000/09/30 13:42:10 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/msg.cc,v $
//--------------------------------------------------------------------------
// Message distribution
//
// (c)1998-1999 Sven Klose
//--------------------------------------------------------------------------

#include "frame.h"
#include "msg.h"
#include "config.h"

msg::msg(int id,int cmd,void* dat,int x,int y,long val) : msgbase(id,cmd,dat,x,y,val) {}

msg::msg(int cmd) : msgbase(0,cmd,0,0,0,0) {}

volatile void msg::distribute(frame* f)
{
  EHEAD("msg::distribute");
  if (f)
    for(;;) {
      // Message versenden und abbrechen, wenn sie angenommen wurde.
      if(f->receiver(this)==_discard) break;
      // Wurde sie nicht, Mutterframe beehren, solange noch eines vorhanden
      // ist.
      if (cmd!=_frame_dragged) {
	xrel+=f->x();
	yrel+=f->y();
      }
      f=f->parent();
      if(!f) break;
    }
#ifndef NOLOG
  else
    EHEAD("Destination is a null pointer.\a\n");
  EOK;
#endif
}

volatile void msg::send(frame* f)
{
  distribute(f);
}
