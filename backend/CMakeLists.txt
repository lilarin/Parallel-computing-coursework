cmake_minimum_required(VERSION 3.20)
project(server)

set(CMAKE_CXX_STANDARD 17)

add_executable(server main.cpp src/thread_pool.cpp src/file_storage.cpp src/inverted_index.cpp src/indexer.cpp src/web_server.cpp src/utils.h)
set_target_properties(server PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

add_executable(client client.cpp)
set_target_properties(server PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

target_link_libraries(server stdc++fs pthread)
