cmake_minimum_required(VERSION 3.10)
project(laba04)

set(BINARY_NAME laba04)

# НАЙТИ ПАКЕТЫ - ЭТО КЛЮЧЕВОЕ!
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)

# Теперь можно использовать импортированные targets
add_executable(${BINARY_NAME} 
    src/main.cpp
    src/Mesh.cpp
    src/ResourceManager.cpp
    src/GraphicObject.cpp
    src/Camera.cpp
    src/Shader.cpp
)

target_include_directories(${BINARY_NAME} PRIVATE include)

target_link_libraries(${BINARY_NAME}
    OpenGL::GL
    GLEW::GLEW
    GLUT::GLUT
)

add_custom_command(TARGET ${BINARY_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders
    $<TARGET_FILE_DIR:${BINARY_NAME}>/shaders
)
add_custom_command(TARGET ${BINARY_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/example
    $<TARGET_FILE_DIR:${BINARY_NAME}>/example
)