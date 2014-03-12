// $Id: config.h,v 1.1.1.1 2000/09/30 13:42:09 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/config.h,v $
//--------------------------------------------------------------------------
// Behavioural configuration
//
// (c)1998-1999 Sven Klose
//--------------------------------------------------------------------------

#ifndef _ptk_config_H
#define _ptk_config_H

//#define EXCESSIVE_LOGGING
//#define PTK_FLUSH_AFTER_EVENT /* Halts until drawing functions finished */
//#define EMBED_X11             /* is broken in driver/windowdrawer.cc|image.cc */
#ifndef EMBED_X11WRAP
#define EMBED_X11WRAP          /* Use X11 wrappers */
#endif
//#define PTK_X11_USE_MITSHM    /* Use MIT shared memory extension for X11 */
//#define EMBED_NONE            /* Not embedded into existing window manager */
//#define PTK_BUFFERED_WINDOWDRAWER /* Window contents are off-screen */
//#define PTK_USES_WINDOWDRAWER /* Use special drawer for window contents */

// Precompiled image drivers
#define PTK_KNOWN_IMAGE_DEPTHS 0x80008080 // be prepared for 32/16/8 bit modes
#define PTK_BIG_ENDIAN true /* False if little endian system */
#define PTK_NO_FSTREAM_H

//////////////////////////////////
// DJ Delories GNU port for DoS //
/////////////////////////////////
#ifdef DJGPP
#define EMBED_NONE
#endif

/////////////////////
// EMBED_X11 setup //
////////////////////
#ifdef EMBED_X11
#define EMBED_WINDOW_REDRAW
#define EMBED_ANYX11
#endif

/////////////////////////
// EMBED_X11WRAP setup //
////////////////////////
#ifdef EMBED_X11WRAP
#define EMBED_ANYX11
#endif

#ifdef EMBED_ANYX11
#define PTK_USES_WINDOWDRAWER
#endif

#ifndef EMBED_NONE
#ifndef EMBED_X11
#ifndef EMBED_X11WRAP
#error No target system specified.
#endif
#endif
#endif

//////////////////////////////////////
// Data types - make them disappear //
/////////////////////////////////////
//#define	Color	unsigned short

////////////
// Logging //
////////////
#ifdef EXCESSIVE_LOGGING
#define DEBUG
#endif

#ifdef DEBUG
    #include <fstream.h>
#endif

#ifdef EXCESSIVE_LOGGING
    #include <fstream.h>
    #define EHEAD(m) cout << m << ": ";
    #define EOK      cout << "- ok\n";
    #define EMSG(m)  cout << m;
    // (Errors that should be immutable)
    #define PTKERROR(m) cout << m;
    #define PTKBASE(m)  cout << "Accidently called pure virtual non-void function " << m << "().\n";return 0;
    #define PTKVBASE(m) cout << "Accidently called pure virtual function " << m << "().\n";
#else
    #define EHEAD(m)
    #define EOK
    #define EMSG(m)
#endif

#endif
