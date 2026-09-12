cmake_minimum_required(VERSION 3.14)
project(glcpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(example
  example.cpp
)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLU REQUIRED)
find_package(GLUT REQUIRED)

target_link_libraries(example
  PRIVATE
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
)