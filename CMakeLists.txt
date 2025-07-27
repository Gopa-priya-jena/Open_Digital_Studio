cmake_minimum_required(VERSION 4.0.0)

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
# if(MSVC)
#     target_compile_options(<target> PUBLIC "/ZI")
#     target_link_options(<target> PUBLIC "/INCREMENTAL")
# endif()
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
    enable_testing()
    add_subdirectory(Test)
endif()

add_executable(Open_Digital_Studio ${CMAKE_CURRENT_SOURCE_DIR}/APP/MAIN.cpp)
target_include_directories(Open_Digital_Studio PUBLIC ${subdirectory_list})
target_link_libraries(
    Open_Digital_Studio
    Open_Digital_Studio_Engine
    DEBUG_Profiler
)
