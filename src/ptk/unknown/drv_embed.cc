// $Id: drv_embed.cc,v 1.1.1.1 2000/09/30 13:42:10 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/unknown/drv_embed.cc,v $
//--------------------------------------------------------------------------
// embed.cc -- Wrappers for underlying operating system or window manager.
//
// 1998-1999 Sven Klose
//--------------------------------------------------------------------------

#include "frame.h"
#include "embed.h"
#include "wmshell.h"
#include "drv_display.h"

#ifdef EMBED_ANYX11
#include <X11/keysym.h>
#include "windowdrawer.h"

wmshell* embed::getWindow(frame* f,Window w)
{
  wmshell* p=(wmshell*) f->child();
  do {
    if (((window_drawer*)p->getDrawer())->_win==w) return p;
  } while ((p=(wmshell*)p->next()));
  return 0;
}

int embed::event_loop(console* con)
{
#warning TODO: Handle LeaveNotify event for X11.
  // X11 message poll, emulates hardware pointer to call process_message
  wmshell* gw=0; // Needed by message_loop() to tell process_message()
  XEvent report;
  KeySym keysym;
  XComposeStatus compose;
  char keyasc;
  int keycount;
  
  Display* disp=x11_display();
  while (1) {
    if (!con->child()) {
#ifdef EXCESSIVE_LOGGING
      cout << "No more windows.\n";
#endif
      return 0;
    }

    XNextEvent(disp,&report);
    con->_nr_sent_messages++;
#ifdef EXCESSIVE_LOGGING
    cout << con->_nr_sent_messages << " msgs ";
#endif
    switch (report.type) {

      // Drawing
    case Expose:
      if (!(gw=getWindow(con,report.xexpose.window))) break;
      if (gw)
	if (gw->parent()) {
	  dim d(report.xexpose.x,
		report.xexpose.y,
		report.xexpose.width,
		report.xexpose.height
		);
#ifdef EXCESSIVE_LOGGING
	  cout << "xexpose: " << report.xexpose.x << " " << report.xexpose.y << " " << report.xexpose.width << " " << report.xexpose.height << "\n";
#endif
	  gw->refresh(&d);
	}
      EMSG("(redraw)");
      break;

      // Pointer 
    case MotionNotify:
      EMSG("(newpos)");
      if (!(gw=getWindow(con,report.xmotion.window))) break;
      con->_curr_x=report.xmotion.x;
      con->_curr_y=report.xmotion.y;
      con->process_message(gw,0);
      break;
    case ButtonPress:
      EMSG("(btnpress)");
      if (!(gw=getWindow(con,report.xbutton.window))) break;
      con->_buttonstate=1;
      con->process_message(gw,report.xbutton.time);
      break;
    case ButtonRelease:
      EMSG("(btnrelease)");
      if (!(gw=getWindow(con,report.xbutton.window))) break;
      con->_buttonstate=0;
      con->process_message(gw,report.xbutton.time);
      break;

      // Keyboard
      case KeyPress:
	EMSG("(keypress)");
	keycount=XLookupString(&report.xkey,&keyasc,1,&keysym,&compose);
	/*	if (((keysym>=XK_KP_Space)
	     && (keysym<=XK_KP_9))
	    || ((keysym>=XK_space)
	    && (keysym<=XK_asciitilde)))*/
	  {
	    msg m(0,_keypress,0,con->_curr_x,con->_curr_y,keysym);//asc);
	    frame* tmp;
	    if (con->_kbdFocus)
	      tmp=con->_kbdFocus;
	    else {
	      int xt=con->_curr_x;
	      int yt=con->_curr_y;
	      tmp=con->seek_frame(gw,&xt,&yt);
	    }
	    m.distribute(tmp);
	  }
	break;
      case MappingNotify:
	EMSG("(X11 MappingNotify)");
	XRefreshKeyboardMapping(&report.xmapping);
	break;

      // Miscellaneous
      case ConfigureNotify:
	{
	  EMSG("(X11 ConfigureNotify)");
	  if (!(gw=getWindow(con,report.xbutton.window))) break;
	  if (gw->isPopup()) {
#ifdef EXCESSIVE_LOGGING
	    cout << "Won't resize popup window.\n";
#endif
	    break;
	  }
	  int nw=report.xconfigure.width;
	  int nh=report.xconfigure.height;
	  if ((nw)&&(nh)) {
	    if((nw!=gw->width())||(nh!=gw->height())) {
#ifdef EXCESSIVE_LOGGING
	      cout << "window::resize "
		   << gw->width() << " x " << gw->height() << " => "
		   << nw << " x " << nh << "\n";
#endif
	      gw->resize(nw,nh);
	    }
	    if (report.xconfigure.above) {
	      int nx=report.xconfigure.x;
	      int ny=report.xconfigure.y;
#ifdef EXCESSIVE_LOGGING
	      cout << "window::move "
		   << gw->x() << " x " << gw->y() << " => "
		   << nx << " x " << ny << "\n";
#endif
	      gw->dim::move(nx,ny);
	    } else {
	      gw->set_emb_x(report.xconfigure.x);
	      gw->set_emb_y(report.xconfigure.y);
	    }
	  }
	}
	break;
      default:
	EMSG("(XEvent dropped)");
	break;
      }
      EMSG("\n");
#ifdef PTK_FLUSH_AFTER_EVENT
      con->scr->flush();
#endif
    }
}
#endif

// $Log: drv_embed.cc,v $
// Revision 1.1.1.1  2000/09/30 13:42:10  sven
// ptk is a lightweight cross-platform user interface written in C++.
//
// Revision 1.1.1.1  2000/02/06 01:23:29  sven
// Let's go!
//
// Revision 1.1.1.1  2000/02/04 21:39:17  sven
// Let's go!
//
