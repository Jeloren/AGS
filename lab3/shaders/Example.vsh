#version 330 core

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

// Явно указываем location = 0, чтобы связать с glVertexAttribPointer
layout(location = 0) in vec3 vPosition; 

void main ()
{
    gl_Position = projectionMatrix * modelViewMatrix * vec4(vPosition, 1.0);
}