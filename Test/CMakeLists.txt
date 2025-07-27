cmake_minimum_required(VERSION 4.0.0)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_CXX_STANDARD 23)
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    message(
        "+++++++++++++++++++++++CCACHE FOUND++++++++++++++++++++++++++++++++++++++++++++++"
    )
endif()
function(file_list NAME)
    message("\n THE LIST OF ${NAME} FILES ARE \n")
    message(
        "----------------------------------------------------------------------------------------------------------------------------------------------"
    )
    foreach(item ${ARGN})
        message(${item})
    endforeach()
endfunction()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}../Engine/INC
    ${CMAKE_CURRENT_SOURCE_DIR}../Engine/External
    ${CMAKE_CURRENT_SOURCE_DIR}../Engine/External/Debug_profiler
    ${CMAKE_CURRENT_SOURCE_DIR}/External/GTest
)

add_subdirectory(External/GTest)
set(subdirectory_list)
list(
    APPEND
    subdirectory_list
    ${CMAKE_CURRENT_SOURCE_DIR}../Engine/INC/
    ${CMAKE_CURRENT_SOURCE_DIR}../Engine/External/Debug_profiler/inc/
)
file(
    GLOB_RECURSE Test_files
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/Tests/*.cpp
)

file_list("SOURCE FILES ARE" ${Test_files} Test.cpp)

add_executable(Unit_Tests ${Test_files} Test.cpp)
target_include_directories(Unit_Tests PUBLIC ${subdirectory_list})
target_link_libraries(
    Unit_Tests
    Open_Digital_Studio_Engine
    DEBUG_Profiler
    GTest::gtest_main
)
add_test(NAME TESTS@UNIT_TEST COMMAND Unit_Tests)
