#include "Mesh.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

Mesh::Mesh() : vao(0), vertexBuffer(0), indexBuffer(0), vertexCount(0) {}

bool Mesh::load(const string& filename) {
    vector<Vertex> vertices;
    vector<GLuint> indices;

    if (!loadObjFile(filename, vertices, indices)) {
        cerr << "Failed to load mesh data from file: " << filename << endl;
        return false;
    }

    createBuffers(vertices, indices);
    return true;
}

void Mesh::drawOne() {
    if (vao == 0) return;
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

// ВСПОМОГАТЕЛЬНАЯ ФУНКЦИЯ ДЛЯ ПАРСИНГА ОДНОЙ ВЕРШИНЫ ГРАНИ
void parseFaceVertex(const string& faceVertex, 
                     GLuint& coordIndex, 
                     GLuint& texIndex, 
                     GLuint& normIndex) {
    string fv = faceVertex;
    replace(fv.begin(), fv.end(), '/', ' ');
    istringstream iss(fv);
    
    iss >> coordIndex;
    
    // Пытаемся прочитать текстуру
    if (iss.peek() != EOF) {
        iss >> texIndex;
    } else {
        texIndex = 0;
    }
    
    // Пытаемся прочитать нормаль
    if (iss.peek() != EOF) {
        iss >> normIndex;
    } else {
        normIndex = 0;
    }
}

bool Mesh::loadObjFile(const string& filename, vector<Vertex>& vertices, vector<GLuint>& indices) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Can't open file: " << filename << endl;
        return false;
    }

    vector<vec3> tempCoords;
    vector<vec3> tempNormals;
    vector<vec2> tempTexCoords;
    vector<GLuint> coordIndices, texCoordIndices, normalIndices;

    string line;
    int lineNum = 0;
    
    while (getline(file, line)) {
        lineNum++;
        istringstream iss(line);
        string type;
        iss >> type;

        if (type == "v") {
            vec3 coord;
            iss >> coord.x >> coord.y >> coord.z;
            tempCoords.push_back(coord);
        }
        else if (type == "vn") {
            vec3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            tempNormals.push_back(normal);
        }
        else if (type == "vt") {
            vec2 texCoord;
            iss >> texCoord.x >> texCoord.y;
            tempTexCoords.push_back(texCoord);
        }
        else if (type == "f") {
            vector<string> faceVertices;
            string vertexData;
            
            // Собираем все вершины грани
            while (iss >> vertexData) {
                faceVertices.push_back(vertexData);
            }
            
            // Если грань из 4 вершин, разбиваем на 2 треугольника
            if (faceVertices.size() == 4) {
                // Первый треугольник: вершины 0, 1, 2
                for (int i = 0; i < 3; i++) {
                    GLuint coordIdx, texIdx, normIdx;
                    parseFaceVertex(faceVertices[i], coordIdx, texIdx, normIdx);
                    coordIndices.push_back(coordIdx - 1);
                    texCoordIndices.push_back(texIdx > 0 ? texIdx - 1 : 0);
                    normalIndices.push_back(normIdx > 0 ? normIdx - 1 : 0);
                }
                // Второй треугольник: вершины 0, 2, 3
                int tri2[] = {0, 2, 3};
                for (int i = 0; i < 3; i++) {
                    GLuint coordIdx, texIdx, normIdx;
                    parseFaceVertex(faceVertices[tri2[i]], coordIdx, texIdx, normIdx);
                    coordIndices.push_back(coordIdx - 1);
                    texCoordIndices.push_back(texIdx > 0 ? texIdx - 1 : 0);
                    normalIndices.push_back(normIdx > 0 ? normIdx - 1 : 0);
                }
            }
            else if (faceVertices.size() == 3) {
                for (const string& fv : faceVertices) {
                    GLuint coordIdx, texIdx, normIdx;
                    parseFaceVertex(fv, coordIdx, texIdx, normIdx);
                    coordIndices.push_back(coordIdx - 1);
                    texCoordIndices.push_back(texIdx > 0 ? texIdx - 1 : 0);
                    normalIndices.push_back(normIdx > 0 ? normIdx - 1 : 0);
                }
            }
        }
    }
    
    // Проверяем, есть ли нормали
    bool hasNormals = !tempNormals.empty();
    
    // Создаем вершины
    for (size_t i = 0; i < coordIndices.size(); ++i) {
        Vertex vertex;
        
        // Координаты
        if (coordIndices[i] < tempCoords.size()) {
            vertex.coord[0] = tempCoords[coordIndices[i]].x;
            vertex.coord[1] = tempCoords[coordIndices[i]].y;
            vertex.coord[2] = tempCoords[coordIndices[i]].z;
        } else {
            cerr << "ERROR: coord index out of range: " << coordIndices[i] << endl;
            return false;
        }
        
        // Нормали (если есть)
        if (hasNormals && normalIndices[i] < tempNormals.size()) {
            vertex.normal[0] = tempNormals[normalIndices[i]].x;
            vertex.normal[1] = tempNormals[normalIndices[i]].y;
            vertex.normal[2] = tempNormals[normalIndices[i]].z;
        } else {
            // Вычисляем простую нормаль (по умолчанию вверх)
            vertex.normal[0] = 0.0f;
            vertex.normal[1] = 1.0f;
            vertex.normal[2] = 0.0f;
        }
        
        // Текстурные координаты
        if (!tempTexCoords.empty() && texCoordIndices[i] < tempTexCoords.size()) {
            vertex.texCoord[0] = tempTexCoords[texCoordIndices[i]].x;
            vertex.texCoord[1] = tempTexCoords[texCoordIndices[i]].y;
        } else {
            vertex.texCoord[0] = 0.0f;
            vertex.texCoord[1] = 0.0f;
        }
        
        vertices.push_back(vertex);
    }
    
    // Индексы
    for (size_t i = 0; i < vertices.size(); ++i) {
        indices.push_back(i);
    }
    
    vertexCount = static_cast<int>(indices.size());
    
    cout << "Loaded " << filename << ": " << vertices.size() << " vertices, " 
         << indices.size() << " indices" << endl;
    
    return true;
}

void Mesh::createBuffers(const vector<Vertex>& vertices, const vector<GLuint>& indices) {
    // Создание VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Создание VBO для вершин
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // Настройка атрибутов вершин
    // Атрибут 0: координаты
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, coord));
    glEnableVertexAttribArray(0);

    // Атрибут 1: нормали
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    // Атрибут 2: текстурные координаты
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(2);

    // Создание IBO для индексов
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    // Отвязываем VAO
    glBindVertexArray(0);
}