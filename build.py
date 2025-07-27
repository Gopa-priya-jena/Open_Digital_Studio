#!/usr/bin/env python

import os
import platform
import sys

system_name = platform.system().lower()
# tracy related config
# (TRACY_ENABLE "Enable profiling"
# (TRACY_ON_DEMAND "On-demand profiling"
# (TRACY_CALLSTACK "Enforce callstack collection for tracy regions"
# (TRACY_NO_CALLSTACK "Disable all callstack related functionality"
# (TRACY_NO_CALLSTACK_INLINES "Disables the inline functions in callstacks"
# (TRACY_ONLY_LOCALHOST "Only listen on the localhost interface"
# (TRACY_NO_BROADCAST "Disable client discovery by broadcast to local network"
# (TRACY_ONLY_IPV4 "Tracy will only accept connections on IPv4 addresses (disable IPv6)"
# (TRACY_NO_CODE_TRANSFER "Disable collection of source code"
# (TRACY_NO_CONTEXT_SWITCH "Disable capture of context switches"
# (TRACY_NO_EXIT "Client executable does not exit until all profile data is sent to server"
# (TRACY_NO_SAMPLING "Disable call stack sampling"
# (TRACY_NO_VERIFY "Disable zone validation for C API"
# (TRACY_NO_VSYNC_CAPTURE "Disable capture of hardware Vsync events"
# (TRACY_NO_FRAME_IMAGE  "Disable the frame image support and its thread"
# (TRACY_NO_SYSTEM_TRACING  "Disable systrace sampling"
# (TRACY_PATCHABLE_NOPSLEDS  "Enable nopsleds for efficient patching by system-level tools (e.g. rr)"
# (TRACY_DELAYED_INIT "Enable delayed initialization of the library (init on first call)"
# (TRACY_MANUAL_LIFETIME "Enable the manual lifetime management of the profile"
# (TRACY_FIBERS "Enable fibers support"
# (TRACY_NO_CRASH_HANDLER "Disable crash handling"
# (TRACY_TIMER_FALLBACK "Use lower resolution timers"
# (TRACY_LIBUNWIND_BACKTRACE "Use libunwind backtracing where supported"
# (TRACY_SYMBOL_OFFLINE_RESOLVE "Instead of full runtime symbol resolution, only resolve the image path and offset to enable offline symbol resolution"
# (TRACY_LIBBACKTRACE_ELF_DYNLOAD_SUPPORT "Enable libbacktrace to support dynamically loaded elfs in symbol resolution resolution after the first symbol resolve operation"
# (TRACY_DEBUGINFOD "Enable debuginfod support"


