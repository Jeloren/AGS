#include "ResourceManager.h"
#include <iostream>

// �������� ���� �� �����
int ResourceManager::loadMesh(const string& filename) {
    // ���������, ��� �� ��� �������� ���� ���
    auto it = meshes_id.find(filename);
    if (it != meshes_id.end()) {
        cout << "Mesh already loaded: " << filename << " (index: " << it->second << ")\n";
        return it->second; // ���������� ������ ��� ������������ ����
    }

    // ������� ����� ��� � ��������� ��� �� �����
    Mesh mesh;
    if (!mesh.load(filename)) {
        cerr << "Failed to load mesh: " << filename << "\n";
        return -1; // ���������� -1 � ������ ������
    }

    // ��������� ��� � ������ � ��������� ��� ������
    int index = static_cast<int>(meshes.size());
    meshes.push_back(mesh);
    meshes_id[filename] = index;

    cout << "Mesh successfully loaded: " << filename << " (index: " << index << ")\n";
    return index;
}

// ��������� ���� �� �������
Mesh* ResourceManager::getMesh(int index) {
    if (index < 0 || index >= meshes.size()) {
        cerr << "Invalid mesh index: " << index << "\n";
        return nullptr;
    }
    return &meshes[index];
}

// ����� ���������� ����������
void ResourceManager::printDebugInfo() const {
    cout << "Debug information:\n";
    cout << "Loaded meshes: " << meshes.size() << "\n";
    for (const auto& pair : meshes_id) {
        cout << "File: " << pair.first << " -> Index: " << pair.second << "\n";
    }
    cout << "Loaded textures: " << textures.size() << "\n";
    for (const auto& pair : textures_id) {
        cout << "File: " << pair.first << " -> Index: " << pair.second << "\n";
    }
    cout << "Loaded materials: " << materials.size() << "\n";
    for (const auto& pair : materials_id) {
        cout << "Name: " << pair.first << " -> Index: " << pair.second << "\n";
    }
}

// ��������� ����� �� ����
int ResourceManager::loadTexture(const string& filename) {
    // ���������, ��� �� ��� ��������� ������ ���
    auto it = textures_id.find(filename);
    if (it != textures_id.end()) {
        cout << "Texture already loaded: " << filename << " (index: " << it->second << ")\n";
        return it->second; // ���������� ������ ��� ���� ������
    }

    // ������� ����� ������ � ��������� ��� �� ����
    Texture texture;
    texture.load(filename);

    // ��������� ������ � ������ � ��������� ��� ������
    int index = static_cast<int>(textures.size());
    textures.push_back(texture);
    textures_id[filename] = index;

    return index;
}

// ��������� ������ �� ������
Texture* ResourceManager::getTexture(int index) {
    if (index < 0 || index >= textures.size()) {
        cerr << "Invalid texture index: " << index << "\n";
        return nullptr;
    }
    return &textures[index];
}

// Загрузка материала (hardcoded для простоты)
int ResourceManager::loadMaterial(const string& name) {
    auto it = materials_id.find(name);
    if (it != materials_id.end()) {
        cout << "Material already loaded: " << name << " (index: " << it->second << ")\n";
        return it->second;
    }

    Material material;
    if (name == "plastic") {
        material = Material(glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);
    } else if (name == "metal") {
        material = Material(glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 32.0f);
    } else if (name == "wood") {
        material = Material(glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.6f, 0.4f, 0.2f), glm::vec3(0.1f, 0.1f, 0.1f), 8.0f);
    } else {
        // Default material
        material = Material();
    }

    int index = static_cast<int>(materials.size());
    materials.push_back(material);
    materials_id[name] = index;

    cout << "Material loaded: " << name << " (index: " << index << ")\n";
    return index;
}

Material* ResourceManager::getMaterial(int index) {
    if (index < 0 || index >= materials.size()) {
        cerr << "Invalid material index: " << index << "\n";
        return nullptr;
    }
    return &materials[index];
}