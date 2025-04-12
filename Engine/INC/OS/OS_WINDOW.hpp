#pragma once
// standard files
#include <cstdint>
#include <string>
#include <string_view>
#include <variant>
#include <vector>
// OS files
#include <OS/LINUX/LINUX_WINDOW.hpp>

#define PLATFORM_MONITOR_STATE \
    LINUX_MONITOR_STATE  // WIN32_MONITOR_STATE COCOA_MONITOR_STATE

#define PLATFORM_WINDOW_STATE \
    LINUX_WINDOW_STATE  // WIN32_WINDOW_STATE COCOA_WINDOW_STATE

namespace OS
{
namespace WINDOW
{

enum class Error_Code
{
    SUCCESS,
    INITIALIZATIONFAILED,
    INVALIDARGUMENT,
    OUTOFMEMORY,
    PLATFORMNOTSUPPORTED
};
//

;
struct WINDOW
{
};
// driver api
enum GRAPHICS_API_Flags
{
    GRAPHICS_API_NONE,
    GRAPHICS_API_OPENGL,
    GRAPHICS_API_OPENGLES,
    GRAPHICS_API_VULKAN,
    GRAPHICS_API_DIRECTX9,
    GRAPHICS_API_DIRECTX11,
    GRAPHICS_API_DIRECTX12,
    GRAPHICS_API_METAL6,

};

enum WINDOW_FEATURE_FLAGS
{

    WINDOW_FEATURE_TRANSPARENT = 1 << 0,
    WINDOW_FEATURE_NOBORDER = 1 << 1,
    WINDOW_FEATURE_RESIZABLE = 1 << 2,
    WINDOW_FEATURE_VISIBLE = 1 << 3,
    WINDOW_FEATURE_MAXIMIZED = 1 << 4,
    WINDOW_FEATURE_CENTER_CURSOR = 1 << 5,
    WINDOW_FEATURE_FOCUS_ON_SHOW = 1 << 6,
    WINDOW_FEATURE_SCALE_TO_MONITOR = 1 << 7,
    WINDOW_FEATURE_STEREO = 1 << 8,
    WINDOW_FEATURE_SRGB_CAPABLE = 1 << 9,
    WINDOW_FEATURE_DOUBLE_BUFFER = 1 << 10,
    WINDOW_FEATURE_TRIPLE_BUFFER = 1 << 12,

};

enum WINDOW_type
{
    NATIVE_WINDOW,
    EGL_WINDOW,
    OMESA_WINDOW,

};

struct Videomode
{
    /*! The width, in screen coordinates, of the video mode.
     */
    int width;
    /*! The height, in screen coordinates, of the video mode.
     */
    int height;
    // /*! The bit depth of the red channel of the video mode.
    //  */
    // int redBits;
    // /*! The bit depth of the green channel of the video mode.
    //  */
    // int greenBits;
    // /*! The bit depth of the blue channel of the video mode.
    //  */
    // int blueBits;
    /*! The refresh rate, in Hz, of the video mode.
     */
    int refreshRate;
};

struct ODS_Gamma_ramp
{
    /*! An array of value describing the response of the red channel.
     */
    unsigned short *red;
    /*! An array of value describing the response of the green channel.
     */
    unsigned short *green;
    /*! An array of value describing the response of the blue channel.
     */
    unsigned short *blue;
    /*! The number of elements in each array.
     */
    unsigned int size;
};

struct ODS_Monitor
{
    PLATFORM_MONITOR_STATE
    std::string            name;
    int                    widthMM, heightMM;
    std::vector<Videomode> modes;
    unsigned int           current_mode;
    ODS_Gamma_ramp         original, current;
};

// Window configuration
//
// Parameters relating to the creation of the window but not directly related
// to the framebuffer.  This is used to pass window creation parameters from
// shared code to the platform API.
//
struct ODS_Window_config
{
    bool             resizable;
    bool             visible;
    bool             decorated;
    bool             focused;
    bool             autoIconify;
    bool             floating;
    bool             maximized;
    bool             centerCursor;
    bool             focusOnShow;
    bool             mousePassthrough;
    bool             scaleToMonitor;
    bool             scaleFramebuffer;
    int              xpos;
    int              ypos;
    int              width;
    int              height;
    std::string_view title;
    WINDOW_type      Type{WINDOW_type::NATIVE_WINDOW};
};

// Context configuration
//
// Parameters relating to the creation of the context but not directly related
// to the framebuffer.  This is used to pass context creation parameters from
// shared code to the platform API.
struct ODS_Vulkan_Context_Config
{
};

struct ODS_Opengl_Context_Config
{
    int  client;
    int  source;
    int  major;
    int  minor;
    bool forward;
    bool debug;
    bool noerror;
    int  profile;
    int  robustness;
    int  release;
    bool nsgl_offline;
};

// Framebuffer configuration
//
// This describes buffers and their sizes.  It also contains
// a platform-specific ID used to map back to the backend API object.
//
// It is used to pass framebuffer parameters from shared code to the platform
// API and also to enumerate and select available framebuffer configs.
//
struct ODS_Frame_Buffer_config
{
    int  redBits;
    int  greenBits;
    int  blueBits;
    int  alphaBits;
    int  depthBits;
    int  stencilBits;
    int  accumRedBits;
    int  accumGreenBits;
    int  accumBlueBits;
    int  accumAlphaBits;
    int  auxBuffers;
    bool stereo;
    int  samples;
    bool sRGB;
    bool doublebuffer;
    bool transparent;
};

struct window_size
{
    int minwidth, minheight;
    int maxwidth, maxheight;
};
struct ODS_Window
{
    std::vector<std::u8string> Title;
    std::vector<window_size>   size;
    std::vector<int>           numer, denom;
    PLATFORM_WINDOW_STATE
};

bool OS_Create_Window(GRAPHICS_API_Flags      api,
                      ODS_Window_config      &window_config,
                      ODS_Frame_Buffer_config framebuffer_config,
                      std::variant<ODS_Opengl_Context_Config,
                                   ODS_Vulkan_Context_Config> api_config

);
// all available functions calls related to operating system
//
bool OS_Window_Init();
// HACK: os specific memory allocation
//  alloc

bool OS_Destroy_Window(WINDOW &);

std::uint64_t get_available_memoy();
}  // namespace WINDOW

}  // namespace OS
