cmake_minimum_required(VERSION 3.6)

project(VcTest)

set(CMAKE_CXX_STANDARD 11)

file(GLOB MYGL_INCLUDES dep/mygl3/*.hpp dep/mygl3/texture.cpp dep/mygl3/utils/*.hpp)
file(GLOB SOURCE_FILES *.cpp *.hpp)

add_executable(
		VcTest
		dep/GL/gl3w.c
		${MYGL_INCLUDES}
		${SOURCE_FILES})

include_directories(dep)
target_link_libraries(VcTest glfw ${CMAKE_DL_LIBS} GL assimp)
