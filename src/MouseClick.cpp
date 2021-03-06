#include "MouseClick.h"

using namespace std;


//Another function taken from stackoverflow...
//If its there might as well use it
//http://stackoverflow.com/questions/7492529/how-to-simulate-a-mouse-movement
void mouseClick(int button)
{
    Display *display = XOpenDisplay(NULL);
    XEvent event;
    if(display == NULL)
    {
        exit(EXIT_FAILURE);
    }
    memset(&event, 0x00, sizeof(event));
    event.type = ButtonPress;
    event.xbutton.button = button;
    event.xbutton.same_screen = True;
    XQueryPointer(display, RootWindow(display, DefaultScreen(display)),
            &event.xbutton.root, &event.xbutton.window, &event.xbutton.x_root,
            &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
    event.xbutton.subwindow = event.xbutton.window;
    while(event.xbutton.subwindow)
    {
        event.xbutton.window = event.xbutton.subwindow;
        XQueryPointer(display, event.xbutton.window, &event.xbutton.root, &event.xbutton.subwindow,
                &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
    }
    if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) fprintf(stderr, "Error\n");
    XFlush(display);
   // usleep(1000);
    event.type = ButtonRelease;
    event.xbutton.state = 0x100;
    if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) fprintf(stderr, "Error\n");
    XFlush(display);
    XCloseDisplay(display);
}

void mouseMove(int x, int y, bool left)
{
    Display *display = XOpenDisplay(NULL);
    Window root = DefaultRootWindow(display);
    XWarpPointer(display,None,root,0,0,0,0,x,y);
    XFlush(display);
    if(left)
        mouseClick(Button1);
    else
        mouseClick(Button3);
    XFlush(display);
    XCloseDisplay(display);
}