tracy_config = (
    "-DTRACY_ENABLE=OFF "  #  # (TRACY_ENABLE "Enable profiling
    "-DTRACY_ON_DEMAND=ON "  #  # (TRACY_ON_DEMAND "On-demand profiling"
    "-DTRACY_CALLSTACK=ON "  #  # (TRACY_CALLSTACK "Enforce callstack collection for tracy regions"
    "-DTRACY_NO_CALLSTACK=OFF "  #  # (TRACY_NO_CALLSTACK "Disable all callstack related functionality"
    "-DTRACY_NO_CALLSTACK_INLINES=OFF "  #  # (TRACY_NO_CALLSTACK_INLINES "Disables the inline functions in callstacks"
    "-DTRACY_ONLY_LOCALHOST=ON "  #  # (TRACY_ONLY_LOCALHOST "Only listen on the localhost interface"
    "-DTRACY_NO_BROADCAST=OFF "  #  # (TRACY_NO_BROADCAST "Disable client discovery by broadcast to local network"
    "-DTRACY_ONLY_IPV4=OFF "  #  # (TRACY_ONLY_IPV4 "Tracy will only accept connections on IPv4 addresses (disable IPv6)"
    "-DTRACY_NO_CODE_TRANSFER=OFF "  #  # (TRACY_NO_CODE_TRANSFER "Disable collection of source code"
    "-DTRACY_NO_CONTEXT_SWITCH=OFF "  #  # (TRACY_NO_CONTEXT_SWITCH "Disable capture of context switches"
    "-DTRACY_NO_EXIT=OFF "  #  # (TRACY_NO_EXIT "Client executable does not exit until all profile data is sent to server"
    "-DTRACY_NO_SAMPLING=ON "  #  # (TRACY_NO_SAMPLING "Disable call stack sampling"
    "-DTRACY_NO_VERIFY=OFF "  #  # (TRACY_NO_VERIFY "Disable zone validation for C API"
    "-DTRACY_NO_VSYNC_CAPTURE=OFF "  #  # (TRACY_NO_VSYNC_CAPTURE "Disable capture of hardware Vsync events"
    "-DTRACY_NO_FRAME_IMAGE=OFF "  #  # (TRACY_NO_FRAME_IMAGE  "Disable the frame image support and its thread"
    "-DTRACY_NO_SYSTEM_TRACING=OFF "  #  # (TRACY_NO_SYSTEM_TRACING  "Disable systrace sampling"
    "-DTRACY_PATCHABLE_NOPSLEDS=ON "  #  # (TRACY_PATCHABLE_NOPSLEDS  "Enable nopsleds for efficient patching by system-level tools (e.g. rr)"
    "-DTRACY_DELAYED_INIT=ON "  #  # (TRACY_DELAYED_INIT "Enable delayed initialization of the library (init on first call)"
    "-DTRACY_MANUAL_LIFETIME=OFF "  #  # (TRACY_MANUAL_LIFETIME "Enable the manual lifetime management of the profile"
    "-DTRACY_FIBERS=ON "  #  # (TRACY_FIBERS "Enable fibers support"
    "-DTRACY_NO_CRASH_HANDLER=ON "  #  # (TRACY_NO_CRASH_HANDLER "Disable crash handling"
    "-DTRACY_TIMER_FALLBACK=OFF "  #  # (TRACY_TIMER_FALLBACK "Use lower resolution timers"
    "-DTRACY_LIBUNWIND_BACKTRACE=ON "  #  # (TRACY_LIBUNWIND_BACKTRACE "Use libunwind backtracing where supported"
    "-DTRACY_SYMBOL_OFFLINE_RESOLVE=ON "  #  # (TRACY_SYMBOL_OFFLINE_RESOLVE "Instead of full runtime symbol resolution, only resolve the image path and offset to enable offline symbol resolution"
    "-DTRACY_LIBBACKTRACE_ELF_DYNLOAD_SUPPORT=ON "  #  # (TRACY_LIBBACKTRACE_ELF_DYNLOAD_SUPPORT "Enable libbacktrace to support dynamically loaded elfs in symbol resolution resolution after the first symbol resolve operation"
    "-DTRACY_DEBUGINFOD=ON "  #  # (TRACY_TIMER_FALLBACK "Use lower resolution timers"
    "-DTRACY_VERBOSE=ON "  #
    "-DTRACY_DEMANGLE=OFF "  #
)


# mingw settings
mingw_win32_linux_settings = (
    """   -DCMAKE_SYSTEM_NAME=Windows """
    + """ -DCMAKE_SYSTEM_PROCESSOR=x86_64 """
    + """ -DCMAKE_CXX_COMPILER="/usr/bin/x86_64-w64-mingw32-g++" """
    + """ -DCMAKE_C_COMPILER="/usr/bin/x86_64-w64-mingw32-gcc"  """
    + """ -DCMAKE_RC_COMPILER="/usr/bin/x86_64-w64-mingw32-windres" """
    + """ -DCMAKE_FIND_ROOT_PATH="/usr/x86_64-w64-mingw32" """
    + """ -DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=NEVER """
    + """ -DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY"""
    + """ -DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY"""
    + """ -DCMAKE_CXX_FLAGS=" -std=c++23 -lstdc++exp" """
)

# generator selaction


def generator():
    response = input("Do you want to build via ninja  Y/N")
    if response == "Y" or response == "y":
        return " -G Ninja "
    else:
        return " "


#
def source():
    return "  .. "


# buid  commmand
# build_cmd = "cmake " + source + generator + " -DCMAKE_BUILD_TYPE=DEBUG " + tracy_config


# test commmand
test_cmd = (
    "cmake "
    + source()
    # + generator()
    + "-DTEST=ON -DCMAKE_BUILD_TYPE=DEBUG "
    + tracy_config
)  # + tracy_config


