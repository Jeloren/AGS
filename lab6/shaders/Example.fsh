#version 330 core

uniform sampler2D texture_0;

// Параметры материала
uniform vec4 mAmbient;
uniform vec4 mDiffuse;
uniform vec4 mSpecular;
uniform float mShininess;

// Параметры первого света
uniform vec4 lPosition;  // Направление света в View Space
uniform vec4 lAmbient;
uniform vec4 lDiffuse;
uniform vec4 lSpecular;

// Параметры второго света
uniform vec4 lPosition2;  // Направление света в View Space
uniform vec4 lAmbient2;
uniform vec4 lDiffuse2;
uniform vec4 lSpecular2;

// Параметры тумана
uniform float minDistance;
uniform float maxDistance;
uniform vec4 fogColor;

in vec3 position;
in vec3 normal;
in vec2 texCoord;

out vec4 outputColor;

// Функция расчета компонент Phong для одного источника света
vec4 calculatePhong(vec3 n_Normal, vec3 n_ToEye, vec4 lightPos, vec4 lightAmb, vec4 lightDiff, vec4 lightSpec)
{
	// Вектор на свет
	vec3 n_ToLight = normalize(vec3(lightPos));
	
	// Отраженный вектор
	vec3 n_Reflect = normalize(reflect(-n_ToLight, n_Normal));
	
	// Компонента Фонга: Ambient
	vec4 ambient = mAmbient * lightAmb;
	
	// Компонента Фонга: Diffuse
	vec4 diffuse = mDiffuse * lightDiff * max(dot(n_Normal, n_ToLight), 0.0);
	
	// Компонента Фонга: Specular
	vec4 specular = mSpecular * lightSpec * pow(max(dot(n_ToEye, n_Reflect), 0.0), mShininess);
	
	return ambient + diffuse + specular;
}

void main()
{
	// Нормализуем входящий вектор нормали
	vec3 n_Normal = normalize(normal);
	
	// Вектор на наблюдателя (камера в начале координат View Space)
	vec3 n_ToEye = normalize(vec3(0.0, 0.0, 0.0) - position);
	
	// Расчетываем освещение от первого источника света
	vec4 phong1 = calculatePhong(n_Normal, n_ToEye, lPosition, lAmbient, lDiffuse, lSpecular);
	
	// Расчетываем освещение от второго источника света
	vec4 phong2 = calculatePhong(n_Normal, n_ToEye, lPosition2, lAmbient2, lDiffuse2, lSpecular2);
	
	// Объединяем освещение от обоих источников
	vec3 color = vec3(phong1 + phong2);
	
	// Применяем текстуру
	vec4 texColor = texture(texture_0, texCoord);
	color = color * vec3(texColor);
	
	// Рассчитываем коэффициент тумана
	float distance = length(position);
	float fogFactor = (maxDistance - distance) / (maxDistance - minDistance);
	fogFactor = clamp(fogFactor, 0.0, 1.0);
	
	// Смешиваем цвет с туманом
	vec3 finalColor = mix(vec3(fogColor), color, fogFactor);
	
	// Устанавливаем альфа-канал из диффузного цвета материала
	float alpha = mDiffuse.a;
	
	// Выводим итоговый цвет
	outputColor = vec4(finalColor, alpha);
}

