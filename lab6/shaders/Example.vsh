#version 330 core

uniform	mat4 projectionMatrix;
uniform	mat4 modelViewMatrix;
uniform	mat3 normalMatrix;
uniform	vec4 color;

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTexCoord;

out vec2 texCoord;
out vec3 fragPosition;
out vec3 fragNormal;

void main ()
{
	texCoord = vec2(vTexCoord.s, 1.0 - vTexCoord.t);
	fragPosition = vec3(modelViewMatrix * vec4(vPosition, 1.0));
	fragNormal = normalize(normalMatrix * vNormal);
	gl_Position = projectionMatrix * modelViewMatrix * vec4 (vPosition, 1);
}

