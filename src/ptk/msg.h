// $Id: msg.h,v 1.2 2001/10/11 03:31:17 sven Exp $
// $Source: /home/cvsroot/ptk/src/ptk/msg.h,v $
//--------------------------------------------------------------------------
// Internal message.
//
// (c)1998 Sven Klose
//--------------------------------------------------------------------------

#ifndef _ptk_msg_H
#define _ptk_msg_H

#include"msgbase.h"
#include"frame.h"

class msg : public msgbase {
public:
       msg(int id,int cmd,void* data,int xrel,int yrel,long val);
       msg(int cmd);
       volatile void distribute(frame* f); // Nachricht an f senden
       volatile void send(frame* f); // Nachricht an f senden und l”schen
};

#endif
