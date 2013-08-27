#ifndef _MOUSECLICK_H_
#define _MOUSECLICK_H_

#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>

void mouseClick(int button);
void mouseMove(int x, int y, bool left);

#endif
