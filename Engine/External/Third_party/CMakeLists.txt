# include(FetchContent)

# function(Get_Project)
#   cmake_parse_arguments(FETCH_SOURCE "" "NAME;URL;DESTINATION" "" ${ARGN})
#   FetchContent_Populate(
#     ${FETCH_SOURCE_NAME}
#     URL ${FETCH_SOURCE_URL}
#     SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${FETCH_SOURCE_DESTINATION})
#   set("${FETCH_SOURCE_NAME}_LOCATION"
#       ${FETCH_SOURCE_DESTINATION}
#       PARENT_SCOPE)
# endfunction()
#
# get_project(
#   NAME Tracy URL https://github.com/wolfpld/tracy/archive/refs/tags/v0.11.1.zip
#   DESTINATION Tracy)

# Core Tracy options
option(TRACY_ENABLE "Enable Tracy profiler integration" ON)
option(TRACY_ON_DEMAND "Enable on-demand profiling mode" ON)
option(TRACY_NO_EXIT "Prevent Tracy from exiting on disconnect" ON)

# Callstack configuration
option(TRACY_CALLSTACK "Enable callstack collection" ON)
option(TRACY_NO_CALLSTACK "Disable callstack collection" OFF)
option(TRACY_NO_CALLSTACK_INLINES "Disable inlined functions in callstacks" OFF)

# Network configuration
option(TRACY_ONLY_LOCALHOST "Restrict Tracy to localhost connections only" ON)
option(TRACY_NO_BROADCAST "Disable broadcast client discovery" OFF)
option(TRACY_ONLY_IPV4 "Use only IPv4 connections" OFF)

# Feature toggles
option(TRACY_NO_CODE_TRANSFER "Disable source code transfer to profiler" OFF)
option(TRACY_NO_CONTEXT_SWITCH "Disable context switch profiling" OFF)
option(TRACY_NO_SAMPLING "Disable sampling" OFF)
option(TRACY_NO_VERIFY "Disable verification of data" OFF)
option(TRACY_NO_VSYNC_CAPTURE "Disable VSync capture" ON)
option(TRACY_NO_FRAME_IMAGE "Disable frame image capture" OFF)
option(TRACY_NO_SYSTEM_TRACING "Disable system-wide tracing" OFF)

# Advanced features
option(TRACY_PATCHABLE_NOPSLEDS "Enable patchable NOPs" ON)
option(TRACY_DELAYED_INIT "Enable delayed initialization" ON)
option(TRACY_MANUAL_LIFETIME "Enable manual lifetime management" OFF)
option(TRACY_FIBERS "Enable fiber support" ON)
option(TRACY_NO_CRASH_HANDLER "Disable crash handler" OFF)
option(TRACY_TIMER_FALLBACK "Enable timer fallback" ON)

# Debug and symbol handling
option(TRACY_LIBUNWIND_BACKTRACE "Use libunwind for backtraces" OFF)
option(TRACY_SYMBOL_OFFLINE_RESOLVE "Enable offline symbol resolution" ON)
option(TRACY_LIBBACKTRACE_ELF_DYNLOAD_SUPPORT
       "Enable ELF dynamic loading support" ON)
option(TRACY_VERBOSE "Enable verbose output" ON)
option(TRACY_DEMANGLE "Enable symbol demangling" OFF)

add_subdirectory(tracy)
MESSAGE("FOUND TRACY \n")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/tracy/public)
target_include_directories(Open_Digital_Studio_Engine PUBLIC
                            ${CMAKE_CURRENT_SOURCE_DIR}/tracy/public)
target_sources(Open_Digital_Studio_Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/tracy/public/TracyClient.cpp)
target_link_libraries(Open_Digital_Studio_Engine PUBLIC Tracy::TracyClient)
