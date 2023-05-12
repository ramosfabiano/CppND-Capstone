cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)

project(cppnd-capstone VERSION "0.0.1" LANGUAGES CXX)

#find_program(CLANG_TIDY_EXE NAMES "clang-tidy" REQUIRED)
#set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "-checks=-*,cppcoreguidelines-*,readability-*")

add_subdirectory(src/lib)
add_subdirectory(src/app)
