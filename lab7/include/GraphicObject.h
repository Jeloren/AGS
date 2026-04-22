#pragma once
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
using namespace std;

// Перечисление типов объектов для уровней детализации
enum class ObjectType {
    SMALL_NATURE,   // маленькая природа
    BIG_PROP,       // большой проп
    MEDIUM_PROP,    // средний проп
    SMALL_PROP      // маленький проп
};

// ����� ��� ������ � ����������� ��������
class GraphicObject
{
public:
	// ����������� �� ���������
	GraphicObject();
	// ���������� ���� �������
	void setColor(const vec4& color);
	// ���������� ������� �������
	void setPosition(const vec3& position);
	// ���������� ���� �������� � �������� ������������ ��� OY �� ������� �������
	void setAngle(float degree);
	// ���������� ������ ����� ��� ����
	void setRotation(float angleX, float angleY, float angleZ);
	// ���������� ������ ���������
	void setScale(float scale);
	void setScale(float scaleX, float scaleY, float scaleZ);
	// ���������� ������������� ������������� ����
	void setMeshId(int id);
	// ���������� ������������� ������
	void setTextureId(int id);
	// ���������� ID ���������
	void setMaterialId(int id);
	// ���������� ��� ������� ������
	void setObjectType(ObjectType type);
	// ���������� ��������� �������
	void setDimensions(const vec3& dims);
	// �������� ��������� ���������
	vec4& getColor();
	const vec4& getColor() const;
	mat4& getModelMatrix();
	const mat4& getModelMatrix() const;
	int getMeshId() const;
	// ������ ID ������
	int getTextureId() const;
	// ������ ID ���������
	int getMaterialId() const;
	// Получение позиции объекта
	vec3 getPosition() const;
	// ������ ��� ������� ������
	ObjectType getObjectType() const;
	// ������ ��������� �������
	vec3 getDimensions() const;
private:
	// ������������� ������������� ����
	int meshId;
	// ������������� ������
	int textureId;
	// ������������� ���������
	int materialId;
	// ��� ������� ������
	ObjectType objectType;
	// ��������� �������
	vec3 dimensions;
	// ���� �������
	vec4 color;
	// ������� ������ (������ ������� � ����������)
	mat4 modelMatrix;
	// ������ ��� ������ ���� trensformations
	vec3 position;
	float angleY;
	float angleX, angleZ;
	vec3 scale;
	// ��� ���� ������ ���� trensformations ��� ���
	void updateModelMatrix();
};