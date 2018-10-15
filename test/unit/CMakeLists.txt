cmake_minimum_required(VERSION 3.10)

#
# Configuration
#
set(UNIT_TEST_SRC_DIR ${PROJECT_SOURCE_DIR}/test/unit/src)

#
# Configuration
#
include_directories(SYSTEM ${asio_INCLUDE})
set(BUILD_SHARED_LIBS false)
find_package(Boost COMPONENTS unit_test_framework system REQUIRED)
find_library(BOOST_SYSTEM_STATIC libboost_system-vc141-mt-gd-x64-1_66.lib ${BOOST_ROOT}/stage/lib)
find_library(BOOST_DATE_TIME_STATIC libboost_date_time-vc141-mt-gd-x64-1_66.lib ${BOOST_ROOT}/stage/lib)
find_library(BOOST_UNIT_TEST_FRAMEWORK_STATIC libboost_unit_test_framework-vc141-mt-gd-x64-1_66.lib ${BOOST_ROOT}/stage/lib)

#
# Build
#
#add_definitions("-DCGAL_Boost_USE_STATIC_LIBS=true")

list(APPEND TEST_SOURCES
        http_server_test
        http_server_test_2)

foreach (file ${TEST_SOURCES})
    set(BINARY_NAME ${file})
    add_executable(${BINARY_NAME} ${UNIT_TEST_SRC_DIR}/${file}.cpp)
    target_link_libraries(${BINARY_NAME} ${STATIC_LIBRARY_NAME} ${BOOST_DATE_TIME_STATIC} ${BOOST_SYSTEM_STATIC} ${BOOST_UNIT_TEST_FRAMEWORK_STATIC})
    add_test(${BINARY_NAME} ${EXECUTABLE_OUTPUT_PATH}/${BINARY_NAME})
endforeach ()