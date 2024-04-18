cmake_minimum_required(VERSION 3.16)
project(data_and_operation_test)
set(CMAKE_CXX_STANDARD 17)

find_package(glog REQUIRED)
find_package(GTest REQUIRED)


set(link_lib glog::glog GTest::gtest)

if (!WIN32)
    set(link_lib "${link_lib} pthread")
endif ()
# set(DGLOG_ROOT_DIR $glog_source_root)
# set(DGFLAGS_ROOT_DIR $gflags_source_root)



add_executable(data_and_operation_test main.cpp)


target_link_libraries(data_and_operation_test ${link_lib})

target_include_directories(data_and_operation_test PUBLIC ${glog_INCLUDE_DIR})
target_include_directories(data_and_operation_test PUBLIC ${GTest_INCLUDE_DIR})
enable_testing()