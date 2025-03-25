cmake_minimum_required(VERSION 3.30)
project(Hash_Table_From_Scratch_Cpp)

set(CMAKE_CXX_STANDARD 20)

include_directories(src)

add_executable(Hash_Table_From_Scratch_Cpp
        src/hash_table_fixed.hpp
        src/hash_table_dynamic.hpp
    src/main.cpp)
