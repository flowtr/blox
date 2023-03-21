cmake_minimum_required(VERSION 3.21)
project(bobux)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Dependencies
include(FetchContent)
FetchContent_Declare(
	luau
	GIT_REPOSITORY https://github.com/Roblox/luau.git
	GIT_TAG master
	GIT_SHALLOW TRUE
	GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(luau)

FetchContent_Declare(
	Eigen
	GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
	GIT_TAG master
	GIT_SHALLOW TRUE
	GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(Eigen)
FetchContent_Declare(
	glfw
	GIT_REPOSITORY https://github.com/glfw/glfw
	GIT_SHALLOW TRUE
	GIT_PROGRESS TRUE
)

FetchContent_GetProperties(glfw)
if(NOT glfw_POPULATED)
	FetchContent_Populate(glfw)

	set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
	set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
	set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
	set(GLFW_INSTALL ON CACHE INTERNAL "Generate installation target")
	set(GLFW_USE_WAYLAND ON CACHE INTERNAL "Enable wayland support")

	add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
endif()

FetchContent_Declare(
	glad
	GIT_REPOSITORY https://github.com/Dav1dde/glad.git
)

FetchContent_GetProperties(glad)
if(NOT glad_POPULATED)
	FetchContent_Populate(glad)
	set(GLAD_PROFILE "core" CACHE STRING "OpenGL profile")
	set(GLAD_API "gl=" CACHE STRING "API type/version pairs, like \"gl=3.2,gles=\", no version means latest")
	set(GLAD_GENERATOR "c" CACHE STRING "Language to generate the binding for")
	add_subdirectory(${glad_SOURCE_DIR} ${glad_BINARY_DIR})
endif()

add_executable(bobux main.cpp)
target_link_libraries(
	bobux 
	PRIVATE
	Luau.VM Luau.CodeGen Luau.Compiler Luau.Ast Luau.Analysis Luau.Common
	eigen
	glfw
	glad
)
target_include_directories(
	bobux PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)
target_compile_options(bobux PRIVATE -Wall -Wextra -Wpedantic -D_FORTIFY_SOURCE=3)

