cmake_minimum_required(VERSION 3.13)

project(http-server-starter-cpp)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.hpp)

set(CMAKE_CXX_STANDARD 23) # Enable the C++23 standard
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)
find_package(ZLIB REQUIRED)

add_executable(server ${SOURCE_FILES}
        src/ConnectionHandler.cpp
        include/ConnectionHandler.h
        src/HTTPMessageHandler.cpp
        include/HTTPMessageHandler.h
        src/GzipUtils.cpp
        include/GzipUtils.h
        src/HTTPMessage.cpp
        include/HTTPMessage.h)

target_link_libraries(server PRIVATE Threads::Threads ZLIB::ZLIB)
