cmake_minimum_required(VERSION 3.14)
project(glcpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)
find_package(Eigen3 REQUIRED NO_MODULE)

add_executable(example
  example.cpp
)

target_link_libraries(example PRIVATE
  OpenGL::GL
  GLEW::GLEW
  GLUT::GLUT
  Eigen3::Eigen
)