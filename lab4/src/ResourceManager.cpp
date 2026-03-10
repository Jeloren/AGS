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
}