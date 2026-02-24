cmake_minimum_required(VERSION 3.10)
project(AGS_Course)

set(CMAKE_CXX_STANDARD 17)
set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)

# Убедись, что имя папки совпадает символ в символ (включая пробелы)
add_subdirectory("lab1")
add_subdirectory("lab2")
add_subdirectory("lab3")