set(BINARY_NAME laba07)

add_executable(${BINARY_NAME}
    src/main.cpp
    src/Mesh.cpp
    src/ResourceManager.cpp
    src/GraphicObject.cpp
    src/Camera.cpp
    src/Shader.cpp
    src/Texture.cpp
    src/RenderManager.cpp
    src/Light.cpp
    src/Material.cpp
    src/Scene.cpp
    src/FrustumCulling.cpp
    src/LevelOfDetail.cpp
)

target_include_directories(${BINARY_NAME} PRIVATE include ${RAPIDJSON_INCLUDE_DIRS})

target_link_libraries(${BINARY_NAME}
    OpenGL::GL
    GLEW::GLEW
    GLUT::GLUT
    ${IL_LIBRARIES}
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