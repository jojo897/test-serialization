cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(boost-serialization-test VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set( INCLUDE_DIR "${PROJECT_SOURCE_DIR}/src" )
set( SOURCE_DIR  "${PROJECT_SOURCE_DIR}/src" )

#
# Configuration
#

if( ${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_WIN32_WINNT=0x0601 /W4 /wd4068 /wd4702" )
endif ( )

#if( NOT WIN32 )
#    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Weffc++ -pedantic -Wno-unknown-pragmas" )
#endif ( )


#
# Dependencies
#
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake" )

find_package( boost REQUIRED )
find_package( asio REQUIRED )
find_package( openssl REQUIRED )

#
# Includes
#
include_directories( ${INCLUDE_DIR} SYSTEM ${asio_INCLUDE} ${ssl_INCLUDE} )


#
# Sources
#
file( GLOB_RECURSE MANIFEST "${SOURCE_DIR}/*.cpp" )

#
# Build
#

# Static Library
set( STATIC_LIBRARY_NAME "${PROJECT_NAME}-static" )
add_library( ${STATIC_LIBRARY_NAME} STATIC ${MANIFEST} )
set_property( TARGET ${STATIC_LIBRARY_NAME} PROPERTY CXX_STANDARD 11 )
set_property( TARGET ${STATIC_LIBRARY_NAME} PROPERTY CXX_STANDARD_REQUIRED ON )
set_target_properties( ${STATIC_LIBRARY_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME} )
target_link_libraries( ${STATIC_LIBRARY_NAME} LINK_PRIVATE ${ssl_LIBRARY_STATIC} ${crypto_LIBRARY_STATIC} )


# Shared Library
set( SHARED_LIBRARY_NAME "${PROJECT_NAME}-shared" )
add_library( ${SHARED_LIBRARY_NAME} SHARED ${MANIFEST} )
set_property( TARGET ${SHARED_LIBRARY_NAME} PROPERTY CXX_STANDARD 11 )
set_property( TARGET ${SHARED_LIBRARY_NAME} PROPERTY CXX_STANDARD_REQUIRED ON )
set_target_properties( ${SHARED_LIBRARY_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME} )
set_target_properties( ${SHARED_LIBRARY_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR} VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR} )
target_link_libraries( ${SHARED_LIBRARY_NAME} LINK_PRIVATE ${ssl_LIBRARY_SHARED} ${crypto_LIBRARY_SHARED} )

#
# Test
#
enable_testing( )
add_subdirectory( "${PROJECT_SOURCE_DIR}/test/unit" )