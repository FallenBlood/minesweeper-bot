#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <list>
using namespace std;

//Taken from stackoverflow
//http://stackoverflow.com/questions/151407/how-to-get-an-x11-window-from-a-process-id
class WindowsMatchingPid
{
    public:
        WindowsMatchingPid(Display *display, Window wRoot, unsigned long pid)
            : _display(display)
              , _pid(pid)
    {
        //        Get the PID property atom.
        _atomPID = XInternAtom(display, "_NET_WM_PID", True);
        if(_atomPID == None)
        {
            cout << "No such atom" << endl;
            return;
        }

        search(wRoot);
    }

        const list<Window> &result() const { return _result; }

    private:
        unsigned long  _pid;
        Atom           _atomPID;
        Display       *_display;
        list<Window>   _result;

        void search(Window w)
        {
            // Get the PID for the current Window.
            Atom           type;
            int            format;
            unsigned long  nItems;
            unsigned long  bytesAfter;
            unsigned char *propPID = 0;
            if(Success == XGetWindowProperty(_display, w, _atomPID, 0, 1, False, XA_CARDINAL,
                        &type, &format, &nItems, &bytesAfter, &propPID))
            {
                if(propPID != 0)
                {
                    // If the PID matches, add this window to the result set.
                    if(_pid == *((unsigned long *)propPID))
                        _result.push_back(w);
                    XFree(propPID);
                }
            }

            // Recurse into child windows.
            Window    wRoot;
            Window    wParent;
            Window   *wChild;
            unsigned  nChildren;
            if(0 != XQueryTree(_display, w, &wRoot, &wParent, &wChild, &nChildren))
            {
                for(unsigned i = 0; i < nChildren; i++)
                    search(wChild[i]);
            }
        }
};

//Another function taken from stackoverflow...
//If its there might as well use it
//http://stackoverflow.com/questions/7492529/how-to-simulate-a-mouse-movement
void mouseClick(int button,Window win)
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
    usleep(100000);
    event.type = ButtonRelease;
    event.xbutton.state = 0x100;
    if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) fprintf(stderr, "Error\n");
    XFlush(display);
    XCloseDisplay(display);
}
main()
{
    FILE *fp = popen("ps","r");
    char buf[1024];
    int process=0;
    while(fgets(buf,1024,fp)){
        char* task = "gnomine";
        for(int i = 0; i < strlen(buf); ++i)
        {
            if(strncmp(&buf[i], task,strlen(task))==0)
            {
                char temp[64];
                strcpy(temp, buf);
                int j = 0;
                while(buf[j]!=' ')
                {
                    j++;
                }
                temp[j] = '\0';
                if(process == 0)
                {
                    process = atoi(temp);
                }
                break;
            }
        }
    }
    fclose(fp);
    Display *display = XOpenDisplay(0);

    process = 12703;
    WindowsMatchingPid match(display, XDefaultRootWindow(display), process);

    XWindowAttributes xwa;
    //    Print the result.
    const list<Window> &result = match.result();
    Window win;
    for(list<Window>::const_iterator it = result.begin(); it != result.end(); it++)
    {
        XMoveWindow(display,*it,0,0);
        win = *it;
        XGetWindowAttributes(display,*it,&xwa);
        break;
    }
    Window root = DefaultRootWindow(display);
    int width = XDisplayWidth(display,0)/2;
    int height = XDisplayHeight(display,0)/2;
    XWarpPointer(display,None,root,0,0,0,0,100,300);
    XFlush(display);
    mouseClick(Button3,win);
    XFlush(display);
    printf("%d %d\n",xwa.x,xwa.y);
    XCloseDisplay(display);
    return 0;
}
