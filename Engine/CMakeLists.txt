cmake_minimum_required(VERSION 4.0.0)
project(
    "Open_Digital_Studio_Engine"
    VERSION 1
    DESCRIPTION "Open_Digital_Studio is a game Engine"
    LANGUAGES CXX
)

# set compilation option
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS ON)

if(CCACHE_FOUND)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
  message(
        "+++++++++++++++++++++++CCACHE FOUND++++++++++++++++++++++++++++++++++++++++++++++"
    )
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  set(CMAKE_CXX_FLAGS "-march=native -mavx2")
endif()

# some function that helps in devlopment proceess
function(file_list NAME)
  message("\n THE LIST OF ${NAME} FILES ARE \n")
  message(
        "----------------------------------------------------------------------------------------------------------------------------------------------"
    )
  foreach(item ${ARGN})
    message(${item})
  endforeach()
endfunction()

# find os level packages

set(INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/External/debug_profiler/
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine/INC/
)
set(LIBRARIES)

find_package(OpenGL REQUIRED)
find_package(Vulkan REQUIRED)

if(Vulkan_FOUND)
  list(APPEND INCLUDE_DIRS ${Vulkan_INCLUDE_DIRS})
  list(APPEND LIBRARIES Vulkan::Vulkan) # Modern CMake target
endif()
# os specific packages

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  # Check for huge page support
  find_library(HUGETLBFS hugetlbfs)
  find_package(X11 REQUIRED)
  if(HUGETLBFS)
    add_definitions(-DUSE_HUGETLB)
    message("HugeTLB support enabled")
  endif()
  # adding support x11

  if(X11_FOUND)
    message("Configuring X11 for Linux")
    list(
            APPEND
            INCLUDE_DIRS
            ${X11_INCLUDE_DIR}
            ${X11_Xrandr_INCLUDE_PATH}
            ${X11_Xrender_INCLUDE_PATH}
            ${X11_Xxf86vm_INCLUDE_PATH}
            ${X11_Xi_INCLUDE_PATH}
            ${X11_Xinerama_INCLUDE_PATH}
        )
    list(
            APPEND
            LIBRARIES
            ${X11_LIBRARIES}
            ${X11_Xrandr_LIB}
            ${X11_Xrender_LIB}
            ${X11_Xxf86vm_LIB}
            ${X11_Xi_LIB}
            ${X11_Xinerama_LIB}
        )
  endif()
  find_library(OPENGL_LIB GL)
  if(OPENGL_LIB AND EXISTS "${X11_INCLUDE_DIR}/GL/glx.h")
    list(APPEND LIBRARIES ${OPENGL_LIB})
    message(STATUS "GLX support enabled with OpenGL")
  else()
    message(WARNING "GLX or OpenGL not fully supported")
  endif()
endif()

# ADDING ODS source code
file(
    GLOB_RECURSE SOURCE_file
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/SRC/*.cpp
)
file_list("SOURCE FILES ARE" ${SOURCE_file})
# ADDING ODS HEAADER FILES
file(
    GLOB_RECURSE HEADER_file
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/INC/*.hpp
)

file_list("HEADER FILES ARE" ${HEADER_file})

set(SOURCE_files)

list(APPEND SOURCE_files ${HEADER_file} ${SOURCE_file})
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/External/Debug_profiler)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/External/Debug_profiler/inc/)

# adding External packages
add_library(Open_Digital_Studio_Engine ${SOURCE_files})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/INC/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/External/Debug_profiler/)

# Adding subdirectories
# add_subdirectory(External/Third_party)
target_include_directories(Open_Digital_Studio_Engine PRIVATE ${INCLUDE_DIRS})
target_link_libraries(
    Open_Digital_Studio_Engine
    PRIVATE ${LIBRARIES}
    PUBLIC DEBUG_Profiler
)
