#include "frame.h"
#include "toolkit.h"

int toolkit::receiver(msgbase* m) {
  switch(m->cmd) {
  case _frame_ack:
    onClick(); break;
  case _entering:
    onEnter(); break;
  case _leaving:
    onLeave(); break;
  default:
    return _propagate;
  }
  return _discard;
}
