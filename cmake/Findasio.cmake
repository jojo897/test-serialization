find_path( asio_INCLUDE asio.hpp HINTS "${PROJECT_SOURCE_DIR}/dependency/asio/asio/include" "C:\\Users\\jss\\Downloads\\boost_1_66_0\\boost" "/usr/include" "/usr/local/include" "/opt/local/include" )

if ( asio_INCLUDE )
    set( ASIO_FOUND TRUE )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DASIO_STANDALONE=YES" )

    message( STATUS "Found ASIO include at: ${asio_INCLUDE}" )
else ( )
    message( FATAL_ERROR "Failed to locate ASIO dependency." )
endif ( )