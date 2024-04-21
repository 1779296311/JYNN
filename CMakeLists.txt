cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(JYNN)

# add_executable(
#     JYNN
#     main.cpp
# )

# target_include_directories(
#     JYNN
#     PRIVATE
#     ./
# )


include_directories(
    ./
)
add_subdirectory(./test)

