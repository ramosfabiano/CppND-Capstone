cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD 20)

project(cppnd-capstone VERSION "0.0.1" LANGUAGES CXX)

find_program(CLANG_TIDY_EXE NAMES "clang-tidy" REQUIRED)
set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "-checks=-*,cppcoreguidelines-*")

# boost
find_package(Boost REQUIRED)

add_subdirectory(src/lib)
add_subdirectory(src/app)
