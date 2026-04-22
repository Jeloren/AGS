#pragma once
#include <string>
#include <vector>
#include <map>
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"

using namespace std;

// ����� ��� ������ � ���������� �������� (���������� �� ������ ������� SINGLTON)
class ResourceManager
{
public:
	// ������-����� ��� ��������� ���������� ��������� �������.
	// ������ ����� ���������� ������ �� ������, ���������� � ������������ ����������.
	static ResourceManager& instance()
	{
		static ResourceManager ResourceManager;
		return ResourceManager;
	}
	// �������� ������ ���� �� ���������� obj-�����
	// ������������ �������� - ������ ���� � ��������� ��������
	int loadMesh(const string& filename);
	// ��������� ���� �� ��� �������
	// ���� ������ ���� ��� (���������������� ������) ������������ nullptr
	Mesh* getMesh(int index);

	// Загружает текстуру из файла изображения
	// возвращаемое значение - индекс текстуры в результирующем массиве
	int loadTexture(const string& filename);
	// получит текстуру по её индексу
	// если индекс выходит за границы возвращает nullptr
	Texture* getTexture(int index);

	// Загружает материал (пока hardcoded, можно расширить для файлов)
	int loadMaterial(const string& name);
	Material* getMaterial(int index);

	void printDebugInfo() const;
private:
	// конструктор по умолчанию (приватный конструктор)
	// в этом случае никто не сможет создать экземпляр класса кроме самого класса
	ResourceManager() {};
	// удаляем конструктор копирования (защита)
	ResourceManager(const ResourceManager& v) = delete;
	// удаляем оператор копирования (защита)
	ResourceManager& operator=(const ResourceManager& v) = delete;
private:
	// вектор для хранения всех мешей
	vector<Mesh> meshes;
	// map для быстрого поиска индекса сетки по имени загруженного файла
	// и получении из вектора meshes
	map <string, int> meshes_id;

	// вектор для хранения всех текстур
	vector<Texture> textures;
	// map для быстрого поиска индекса текстуры по имени загруженного файла
	map <string, int> textures_id;

	// вектор для хранения всех материалов
	vector<Material> materials;
	// map для быстрого поиска индекса материала по имени
	map <string, int> materials_id;
};