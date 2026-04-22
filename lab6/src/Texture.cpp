#include "Texture.h"
#include <IL/il.h>
#include <IL/ilu.h>

Texture::Texture() : texIndex(0)
{
}

void Texture::load(const string& filename)
{
	// Генерируем текстурный объект
	glGenTextures(1, &texIndex);
	// Привязываем текстуру к GL_TEXTURE_2D
	glBindTexture(GL_TEXTURE_2D, texIndex);

	// Устанавливаем параметр выравнивания для корректной загрузки
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Загружаем изображение с помощью DevIL
	ILuint imageId;
	ilGenImages(1, &imageId);
	ilBindImage(imageId);

	// Загружаем файл изображения
	if (!ilLoadImage(filename.c_str())) {
		cerr << "Failed to load image: " << filename << "\n";
		ilDeleteImages(1, &imageId);
		return;
	}

	// Конвертируем изображение в RGBA формат для OpenGL
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

	// Загружаем данные в видеопамять
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), 
	             ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE, 
	             ilGetData());

	// Генерируем мипмапы для лучшего качества при уменьшении
	glGenerateMipmap(GL_TEXTURE_2D);

	// Устанавливаем параметры фильтрации
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Очищаем ресурсы DevIL
	ilDeleteImages(1, &imageId);

	cout << "Texture successfully loaded: " << filename << " (id: " << texIndex << ")\n";
}

void Texture::bind(GLenum texUnit)
{
	// Активируем нужный текстурный блок
	glActiveTexture(texUnit);
	// Привязываем текстуру
	glBindTexture(GL_TEXTURE_2D, texIndex);
}
