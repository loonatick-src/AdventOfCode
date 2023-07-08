cmake_minimum_required( VERSION 3.12...3.26 ) 

project ( AoC2017 VERSION 0.1
    DESCRIPTION "Advent of Code 2017"
    LANGUAGES CXX )

include( FetchContent )
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 10.0.0
)
FetchContent_Declare(
    gtest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.13.0
)

FetchContent_MakeAvailable( fmt )
FetchContent_MakeAvailable( gtest )

set( SRC_DIR src )
set( BOOST_IOSTREAMS_SOURCE_FILE lib/boost/iostreams/mapped_file.cpp )
file( GLOB SRC_FILES ${SRC_DIR}/*.cpp )
message( "bitch, source files are ${SRC_FILES}" )
foreach( SOURCE_FILE ${SRC_FILES} )
    get_filename_component( EXECUTABLE_NAME ${SOURCE_FILE} NAME_WLE )
    add_executable( ${EXECUTABLE_NAME} ${SOURCE_FILE} ${BOOST_IOSTREAMS_SOURCE_FILE} )
    target_compile_features( ${EXECUTABLE_NAME} PUBLIC cxx_std_17 )
    target_link_libraries( ${EXECUTABLE_NAME} fmt )
endforeach( SOURCE_FILE ${SOURCE_FILES} )

# TODO add googletest etc
