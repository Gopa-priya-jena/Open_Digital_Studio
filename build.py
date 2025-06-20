import os
import platform
import sys

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
    "-DTRACY_SYMBOL_OFFLINE_RESOLVE=ON "
    "-DTRACY_LIBBACKTRACE_ELF_DYNLOAD_SUPPORT=OFF "
    "-DTRACY_DEBUGINFOD=ON "
    "-DTRACY_VERBOSE=ON "
    "-DTRACY_DEMANGLE=OFF "
)
# """
source = " -S .. "
build_cmd = "cmake " + source + " -DCMAKE_BUILD_TYPE=DEBUG " + tracy_config
test_cmd = "cmake " + source + "-DTEST=ON -DCMAKE_BUILD_TYPE=DEBUG " + tracy_config
system_name = platform.system().lower()


def clear_terminal():
    if system_name == "windows":
        os.system("cls")
    else:  # Linux, macOS, etc.
        os.system("clear")


def build_dir():
    if system_name == "windows":
        return "build_windows"
    else:
        return "build_linux"


def test_dir():
    if system_name == "windows":
        return "test_build_windows"
    else:
        return "test_build_linux"


def build():
    dir = build_dir()
    if not os.path.exists(dir):
        os.mkdir(dir)
    else:
        os.chdir(dir)
    print("PRESENT DIRECTOR IS ", os.getcwd())
    if os.system(build_cmd) == 0:
        print("CMAKE SUCCEED \n")
        os.system(" mv compile_commands.json ..")
        print("\n========================= RUNNING MAKE ===================== \n")
        if os.system("cmake --build . --clean-first -j256") == 0:
            print("COMPILATION SUCCEED \n")
            print("RUNNING APP  \n")
            if os.path.exists("./Open_Digital_Studio"):
                print("build sucessfull")
                os.execlp("./Open_Digital_Studio", "./Open_Digital_Studio")
            else:
                print("build failed")
        else:
            print("cmake  build command failed")
    else:
        print("cmake failed")
    return


def test():
    dir = test_dir()
    if not os.path.exists(dir):
        os.mkdir(dir)
    else:
        os.chdir(dir)
    print("PRESENT DIRECTOR IS ", os.getcwd())
    if os.system(test_cmd) == 0:
        print("CMAKE SUCCEED \n")
        os.system(" mv compile_commands.json ..")
        print("\n========================= RUNNING MAKE ===================== \n")
        if os.system("cmake --build . --clean-first -j256") == 0:
            print("COMPILATION SUCCEED \n")
            print("RUNNING APP  \n")
            if os.path.exists("./test"):
                print("build sucessfull")
                os.execlp("./test", "./test")
            else:
                print("build failed")
        else:
            print("cmake  build command failed")
    else:
        print("cmake failed")
    return


def rapid():
    ## rapid devlopment
    return


def command():
    n = len(sys.argv)
    if n == 2:
        cmd = sys.argv[1]
    else:
        cmd = 0
        cmd = int(input("Enter action\n1. Build \n2. Test\n "))
    if cmd == 1 or cmd == 0:
        build()
    elif cmd == 2:
        test()
    elif cmd == 3:
        rapid()


command()
