/*

// standard header
#include <Define.hpp>
#include <algorithm>
#include <cassert>
#include <climits>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <format>
#include <iterator>

//
// #include "OS/LINUX/X11_Platform.hpp"

// X11 headers
#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/extensions/XI2.h>
#include <X11/extensions/XInput2.h>
#include <X11/extensions/Xge.h>
#include <X11/extensions/Xinerama.h>
#include <X11/extensions/Xrandr.h>
#include <X11/extensions/shape.h>
#include <X11/extensions/xf86vmode.h>
#include <xcb/xcb.h>
// ODS library
#include <DEBUG.hpp>
#include <Define.hpp>
#include <Global.hpp>
#include <OS/LINUX/LINUX_WINDOW.hpp>
#include <OS/LINUX/LINUX_X11_Platform.hpp>
#include <OS/OS.hpp>
#include <OS/OS_WINDOW.hpp>
#include <string_view>
#include <utility>

#include "ALGORITHMS/Algorithms.hpp"
#include "DATA_STRUCTURE/Fixed_list.hpp"

//
namespace OS
{

namespace WINDOW
{

// NOTE:  global  monitor variable

std::vector<ODS_Monitor> Monitor_vector;
unsigned int             current_monitor_index{0};

// global window datastucture
ODS_Window Window_Element_List;

// monitor realted function
void monitor_Update();

bool OS_Window_Init()
{
    if (!Feature_available.server_init)
    {
        Feature_available.server_init = true;
        server.init();
        return true;
    }
    return true;
}

X11_Display_Server::X11_Display_Server()

{
}

//
unsigned long X11_Display_Server::GetWindowPropertyX11(Window          window,
                                                       Atom            property,
                                                       Atom            type,
                                                       unsigned char **value)
{
    FUNCTION_PROFILE

    Atom          actualType;
    int           actualFormat;
    unsigned long itemCount, bytesAfter;

    XGetWindowProperty(Main_Display,
                       window,
                       property,
                       0,
                       LONG_MAX,
                       False,
                       type,
                       &actualType,
                       &actualFormat,
                       &itemCount,
                       &bytesAfter,
                       value);

    return itemCount;
}
Atom X11_Display_Server::getAtomIfSupported(Atom         *supportedAtoms,
                                            unsigned long atomCount,
                                            const char   *atomName)
{
    FUNCTION_PROFILE
    const Atom atom = XInternAtom(Main_Display, atomName, False);

    for (unsigned long i = 0; i < atomCount; i++)
    {
        if (supportedAtoms[i] == atom) return atom;
    }

    return None;
}

void X11_Display_Server::GrabErrorHandlerX11(void)
{
    FUNCTION_PROFILE
    assert(errorHandler == NULL);
    errorCode = Success;
    errorHandler = XSetErrorHandler(errorHandler);
}

// Clears the X error handler callback
//
void X11_Display_Server::ReleaseErrorHandlerX11(void)
{
    FUNCTION_PROFILE
    // Synchronize to make sure all commands are processed
    XSync(Main_Display, False);
    XSetErrorHandler(errorHandler);
    errorHandler = NULL;
}

void X11_Display_Server::detectEWMH()
{
    FUNCTION_PROFILE
    // First we read the _NET_SUPPORTING_WM_CHECK property on the root window

    Window *windowFromRoot = NULL;
    if (!GetWindowPropertyX11(root,
                              NET_SUPPORTING_WM_CHECK,
                              XA_WINDOW,
                              (unsigned char **)&windowFromRoot))
    {
        return;
    }

    GrabErrorHandlerX11();

    // If it exists, it should be the XID of a top-level window
    // Then we look for the same property on that window

    Window *windowFromChild = NULL;
    if (!GetWindowPropertyX11(*windowFromRoot,
                              NET_SUPPORTING_WM_CHECK,
                              XA_WINDOW,
                              (unsigned char **)&windowFromChild))
    {
        ReleaseErrorHandlerX11();
        XFree(windowFromRoot);
        return;
    }

    ReleaseErrorHandlerX11();

    // If the property exists, it should contain the XID of the window

    if (*windowFromRoot != *windowFromChild)
    {
        XFree(windowFromRoot);
        XFree(windowFromChild);
        return;
    }

    XFree(windowFromRoot);
    XFree(windowFromChild);

    // We are now fairly sure that an EWMH-compliant WM is currently running
    // We can now start querying the WM about what features it supports by
    // looking in the _NET_SUPPORTED property on the root window
    // It should contain a list of supported EWMH protocol and state atoms

    Atom               *supportedAtoms = NULL;
    const unsigned long atomCount = GetWindowPropertyX11(
        root, NET_SUPPORTED, XA_ATOM, (unsigned char **)&supportedAtoms);

    // See which of the atoms we support that are supported by the WM

    NET_WM_STATE =
        getAtomIfSupported(supportedAtoms, atomCount, "_NET_WM_STATE");
    NET_WM_STATE_ABOVE =
        getAtomIfSupported(supportedAtoms, atomCount, "_NET_WM_STATE_ABOVE");
    NET_WM_STATE_FULLSCREEN = getAtomIfSupported(
        supportedAtoms, atomCount, "_NET_WM_STATE_FULLSCREEN");
    NET_WM_STATE_MAXIMIZED_VERT = getAtomIfSupported(
        supportedAtoms, atomCount, "_NET_WM_STATE_MAXIMIZED_VERT");
    NET_WM_STATE_MAXIMIZED_HORZ = getAtomIfSupported(
        supportedAtoms, atomCount, "_NET_WM_STATE_MAXIMIZED_HORZ");
    NET_WM_STATE_DEMANDS_ATTENTION = getAtomIfSupported(
        supportedAtoms, atomCount, "_NET_WM_STATE_DEMANDS_ATTENTION");
    NET_WM_FULLSCREEN_MONITORS = getAtomIfSupported(
        supportedAtoms, atomCount, "_NET_WM_FULLSCREEN_MONITORS");
    NET_WM_WINDOW_TYPE =
        getAtomIfSupported(supportedAtoms, atomCount, "_NET_WM_WINDOW_TYPE");
    NET_WM_WIDOW_TYPE_NORMAL = getAtomIfSupported(
        supportedAtoms, atomCount, "_NET_WM_WINDOW_TYPE_NORMAL");
    NET_WORKAREA =
        getAtomIfSupported(supportedAtoms, atomCount, "_NET_WORKAREA");
    NET_CURRENT_DESKTOP =
        getAtomIfSupported(supportedAtoms, atomCount, "_NET_CURRENT_DESKTOP");
    NET_ACTIVE_WINDOW =
        getAtomIfSupported(supportedAtoms, atomCount, "_NET_ACTIVE_WINDOW");
    NET_FRAME_EXTENTS =
        getAtomIfSupported(supportedAtoms, atomCount, "_NET_FRAME_EXTENTS");
    NET_REQUEST_FRAME_EXTENTS = getAtomIfSupported(
        supportedAtoms, atomCount, "_NET_REQUEST_FRAME_EXTENTS");

    if (supportedAtoms) XFree(supportedAtoms);
}

bool X11_Display_Server::init()
{
    FUNCTION_PROFILE

    // here we initialize the  x11 server

    XInitThreads();
    XrmInitialize();
    // getting a connection

    Main_Display = XOpenDisplay(NULL);

    if (!Main_Display)
    {
        const char *name = getenv("DISPLAY");
        if (name)
        {
            OS_console_out(
                std::format("X11: Failed to open Main_Display {} ", name),
                RED,
                BLACK,
                Bold);
            return 0;
        }
    }

    {
        screen = DefaultScreen(Main_Display);
        root = RootWindow(Main_Display, screen);

        context = XUniqueContext();
        // doing dpi settings
        float xdpi = 96.f, ydpi = 96.f;

        // NOTE: Basing the scale on Xft.dpi where available should provide the
        // most
        //       consistent user experience (matches Qt, Gtk, etc), although not
        //       always the most accurate one
        char *rms = XResourceManagerString(Main_Display);
        if (rms)
        {
            XrmDatabase db = XrmGetStringDatabase(rms);
            if (db)
            {
                XrmValue value;
                char    *type = NULL;

                if (XrmGetResource(db, "Xft.dpi", "Xft.Dpi", &type, &value))
                {
                    if (type && strcmp(type, "String") == 0)
                        xdpi = ydpi = atof(value.addr);
                }

                XrmDestroyDatabase(db);
            }
        }

        contentScaleX = xdpi / 96.f;
        contentScaleY = ydpi / 96.f;
    }
    if (!setlocale(LC_CTYPE, ""))
    {
        fprintf(stderr, "Warning: Failed to set locale.\n");
    }

    // initialize extensions

    ///////////////// check for X input system for advance
    /// input///////////////////////////
    int major = 0, minor = 0;
    {
        if (XQueryExtension(Main_Display,
                            "XInputExtension",
                            &major,
                            &Extension_codes.xi.eventBase,
                            &Extension_codes.xi.errorBase))
        {
            major = 2;
            minor = 0;

            if (XIQueryVersion(Main_Display, &major, &minor) == Success)
            {
                Feature_available.Xi_available = true;
            }
        }
    }

    major = 0;
    minor = 0;
    ///////////////////////////////check for
    /// videomode////////////////////////////

    XF86VidModeQueryExtension(Main_Display,
                              &Extension_codes.vidmode.eventBase,
                              &Extension_codes.vidmode.errorBase);

    ///////////////////////// check for randar///////////////////////
    if (XRRQueryExtension(Main_Display,
                          &Extension_codes.randr.eventBase,
                          &Extension_codes.randr.errorBase))
    {
        if (XRRQueryVersion(Main_Display, &major, &minor))
        {
            // The GLFW RandR path requires at least version 1.3
            if (major > 1 || minor >= 3)
                Feature_available.Xrandr_availble = true;
        }
        else
        {
            OS::TERMINAL::OS_console_out(
                "X11: Failed to query RandR version \n", RED, BLACK, Bold);
        }
    }

    {
        if (Feature_available.Xrandr_availble)
        {
            XRRScreenResources *sr =
                XRRGetScreenResourcesCurrent(Main_Display, root);

            if (!sr->ncrtc || !XRRGetCrtcGammaSize(Main_Display, sr->crtcs[0]))
            {
                // This is likely an older Nvidia driver with broken gamma
                // support Flag it as useless and fall back to xf86vm gamma, if
                // available
                Feature_available.Xrandr_gammaBroken = true;
            }

            if (!sr->ncrtc)
            {
                // A system without CRTCs is likely a system with broken RandR
                // Disable the RandR  Monitor_vector.back() path and fall back
                // to core functions
                Feature_available.Xrandr_availble = true;
            }

            XRRFreeScreenResources(sr);
        }

        if (Feature_available.Xrandr_availble &&
            !Feature_available.Xrandr_gammaBroken)
        {
            XRRSelectInput(Main_Display, root, RROutputChangeNotifyMask);
        }
    }
    major = 0;
    minor = 0;
    ///////////////////////// check for Xinerama support//////////////////////
    if (XineramaQueryExtension(Main_Display, &major, &minor))
    {
        if (XineramaIsActive(Main_Display))
            Feature_available.Xinerama_available = true;
    }

    major = 1;
    minor = 0;
    ///////////////////////// check for Xkb  support//////////////////////
    Feature_available.Xkb_available =
        XkbQueryExtension(Main_Display,
                          &Extension_codes.xkb.majorOpcode,
                          &Extension_codes.xkb.eventBase,
                          &Extension_codes.xkb.errorBase,
                          &major,
                          &minor);

    if (Feature_available.Xkb_available)
    {
        Bool supported;

        if (XkbSetDetectableAutoRepeat(Main_Display, True, &supported))
        {
            if (supported) Feature_available.Xkb_detectable = true;
        }

        XkbStateRec state;
        if (XkbGetState(Main_Display, XkbUseCoreKbd, &state) == Success)
            Extension_codes.xkb.group = (unsigned int)state.group;

        XkbSelectEventDetails(Main_Display,
                              XkbUseCoreKbd,
                              XkbStateNotify,
                              XkbGroupStateMask,
                              XkbGroupStateMask);
    }

    ///////////////////////// check for Xrender  support//////////////////////

    if (XRenderQueryExtension(Main_Display,
                              &Extension_codes.xrender.errorBase,
                              &Extension_codes.xrender.eventBase))
    {
        if (XRenderQueryVersion(Main_Display, &major, &minor))
        {
            Feature_available.Xrender_available = true;
        }
    }

    ///////////////////////// check for Xshape   support//////////////////////
    if (XShapeQueryExtension(Main_Display,
                             &Extension_codes.xshape.errorBase,
                             &Extension_codes.xshape.eventBase))
    {
        if (XShapeQueryVersion(Main_Display, &major, &minor))
        {
            Feature_available.Xshape_available = true;
        }
    }

    // String format atoms
    NULL_ = XInternAtom(Main_Display, "NULL", False);
    UTF8_STRING = XInternAtom(Main_Display, "UTF8_STRING", False);
    ATOM_PAIR = XInternAtom(Main_Display, "ATOM_PAIR", False);

    // Custom selection property atom
    GLFW_SELECTION = XInternAtom(Main_Display, "GLFW_SELECTION", False);

    // ICCCM standard clipboard atoms
    TARGETS = XInternAtom(Main_Display, "TARGETS", False);
    MULTIPLE = XInternAtom(Main_Display, "MULTIPLE", False);
    PRIMARY = XInternAtom(Main_Display, "PRIMARY", False);
    INCR = XInternAtom(Main_Display, "INCR", False);
    CLIPBOARD = XInternAtom(Main_Display, "CLIPBOARD", False);

    // Clipboard manager atoms
    CLIPBOARD_MANAGER = XInternAtom(Main_Display, "CLIPBOARD_MANAGER", False);
    SAVE_TARGETS = XInternAtom(Main_Display, "SAVE_TARGETS", False);

    // Xdnd (drag and drop) atoms
    XdndAware = XInternAtom(Main_Display, "XdndAware", False);
    XdndEnter = XInternAtom(Main_Display, "XdndEnter", False);
    XdndPosition = XInternAtom(Main_Display, "XdndPosition", False);
    XdndStatus = XInternAtom(Main_Display, "XdndStatus", False);
    XdndActionCopy = XInternAtom(Main_Display, "XdndActionCopy", False);
    XdndDrop = XInternAtom(Main_Display, "XdndDrop", False);
    XdndFinished = XInternAtom(Main_Display, "XdndFinished", False);
    XdndSelection = XInternAtom(Main_Display, "XdndSelection", False);
    XdndTypeList = XInternAtom(Main_Display, "XdndTypeList", False);
    text_uri_list = XInternAtom(Main_Display, "text/uri-list", False);

    // ICCCM, EWMH and Motif window property atoms
    // These can be set safely even without WM support
    // The EWMH atoms that require WM support are handled in detectEWMH
    WM_PROTOCOLS = XInternAtom(Main_Display, "WM_PROTOCOLS", False);
    WM_STATE = XInternAtom(Main_Display, "WM_STATE", False);
    WM_DELETE_WINDOW = XInternAtom(Main_Display, "WM_DELETE_WINDOW", False);
    NET_SUPPORTED = XInternAtom(Main_Display, "_NET_SUPPORTED", False);
    NET_SUPPORTING_WM_CHECK =
        XInternAtom(Main_Display, "_NET_SUPPORTING_WM_CHECK", False);
    NET_WM_ICON = XInternAtom(Main_Display, "_NET_WM_ICON", False);
    NET_WM_PING = XInternAtom(Main_Display, "_NET_WM_PING", False);
    NET_WM_PID = XInternAtom(Main_Display, "_NET_WM_PID", False);
    NET_WM_NAME = XInternAtom(Main_Display, "_NET_WM_NAME", False);
    NET_WM_ICON_NAME = XInternAtom(Main_Display, "_NET_WM_ICON_NAME", False);
    NET_WM_BYPASS_COMPOSITOR =
        XInternAtom(Main_Display, "_NET_WM_BYPASS_COMPOSITOR", False);
    NET_WM_WINDOW_OPACITY =
        XInternAtom(Main_Display, "_NET_WM_WINDOW_OPACITY", False);
    MOTIF_WM_HINTS = XInternAtom(Main_Display, "_MOTIF_WM_HINTS", False);
    // The compositing manager selection name contains the screen number
    {
        char name[32];
        snprintf(name, sizeof(name), "_NET_WM_CM_S%u", screen);
        NET_WM_CM_Sx = XInternAtom(Main_Display, name, False);
    }

    detectEWMH();

    if (XSupportsLocale())
    {
        XSetLocaleModifiers("");

        // If an IM is already present our callback will be called right away
        //   XRegisterIMInstantiateCallback(Main_Display, NULL, NULL, NULL,
        //                                  inputMethodInstantiateCallback,
        //                                  NULL);
    }
    monitor_Update();
    return true;
}

// monitor implementations

inline bool modeIsGood(const XRRModeInfo &mi)
{
    FUNCTION_PROFILE
    return (mi.modeFlags & RR_Interlace) == 0;
}

inline int calculateRefreshRate(const XRRModeInfo &mi)
{
    FUNCTION_PROFILE
    if (mi.hTotal && mi.vTotal)
    {
        return round((double)mi.dotClock /
                     ((double)mi.hTotal * (double)mi.vTotal));
    }
    return 0.0;
}

void monitor_Update()
{
    FUNCTION_PROFILE

    int                 screen_count = 0;
    XineramaScreenInfo *screens =
        XineramaQueryScreens(server.Main_Display, &screen_count);
    XRRScreenResources *screen_resource =
        XRRGetScreenResourcesCurrent(server.Main_Display, server.root);
    RROutput primary = XRRGetOutputPrimary(server.Main_Display, server.root);
    unsigned int present_monitor = Monitor_vector.size();
    // TODO: this will need bit class
    // std::uint64_t checked[7]{0, 0, 0, 0, 00, 0, 0};
    // ERROR: code is a erron needs fix
    bool checked[Monitor_vector.size()];
    // for (unsigned int i = 0; i <= Monitor_vector.size(); i++)
    // {
    //     checked[i] = false;
    // }

    // bool    *checked = (bool *)Stack_alloc(sizeof(bool), present_monitor);
    bool changed = false;
    for (unsigned int i = 0; i < screen_resource->noutput; i++)
    {
        XRROutputInfo *output_info = XRRGetOutputInfo(
            server.Main_Display, screen_resource, screen_resource->outputs[i]);
        if (output_info->connection != RR_Connected ||
            output_info->crtc == None)
        {
            XRRFreeOutputInfo(output_info);
            continue;
        }
        bool flag = 0;
        for (unsigned int j = 0; j < Monitor_vector.size(); j++)
        {
            if ((!checked[j]) &&
                (screen_resource->outputs[i] == Monitor_vector[j].X11.output))
            {
                flag = 1;
                checked[j] = true;
                break;
            }
        }
        if (!flag)
        {
            changed = true;
            // adding element
            Monitor_vector.push_back({});

            XRRCrtcInfo *crtc_info = XRRGetCrtcInfo(
                server.Main_Display, screen_resource, output_info->crtc);

            if (crtc_info->rotation == RR_Rotate_90 ||
                crtc_info->rotation == RR_Rotate_270)
            {
                Monitor_vector.back().widthMM = output_info->mm_height;
                Monitor_vector.back().heightMM = output_info->mm_width;
            }
            else
            {
                Monitor_vector.back().widthMM = output_info->mm_width;
                Monitor_vector.back().heightMM = output_info->mm_height;
            }

            Monitor_vector.back().name = output_info->name;
            Monitor_vector.back().widthMM = output_info->mm_width;
            Monitor_vector.back().heightMM = output_info->mm_height;
            Monitor_vector.back().X11.output = screen_resource->outputs[i];
            Monitor_vector.back().X11.crtc = output_info->crtc;

            for (int j = 0; j < screen_count; j++)
            {
                if (screens[j].x_org == crtc_info->x &&
                    screens[j].y_org == crtc_info->y &&
                    screens[j].width == crtc_info->width &&
                    screens[j].height == crtc_info->height)
                {
                    Monitor_vector.back().X11.index = j;
                    break;
                }
            }

            if (Monitor_vector.back().widthMM <= 0 ||
                Monitor_vector.back().heightMM <= 0)
            {
                // HACK: If RandR does not provide a physical size, assume the
                //       X11 default 96 DPI and calculate from the CRTC viewport
                // NOTE: These members are affected by rotation, unlike the mode
                //       info and output info members
                Monitor_vector.back().widthMM =
                    (int)(crtc_info->width * 25.4f / 96.f);
                Monitor_vector.back().heightMM =
                    (int)(crtc_info->height * 25.4f / 96.f);
            }

            // Get all available modes

            for (int m = 0; m < output_info->nmode; m++)
            {
                const XRRModeInfo *mi = nullptr;
                for (int k = 0; k < screen_resource->nmode; k++)
                {
                    if (screen_resource->modes[k].id == output_info->modes[m])
                    {
                        mi = &screen_resource->modes[k];
                        break;
                    }
                }
                if (mi && modeIsGood(*mi))
                {
                    Monitor_vector.back().modes.push_back({});
                    Monitor_vector.back().modes.back().width = mi->width;
                    Monitor_vector.back().modes.back().height = mi->height;
                    Monitor_vector.back().modes.back().refreshRate =
                        calculateRefreshRate(*mi);
                }
            }
            XRRFreeCrtcInfo(crtc_info);

            if (primary == Monitor_vector.back().X11.output)
            {
                std::swap(Monitor_vector[0], Monitor_vector.back());
            }
        }

        XRRFreeOutputInfo(output_info);
    }
    // removing removed monitors
    if (changed)
    {
        for (unsigned int i = 0; i < present_monitor; i++)
        {
            if (!checked[i])
            {
                Monitor_vector.erase(Monitor_vector.begin() + i);
            }
        }
        // resizing Monitor_vector size
        Monitor_vector.shrink_to_fit();
    }
    if (Monitor_vector[0].X11.output != primary)
        // finding if primary monitor changed
        for (size_t I = 0; I < Monitor_vector.size(); ++I)
        {
            if (Monitor_vector[I].X11.output == primary)
            {
                std::swap(Monitor_vector[0], Monitor_vector[I]);

                break;
            }
        }
}

bool Get_Gamma_ramp(const unsigned int Monitor_index,
                    ODS_Gamma_ramp    *gamma_ramp)
{
    FUNCTION_PROFILE
    const size_t size = XRRGetCrtcGammaSize(
        server.Main_Display, Monitor_vector[Monitor_index].X11.crtc);
    XRRCrtcGamma *gamma = XRRGetCrtcGamma(
        server.Main_Display, Monitor_vector[Monitor_index].X11.crtc);

    gamma_ramp->red =
        new unsigned short[size];  // std::calloc(size, sizeof(unsigned short));
    gamma_ramp->green =
        new unsigned short[size];  // std::calloc(size, sizeof(unsigned short));
    gamma_ramp->blue =
        new unsigned short[size];  // std::calloc(size, sizeof(unsigned short));
    gamma_ramp->size = size;
    memcpy(gamma_ramp->red, gamma->red, size * sizeof(unsigned short));
    memcpy(gamma_ramp->green, gamma->green, size * sizeof(unsigned short));
    memcpy(gamma_ramp->blue, gamma->blue, size * sizeof(unsigned short));

    XRRFreeGamma(gamma);
    return true;
}

void Set_Gamma_ramp(const unsigned int Monitor_index,
                    ODS_Gamma_ramp    *gamma_ramp)
{
    FUNCTION_PROFILE

    if (XRRGetCrtcGammaSize(server.Main_Display,
                            Monitor_vector[Monitor_index].X11.crtc) !=
        gamma_ramp->size)
    {
        OS::TERMINAL::OS_console_out(
            "X11: Gamma ramp size must match current ramp size",
            RED,
            BLACK,
            Bold);
        return;
    }

    XRRCrtcGamma *gamma = XRRAllocGamma(gamma_ramp->size);

    memcpy(
        gamma->red, gamma_ramp->red, gamma_ramp->size * sizeof(unsigned short));
    memcpy(gamma->green,
           gamma_ramp->green,
           gamma_ramp->size * sizeof(unsigned short));
    memcpy(gamma->blue,
           gamma_ramp->blue,
           gamma_ramp->size * sizeof(unsigned short));

    XRRSetCrtcGamma(
        server.Main_Display, Monitor_vector[Monitor_index].X11.crtc, gamma);
    XRRFreeGamma(gamma);
}
/// window creation and managment library
/// TODO: complete window creation and mangment library
// glx related functions
//
bool extensionSupportedGLX(std::string_view *extension, std::uint16_t Size)
{
    std::string_view extensions =
        glXQueryExtensionsString(server.Main_Display, server.screen);

    for (auto i = 0; i < Size; i++)
    {
        if (extensions.find(extension[i]) == std::string_view::npos)
        {
            return false;
        }
    }

    return true;
}

bool glxinit()
{
    if (!glXQueryExtension(server.Main_Display,
                           &Extension_codes.glx.errorBase,
                           &Extension_codes.glx.eventBase))
    {
        OS_console_out("GLX: GLX extension not found", RED, BLACK, Bold);
        return false;
    }
    int major = 0, minor = 0;
    if (!glXQueryVersion(server.Main_Display, &major, &minor))
    {
        // "GLX: Failed to query GLX version");
        return false;
    }

    if (major == 1 && minor < 3)
    {
        // "GLX: GLX version 1.3 is required");
        return false;
    }

    std::string_view extensions[] =

        {"GLX_ARB_multisample",
         "GLX_ARB_framebuffer_sRGB",
         "GLX_EXT_framebuffer_sRGB",
         "GLX_ARB_create_context",
         "GLX_ARB_create_context_robustness",
         "GLX_ARB_create_context_profile",
         "GLX_EXT_create_context_es2_profile",
         "GLX_ARB_create_context_no_error",
         "GLX_ARB_context_flush_control"};
    extensionSupportedGLX(extensions, 9);
    return true;
}

inline bool setGLXFrame_Buffer_Config(const ODS_Frame_Buffer_config &fb_config,
                                      GLXFBConfig &GLXfb_config)
{
    int          nativeCount{};
    GLXFBConfig *nativeConfigs = nullptr;
    // some sub  function calls
    auto GLX_FB_Config_attribute = [&](const int  &fb_index,
                                       const int &&attrib) -> int
    {
        int value;
        glXGetFBConfigAttrib(
            server.Main_Display, nativeConfigs[fb_index], attrib, &value);
        return value;
    };

    // here   we are setting the config for the framebuffer fron fb_config

    nativeConfigs =
        glXGetFBConfigs(server.Main_Display, server.screen, &nativeCount);
    if (!nativeConfigs || !nativeCount)
    {
        ALERT("GLX: No GLXFBConfigs returned");
        return false;
    }

    ODS_Frame_Buffer_config *usable_config =
        new ODS_Frame_Buffer_config[nativeCount];

    for (int i = 0; i < nativeCount; i++)
    {
        // Only consider RGBA GLXFBConfigs
        if (!(GLX_FB_Config_attribute(i, GLX_RENDER_TYPE) & GLX_RGBA_BIT))
            continue;

        // Only consider window GLXFBConfigs
        if (!(GLX_FB_Config_attribute(i, GLX_DRAWABLE_TYPE) & GLX_WINDOW_BIT))
        {
            if (trustWindowBit) continue;
        }

        if (GLX_FB_Config_attribute(i, GLX_DOUBLEBUFFER) !=
            desired->doublebuffer)
            continue;

        if (desired->transparent)
        {
            XVisualInfo *vi = glXGetVisualFromFBConfig(server.Main_Display, n);
            if (vi)
            {
                u->transparent = _glfwIsVisualTransparentX11(vi->visual);
                XFree(vi);
            }
        }

        usable_config[i].redBits = GLX_FB_Config_attribute(i, GLX_RED_SIZE);
        usable_config[i].greenBits = GLX_FB_Config_attribute(i, GLX_GREEN_SIZE);
        usable_config[i].blueBits = GLX_FB_Config_attribute(i, GLX_BLUE_SIZE);

        usable_config[i].alphaBits = GLX_FB_Config_attribute(i, GLX_ALPHA_SIZE);
        u->depthBits = GLX_FB_Config_attribute(i, GLX_DEPTH_SIZE);
        u->stencilBits = GLX_FB_Config_attribute(i, GLX_STENCIL_SIZE);

        u->accumRedBits = GLX_FB_Config_attribute(i, GLX_ACCUM_RED_SIZE);
        u->accumGreenBits = GLX_FB_Config_attribute(i, GLX_ACCUM_GREEN_SIZE);
        u->accumBlueBits = GLX_FB_Config_attribute(i, GLX_ACCUM_BLUE_SIZE);
        u->accumAlphaBits = GLX_FB_Config_attribute(i, GLX_ACCUM_ALPHA_SIZE);

        u->auxBuffers = GLX_FB_Config_attribute(n, GLX_AUX_BUFFERS);

        if (GLX_FB_Config_attribute(n, GLX_STEREO)) u->stereo = GLFW_TRUE;

        if (_glfw.glx.ARB_multisample)
            u->samples = GLX_FB_Config_attribute(n, GLX_SAMPLES);

        if (_glfw.glx.ARB_framebuffer_sRGB || _glfw.glx.EXT_framebuffer_sRGB)
            u->sRGB =
                GLX_FB_Config_attribute(n, GLX_FRAMEBUFFER_SRGB_CAPABLE_ARB);

        u->handle = (uintptr_t)n;
        usableCount++;
    }

    return true;
}

inline bool selectvisualGLX(const ODS_Window_config         &Win_config,
                            const ODS_Frame_Buffer_config   &fb_config,
                            const ODS_Opengl_Context_Config &opengl_config,
                            Visual                         **visual,
                            int                             &depth)
{
    GLXFBConfig  native;
    XVisualInfo *result;

    if (!setGLXFrame_Buffer_Config(fb_config, native))
    {
        ALERT("GLX: Failed to find a suitable GLXFBConfig");
        return false;
    }

    result = glXGetVisualFromFBConfig(server.Main_Display, native);
    if (!result)
    {
        ALERT("GLX: Failed to retrieve Visual for GLXFBConfig");
        return false;
    }

    *visual = result->visual;
    depth = result->depth;

    XFree(result);
    return true;
}
bool Create_Window_X11(const GRAPHICS_API_Flags       api,
                       const ODS_Window_config       &window_config,
                       const ODS_Frame_Buffer_config &framebuffer_config,
                       const std::variant<ODS_Opengl_Context_Config,
                                          ODS_Vulkan_Context_Config> api_config

)
{
    FUNCTION_PROFILE

    assert(api != GRAPHICS_API_DIRECTX12);
    assert(api != GRAPHICS_API_DIRECTX11);
    assert(api != GRAPHICS_API_DIRECTX9);
    assert(api != GRAPHICS_API_METAL6);
    assert(!window_config.title.empty());
    assert(window_config.width >= 0);
    assert(window_config.height >= 0);
    assert(Feature_available.server_init);

    Visual *visual = nullptr;
    int     depth;
    auto    setvisual_depth = [&]()
    {
        if (!visual)
        {
            visual = DefaultVisual(server.Main_Display, server.screen);
            depth = DefaultDepth(server.Main_Display, server.screen);
        }
    };
    switch (api)
    {
            // type of api for context creation

        case GRAPHICS_API_Flags::GRAPHICS_API_OPENGL:
        {  // glx initialization with visual creation
            if (glxinit())
            {
                if (selectvisualGLX(
                        window_config,
                        framebuffer_config,
                        std::get<ODS_Opengl_Context_Config>(api_config),
                        &visual,
                        depth))
                {
                    setvisual_depth();
                    if (!createNativeWindow(window_config, visual, depth))
                        return false;
                }
            }
            break;
        }
        case GRAPHICS_API_Flags::GRAPHICS_API_OPENGLES:
        {
            break;
        }
        case GRAPHICS_API_Flags::GRAPHICS_API_VULKAN:
        {
            break;
        }
        case GRAPHICS_API_Flags::GRAPHICS_API_NONE:
        {
            break;
        }
    }

    return true;
}

}  // namespace WINDOW

}  // namespace OS
*/
