#pragma once
#include <Define.hpp>
#if ODS_LINUX

#include <OS/LINUX/LINUX_X11_Platform.hpp>
// this is needed as  linux has two display server x11 nad wayland
//

namespace OS
{
namespace WINDOW
{

#define LINUX_MONITOR_STATE X11_MONITOR_STATE  // WAYLAND_WINDOW_STATE
#define LINUX_WINDOW_STATE X11_WINDOW_STATE    // WAYLAND_WINDOW_STATE
#define STACK_ALLOC_(DATA_STRUCTURE, NUMBER_OF_DATA_STRUTURE) \
    alloca(sizeof(DATA_STRUCTURE) * (NUMBER_OF_DATA_STRUTURE));
#define STACK_ALLOC_BYTES(X) alloca(X);
enum class Display_Server : short
{
    X11,
    WAYLAND,
    NONE

};

}  // namespace WINDOW
}  // namespace OS
//
#endif
