cmake_minimum_required(VERSION 3.16)
project(data_and_operation_test)
set(CMAKE_CXX_STANDARD 17)

aux_source_directory(. DIR_SOURCE_ARMA)

add_executable(
data_and_operation_test  ${DIR_SOURCE_ARMA} 
)

find_package(GTest REQUIRED)

set(link_lib GTest::gtest)

target_link_libraries(data_and_operation_test ${link_lib})

target_include_directories(data_and_operation_test PUBLIC ${GTest_INCLUDE_DIR})
enable_testing()