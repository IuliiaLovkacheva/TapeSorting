cmake_minimum_required(VERSION 3.25)
project(tape-sorting)

set(CMAKE_CXX_STANDARD 17)

add_executable(tape-sorting main.cpp file-tape.cpp sort.cpp counts-collection.h tape-counts-collection.h tape-counts-collection.h vector-counts-collection.h vector-counts-collection.h vector-counts-collection.cpp tape-counts-collection.cpp delays.cpp utils.h utils.cpp value.h tape-value.h memory-value.h memory-value.cpp tape-value.cpp)