# mingw build command
build_cmd_mingw = (
    "x86_64-w64-mingw32-cmake  "
    + source()
    + mingw_win32_linux_settings
    + " -DTEST=ON -DCMAKE_BUILD_TYPE=DEBUG "
    + tracy_config
    + """ -DCMAKE_CXX_FLAGS=" -std=c++23 -lstdc++exp -g -fno-omit-frame-pointer --export-dynmaic -rdynamic -fsanitize=address " """
)


def clear_terminal():
    if system_name == "windows":
        os.system("cls")
    else:  # Linux, macOS, etc.
        os.system("clear")


def move(file, location):
    location = os.path.abspath(location)
    if system_name == "linux":
        cmd = "mv " + file + " " + location
        os.system(cmd)
    else:
        cmd = "move /Y " + file + " " + location
        # os.system(cmd)
        return


def build_dir():
    if system_name == "windows":
        return "build_windows"
    else:
        return "build_linux"


def build_cmd():
    cmd = (
        "cmake "
        + source()
        + """ -DCMAKE_CXX_FLAGS=" -std=c++23 -lstdc++exp -g -fno-omit-frame-pointer  " """
    )

    response = input("\nDo you want to build tests  Y/N \n")
    if response == "Y" or response == "y" or response == "":
        cmd = cmd + " -DTEST=ON -DCMAKE_BUILD_TYPE=DEBUG "
    cmd = cmd + tracy_config
    if system_name != "linux":
        cmd = cmd + generator()
    return cmd


def execute():
    clear_terminal()
    response = input("\nDo you want to run tests  Y/N \n")
    if response == "Y" or response == "y" or response == "":
        if os.path.exists("./Unit_Tests"):
            print("Test is  build sucessfull")
            os.execlp("./Unit_Tests", "./Unit_Tests")
        else:
            print("Test is not build")
    # clear_terminal()
    response = input("\nDo you want to run app  Y/N \n")
    if response == "Y" or response == "y":
        if os.path.exists("./Open_Digital_Studio"):
            print(" ODS build sucessfull")
            os.execlp("./Open_Digital_Studio", "./Open_Digital_Studio")
        else:
            print("build failed")
    return


def build():
    dir = build_dir()
    if not os.path.exists(dir):
        os.mkdir(dir)
    os.chdir(dir)
    print("PRESENT DIRECTOR IS ", os.getcwd())
    cmd = build_cmd()
    if os.system(cmd) == 0:
        print("CMAKE SUCCEED \n")
        # os.system(" mv compile_commands.json ..")
        move("compile_commands.json", "..")
        clear_terminal()
        print("\n========================= RUNNING MAKE ===================== \n")
        if os.system("cmake --build . --clean-first -j256") == 0:
            print("COMPILATION SUCCEED \n")
            print("Executing ............  \n")
            clear_terminal()
            execute()
        else:
            print("cmake  build command failed")
    else:
        print("cmake failed")
    return


def build_mingw():
    clear_terminal()
    dir = "mingw_build_test"
    if not os.path.exists(dir):
        os.mkdir(dir)
    os.chdir(dir)
    print("PRESENT DIRECTOR IS ", os.getcwd())
    print(build_cmd_mingw)
    if os.system(build_cmd_mingw) == 0:
        print("CMAKE SUCCEED \n")
        os.system(" mv compile_commands.json ..")
        clear_terminal()
        print("\n========================= RUNNING MAKE ===================== \n")
        if os.system("cmake --build . -j64") == 0:
            print("COMPILATION SUCCEED \n")
            print("RUNNING APP  \n")
            if os.path.exists("./test"):
                print("build sucessfull")
                os.system("x86_64-w64-mingw32-wine test.exe")
            else:
                print("build failed")
        else:
            print("cmake  build command failed")
    else:
        print("cmake failed")
    return


def command():
    clear_terminal()
    n = len(sys.argv)
    if n == 2:
        cmd = int(sys.argv[1])
    else:
        cmd = 0
        clear_terminal()
        cmd = int(
            input("Enter action\n1. Build \n2. Mingw_win32\n\n\n Your Response: ")
        )
    if cmd == 1 or cmd == 0:
        build()
    elif cmd == 2:
        build_mingw()


command()
