#!/usr/bin/env python

import os
import platform
import sys

system_name = platform.system().lower()
# tracy related config

tracy_config = (
    "-DTRACY_ENABLE=OFF "
    "-DTRACY_ON_DEMAND=OFF "
    "-DTRACY_CALLSTACK=OFF "
    "-DTRACY_NO_CALLSTACK=OFF "
    "-DTRACY_NO_CALLSTACK_INLINES=OFF "
    "-DTRACY_ONLY_LOCALHOST=ON "
    "-DTRACY_NO_BROADCAST=OFF "
    "-DTRACY_ONLY_IPV4=OFF "
    "-DTRACY_NO_CODE_TRANSFER=ON "
    "-DTRACY_NO_CONTEXT_SWITCH=ON "
    "-DTRACY_NO_EXIT=OFF "
    "-DTRACY_NO_SAMPLING=ON "
    "-DTRACY_NO_VERIFY=OFF "
    "-DTRACY_NO_VSYNC_CAPTURE=OFF "
    "-DTRACY_NO_FRAME_IMAGE=OFF "
    "-DTRACY_NO_SYSTEM_TRACING=OFF "
    "-DTRACY_PATCHABLE_NOPSLEDS=OFF "
    "-DTRACY_DELAYED_INIT=OFF "
    "-DTRACY_MANUAL_LIFETIME=OFF "
    "-DTRACY_FIBERS=OFF "
    "-DTRACY_NO_CRASH_HANDLER=OFF "
    "-DTRACY_TIMER_FALLBACK=OFF "
    "-DTRACY_LIBUNWIND_BACKTRACE=OFF "
    "-DTRACY_SYMBOL_OFFLINE_RESOLVE=OFF "
    "-DTRACY_LIBBACKTRACE_ELF_DYNLOAD_SUPPORT=OFF "
    "-DTRACY_DEBUGINFOD=ON "
    "-DTRACY_VERBOSE=ON "
    "-DTRACY_DEMANGLE=OFF "
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
    cmd = "cmake " + source()
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
        if os.path.exists("./test"):
            print("Test is  build sucessfull")
            os.execlp("./test", "./test")
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
