cmake_minimum_required(VERSION 3.30)
project(Hash_Table_From_Scratch_Cpp)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(Hash_Table_From_Scratch_Cpp
        src/hash_table_fixed.hpp
    src/main.cpp
    src/MurmurHash3.h
    src/MurmurHash3.cpp)
