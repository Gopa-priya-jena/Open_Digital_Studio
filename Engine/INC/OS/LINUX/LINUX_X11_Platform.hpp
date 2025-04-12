

#pragma once

// #include <OS/OS.hpp>
#include <X11/X.h>
#include <X11/Xatom.h>
#include <X11/Xcms.h>
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/Xprotostr.h>
#include <X11/Xresource.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <X11/extensions/Xrandr.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>

#include <Define.hpp>
#include <vector>
// #define ODS_LINUX 1
#if ODS_LINUX

#define X11_MONITOR_STATE Monitor_X11 X11;
#define X11_WINDOW_STATE Window_X11 X11;

struct ODS_Gamma_ramp;
namespace OS

{

namespace WINDOW
{

void monitor_Update();

// TODO: implement
struct Window_X11
{
    std::vector<Colormap> colormap;
    std::vector<Window>   handle;
    Window                parent;
    std::vector<XIC>      ic;

    bool overrideRedirect;
    // Cached position and size used to filter out duplicate events
    int width, height;
    int xpos, ypos;

    // The last received cursor position, regardless of source
    int lastCursorPosX, lastCursorPosY;
    // The last position the cursor was warped to by GLFW
    int warpCursorPosX, warpCursorPosY;

    // The time of the last KeyPress event per keycode, for discarding
    // duplicate key events generated for some keys by ibus
    Time keyPressTimes[256];
};
struct Gamma_ramp;
typedef struct Monitor_X11
{
    RROutput output;
    RRCrtc   crtc;
    RRMode   oldMode;

    // Index of corresponding Xinerama screen,
    // for EWMH full screen window placement
    int index;
} Monitor_X11;

struct Feature_Availability_data
{
    bool server_init = false;
    bool Xi_available = false;
    bool Xrandr_availble = false;
    bool Xrandr_gammaBroken = false;
    bool Xrandr_monitorBroken = false;
    bool Xinerama_available = false;
    bool Xkb_available = false;
    bool Xkb_detectable = false;
    bool Xrender_available = false;
    bool Xshape_available = true;

    struct
    {
        bool SGI_swap_control;
        bool EXT_swap_control;
        bool MESA_swap_control;
        bool ARB_multisample;
        bool ARB_framebuffer_sRGB;
        bool EXT_framebuffer_sRGB;
        bool ARB_create_context;
        bool ARB_create_context_profile;
        bool ARB_create_context_robustness;
        bool EXT_create_context_es2_profile;
        bool ARB_create_context_no_error;
        bool ARB_context_flush_control;

    } glx;
};

struct X11_Extension_codes
{
    struct
    {
        int majorOpcode;
        int eventBase;
        int errorBase;
    } xi;
    struct
    {
        int eventBase;
        int errorBase;

    } glx;

    struct
    {
        int eventBase;
        int errorBase;
    } randr;

    struct
    {
        int          majorOpcode;
        int          eventBase;
        int          errorBase;
        unsigned int group;
    } xkb;
    struct
    {
        int eventBase;
        int errorBase;
    } xrender;
    struct
    {
        int eventBase;
        int errorBase;
    } xshape;
    struct
    {
        int eventBase;
        int errorBase;
    } vidmode;
};

class X11_Display_Server
{
   public:
    Display *Main_Display{};
    int      screen{};
    Window   root{};

    // System content scale
    float contentScaleX{}, contentScaleY{};
    // Context for mapping window XIDs to _GLFWwindow pointers
    XContext context{};
    // XIM input method
    XIM im{};
    // The previous X error handler, to be restored later
    XErrorHandler errorHandler{};
    // Most recent error code received by X error handler
    int errorCode{};
    // Primary selection string (while the primary selection is owned)
    char *primarySelectionString{};
    // Clipboard string (while the selection is owned)
    char *clipboardString{};
    // Key name string
    // char keynames[GLFW_KEY_LAST + 1][5];
    // X11 keycode to GLFW key LUT
    // short int keycodes[256];
    // GLFW key to X11 keycode LUT
    // short int scancodes[GLFW_KEY_LAST + 1];
    // Where to place the cursor when re-enabled
    double restoreCursorPosX{}, restoreCursorPosY{};
    // The window whose disabled cursor mode is active
    // _GLFWwindow *disabledCursorWindow;

    // Window manager atoms
    Atom NET_SUPPORTED;
    Atom NET_SUPPORTING_WM_CHECK;
    Atom WM_PROTOCOLS;
    Atom WM_STATE;
    Atom WM_DELETE_WINDOW;
    Atom NET_WM_NAME;
    Atom NET_WM_ICON_NAME;
    Atom NET_WM_ICON;
    Atom NET_WM_PID;
    Atom NET_WM_PING;
    Atom NET_WM_WINDOW_TYPE;
    Atom NET_WM_WIDOW_TYPE_NORMAL;
    Atom NET_WM_STATE;
    Atom NET_WM_STATE_ABOVE;
    Atom NET_WM_STATE_FULLSCREEN;
    Atom NET_WM_STATE_MAXIMIZED_VERT;
    Atom NET_WM_STATE_MAXIMIZED_HORZ;
    Atom NET_WM_STATE_DEMANDS_ATTENTION;
    Atom NET_WM_BYPASS_COMPOSITOR;
    Atom NET_WM_FULLSCREEN_MONITORS;
    Atom NET_WM_WINDOW_OPACITY;
    Atom NET_WM_CM_Sx;
    Atom NET_WORKAREA;
    Atom NET_CURRENT_DESKTOP;
    Atom NET_ACTIVE_WINDOW;
    Atom NET_FRAME_EXTENTS;
    Atom NET_REQUEST_FRAME_EXTENTS;
    Atom MOTIF_WM_HINTS;

    // Xdnd (drag and drop) atoms
    Atom XdndAware;
    Atom XdndEnter;
    Atom XdndPosition;
    Atom XdndStatus;
    Atom XdndActionCopy;
    Atom XdndDrop;
    Atom XdndFinished;
    Atom XdndSelection;
    Atom XdndTypeList;
    Atom text_uri_list;

    // Selection (clipboard) atoms
    Atom TARGETS;
    Atom MULTIPLE;
    Atom INCR;
    Atom CLIPBOARD;
    Atom PRIMARY;
    Atom CLIPBOARD_MANAGER;
    Atom SAVE_TARGETS;
    Atom NULL_;
    Atom UTF8_STRING;
    Atom COMPOUND_STRING;
    Atom ATOM_PAIR;
    Atom GLFW_SELECTION;
    void detectEWMH();

    X11_Display_Server();
    bool          init();
    void          setup();
    bool          Create_Window();
    bool          Terminate_Window();
    unsigned long GetWindowPropertyX11(Window          window,
                                       Atom            property,
                                       Atom            type,
                                       unsigned char **value);
    void          GrabErrorHandlerX11(void);
    void          ReleaseErrorHandlerX11(void);
    Atom          getAtomIfSupported(Atom         *supportedAtoms,
                                     unsigned long atomCount,
                                     const char   *atomName);
};
// some static variables

static X11_Extension_codes Extension_codes;

static Feature_Availability_data Feature_available;
// NOTE: global X11server
//
//
//
//
inline X11_Display_Server server;
}  // namespace WINDOW

}  // namespace OS
#endif
