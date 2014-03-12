// $Id: main.cc,v 1.1.1.1 2000/09/30 13:42:09 sven Exp $
// $Source: /home/cvsroot/ptk/programs/helloworld/main.cc,v $
//--------------------------------------------------------------------------
// ptkhelloworld demo.
//
// (c)2000 Sven Klose
//--------------------------------------------------------------------------

#include "ptk.h"    // General ptk control
#include "layout.h" // Layout managers
#include "tkTextButton.h"
#include "tkWindow.h"

// Exit ptk
void my_exit (frame*, void*)
{
  ptk::shutdown ();
}

// Application derived from toolkit window
class HelloWorld : public tkWindow {
  public:
    HelloWorld () : tkWindow ("Hello World!")
    {
      link (new lmEqual)
	->link (new tkTextButton ("Hello World!", &my_exit));
      show ();
    }
};

// Create our window object and link it to the console
int main()
{
  return ptk::startup (new HelloWorld);
}
