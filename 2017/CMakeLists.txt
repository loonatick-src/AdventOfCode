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


set( SRC_PATTERN "day[0-9]+\\.cpp" )
set( SRC_DIR src )
set( INC_DIR include )
set( BOOST_IOSTREAMS_SRC_FILE lib/boost/iostreams/mapped_file.cpp )

file( GLOB INC_FILES ${INC_DIR}/*.hpp )
file( GLOB SRC_FILES ${SRC_DIR}/*.cpp )

# this is not modern CMake now is it?
set( UTIL_SRC_FILES "" )
foreach( SRC_FILE ${SRC_FILES} )
    string( REGEX MATCH ${SRC_PATTERN} MATCHED ${SRC_FILE} )
    if ( NOT MATCHED )
        set( UTIL_SRC_FILES "${UTIL_SRC_FILES};${SRC_FILE}" )
    endif()
endforeach()

foreach( SRC_FILE ${SRC_FILES} )
    string( REGEX MATCH ${SRC_PATTERN} MATCHED ${SRC_FILE} )
    if ( MATCHED )
        get_filename_component( EXECUTABLE_NAME ${SRC_FILE} NAME_WLE )
        add_executable( ${EXECUTABLE_NAME} ${SRC_FILE} ${UTIL_SRC_FILES} ${BOOST_IOSTREAMS_SRC_FILE} )
        target_include_directories( ${EXECUTABLE_NAME} PRIVATE ${INC_DIR} )
        target_compile_features( ${EXECUTABLE_NAME} PUBLIC cxx_std_17 )
        target_link_libraries( ${EXECUTABLE_NAME} fmt )
    endif()
endforeach()

# TODO add googletest etc
