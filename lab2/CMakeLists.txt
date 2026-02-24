set(BINARY_NAME laba02)

add_executable(${BINARY_NAME} 
    src/main.cpp 
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