cmake_minimum_required(VERSION 3.10)

#
# Configuration
#
set( UNIT_TEST_SRC_DIR ${PROJECT_SOURCE_DIR}/test/unit/src )

#
# Configuration
#
include_directories( SYSTEM ${asio_INCLUDE} )
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

#
# Build
#
#add_definitions("-DCGAL_Boost_USE_STATIC_LIBS=true")

set(BINARY_NAME SerializationTestSuite)
add_executable( ${BINARY_NAME} ${UNIT_TEST_SRC_DIR}/SerializationTestSuite.cpp )
target_link_libraries( ${BINARY_NAME} ${STATIC_LIBRARY_NAME} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test( ${BINARY_NAME} ${EXECUTABLE_OUTPUT_PATH}/${BINARY_NAME} )

