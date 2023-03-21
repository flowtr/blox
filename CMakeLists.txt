cmake_minimum_required(VERSION 3.21)
project(luau_demo)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Download and use the luau library
include(FetchContent)
FetchContent_Declare(
  luau
  GIT_REPOSITORY https://github.com/Roblox/luau.git
  GIT_TAG 0.568
)
FetchContent_MakeAvailable(luau)

add_executable(luau_demo main.cpp)
target_link_libraries(luau_demo PRIVATE Luau.VM Luau.CodeGen Luau.Compiler Luau.Ast Luau.Analysis Luau.Common)
target_compile_options(luau_demo PRIVATE -Wall -Wextra -Wpedantic -D_FORTIFY_SOURCE=3)

