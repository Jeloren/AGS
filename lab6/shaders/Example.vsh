#version 330 core

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTexCoord;

out vec3 position;
out vec3 normal;
out vec2 texCoord;

void main()
{
	// Рассчитываем позицию фрагмента в View Space
	position = vec3(modelViewMatrix * vec4(vPosition, 1.0));
	
	// Рассчитываем нормаль в View Space
	normal = vec3(modelViewMatrix * vec4(vNormal, 0.0));
	
	// Передаём координаты текстуры
	texCoord = vec2(vTexCoord.s, 1.0 - vTexCoord.t);
	
	// Рассчитываем итоговую позицию
	gl_Position = projectionMatrix * modelViewMatrix * vec4(vPosition, 1.0);
}

