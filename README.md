ptk – a tiny ANSI-C++ graphical user interface
==============================================

ptk is a lightweight cross-platform user interface written in C++.
ptk can run as a toolkit for applications running on other user interfaces
or as a standalone application for environments where system resources are
scarce.

ptk consists of three layers, each in a separate library:

* libptkdrv: Hardware dependend drivers for keyboard, pointer,
  display, canvases and windows.
* libptk: Framework for drawing and auto-layout.
* libptktoolkit: A simple collection of widgets.

ptk uses only standard ANSI-C++ features so you're free to use your
preferred components like libsigc++ for signals.
