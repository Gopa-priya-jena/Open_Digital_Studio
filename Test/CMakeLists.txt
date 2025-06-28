cmake_minimum_required(VERSION 4.0.0)
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
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}../Engine/INC
    ${CMAKE_CURRENT_SOURCE_DIR}../Engine/External
    ${CMAKE_CURRENT_SOURCE_DIR}../Engine/External/Debug_profiler
)
# add_subdirectory(../Engine)
set(subdirectory_list)
list(
    APPEND
    subdirectory_list
    ${CMAKE_CURRENT_SOURCE_DIR}../Engine/INC/
    ${CMAKE_CURRENT_SOURCE_DIR}../Engine/External/Debug_profiler/inc/
)

add_executable(test test.cpp)
target_include_directories(test PUBLIC ${subdirectory_list})
target_link_libraries(test Open_Digital_Studio_Engine DEBUG_Profiler)
