cmake_minimum_required(VERSION 2.8)
project(rpc)

set(CMAKE_CXX_STANDARD 17)

include_directories(/home/klc/build_lib/include/3partlib/zeromq/include/ /usr/local/include ./include)
link_directories(/home/klc/build_lib/lib/3partlib/ /usr/local/lib64/)

SET(CMAKE_BUILD_TYPE "Debug")

add_executable(main_server ./src/main_server.cpp)
target_link_libraries(main_server -lzmq)

add_executable(main_client ./src/main_client.cpp)
target_link_libraries(main_client -lzmq)
