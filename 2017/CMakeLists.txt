cmake_minimum_required( VERSION 3.12...3.26 ) 

project ( AoC2017 VERSION 0.1
    DESCRIPTION "Advent of Code 2017"
    LANGUAGES CXX )

find_package( boost REQUIRED )

include( FetchContent )
FetchContent_Declare(
    fmtlib
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 10.0.0
)
FetchContent_MakeAvailable( fmtlib )
FetchContent_Declare(
    gtest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.13.0
)
FetchContent_MakeAvailable( gtest )

set( SRC_DIR src )
set( BOOST_IOSTREAMS_SOURCE_FILE lib/boost/iostreams/mapped_file.cpp )
file( GLOB SOURCE_FILES ${SOURCE_DIR}/*.cpp )
foreach( SOURCE_FILE ${SOURCE_FILES} )
    get_filename_component( EXECUTABLE_NAME ${SOURCE_FILE} NAME_WLE )
    add_executable( ${EXECUTABLE_NAME} ${SOURCE_FILE} ${BOOST_IOSTREAMS_SOURCE_FILE} )
    target_compiler_features( ${EXECUTABLE_NAME} PUBLIC cxx_std_17 )
    target_link_libraries( ${ EXECUTABLE_NAME } fmtlib )
endforeach()

# TODO add googletest etc
