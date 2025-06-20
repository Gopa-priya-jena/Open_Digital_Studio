cmake_minimum_required(VERSION 4.0.0)
# set(CMAKE_CXX_COMPILER /usr/bin/clang++)
# set(CMAKE_C_COMPILER /usr/bin/clang)

# set(CMAKE_CXX_COMPILER /usr/bin/g++)
# set(CMAKE_C_COMPILER /usr/bin/gcc)
project(
    "Open_Digital_Studio"
    VERSION 1
    DESCRIPTION "Open_Digital_Studio is a game Engine"
    LANGUAGES CXX C
)
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    message(
        "+++++++++++++++++++++++CCACHE FOUND++++++++++++++++++++++++++++++++++++++++++++++"
    )
endif()
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine/INC
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine/External
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine/External/Debug_profiler
)
add_subdirectory(Engine)
set(subdirectory_list)
list(
    APPEND
    subdirectory_list
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine/INC/
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine/External/Debug_profiler/inc/
)

if(TEST)
    add_executable(test test/test.cpp)
    target_include_directories(test PUBLIC ${subdirectory_list})
    target_link_libraries(test Open_Digital_Studio_Engine DEBUG_Profiler)

    #target_link_options(test
    #BEFORE PUBLIC -fsanitize=undefined PUBLIC -fsanitize=address )
    #
else()
    add_executable(Open_Digital_Studio ${CMAKE_CURRENT_SOURCE_DIR}/APP/MAIN.cpp)
    target_include_directories(Open_Digital_Studio PUBLIC ${subdirectory_list})
    target_link_libraries(
        Open_Digital_Studio
        Open_Digital_Studio_Engine
        DEBUG_Profiler
    )
    #   target_link_options(Open_Digital_Studio
    #   BEFORE PUBLIC -fsanitize=undefined PUBLIC -fsanitize=address
    # )
endif()
