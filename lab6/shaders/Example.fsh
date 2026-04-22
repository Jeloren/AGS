#version 330 core

uniform	vec4 color;
uniform sampler2D texture_0;

// Material properties
uniform vec3 materialAmbient;
uniform vec3 materialDiffuse;
uniform vec3 materialSpecular;
uniform float materialShininess;

// Light 1 properties
uniform vec3 lightPosition;
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;

// Light 2 properties
uniform vec3 lightPosition2;
uniform vec3 lightAmbient2;
uniform vec3 lightDiffuse2;
uniform vec3 lightSpecular2;

in vec2 texCoord;
in vec3 fragPosition;
in vec3 fragNormal;

out vec4 outputColor;

vec3 calculateLight(vec3 lightPos, vec3 lightAmb, vec3 lightDiff, vec3 lightSpec)
{
	// Ambient
	vec3 ambient = lightAmb * materialAmbient;

	// Diffuse
	vec3 norm = normalize(fragNormal);
	vec3 lightDir = normalize(lightPos - fragPosition);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = lightDiff * (diff * materialDiffuse);

	// Specular
	vec3 viewDir = normalize(-fragPosition); // Camera is at (0,0,0) in view space
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
	vec3 specular = lightSpec * (spec * materialSpecular);

	return ambient + diffuse + specular;
}

void main (void)
{
	vec4 texColor = texture(texture_0, texCoord);

	// Calculate contribution from both lights
	vec3 result = calculateLight(lightPosition, lightAmbient, lightDiffuse, lightSpecular);
	result += calculateLight(lightPosition2, lightAmbient2, lightDiffuse2, lightSpecular2);

	outputColor = vec4(result, 1.0) * texColor * color;
}

