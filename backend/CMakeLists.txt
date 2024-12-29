cmake_minimum_required(VERSION 3.20)
project(server)

set(CMAKE_CXX_STANDARD 23)

add_executable(server main.cpp src/thread_pool.cpp src/file_storage.cpp src/inverted_index.cpp src/indexer.cpp src/web_server.cpp src/utils.h)

add_executable(client client.cpp)

target_link_libraries(server stdc++fs pthread)
