#include "Mesh.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

Mesh::Mesh() : vao(0), vertexBuffer(0), indexBuffer(0), vertexCount(0) {}

static int parseIndex(const string& token) {
    if (token.empty()) return -1;
    try {
        int index = stoi(token);
        return index - 1;
    }
    catch (...) {
        return -1;
    }
}

bool Mesh::load(const  string& filename) {
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

bool Mesh::loadObjFile(const  string& filename,  vector<Vertex>& vertices,  vector<GLuint>& indices) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Can't open file: " << filename <<  endl;
        return false;
    }

    vector<vec3> tempCoords;
    vector<vec3> tempNormals;
    vector<vec2> tempTexCoords;
    struct FaceIndex { int coordIndex; int texIndex; int normalIndex; };
    vector<FaceIndex> faceIndices;

    string line;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

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
            vector<FaceIndex> face;
            string token;
            while (iss >> token) {
                FaceIndex idx = {-1, -1, -1};
                size_t firstSlash = token.find('/');
                size_t secondSlash = string::npos;
                if (firstSlash != string::npos) {
                    secondSlash = token.find('/', firstSlash + 1);
                }

                if (firstSlash == string::npos) {
                    idx.coordIndex = parseIndex(token);
                } else if (secondSlash == string::npos) {
                    idx.coordIndex = parseIndex(token.substr(0, firstSlash));
                    idx.texIndex = parseIndex(token.substr(firstSlash + 1));
                } else {
                    idx.coordIndex = parseIndex(token.substr(0, firstSlash));
                    idx.texIndex = parseIndex(token.substr(firstSlash + 1, secondSlash - firstSlash - 1));
                    idx.normalIndex = parseIndex(token.substr(secondSlash + 1));
                }
                face.push_back(idx);
            }

            if (face.size() < 3) continue;
            for (size_t i = 1; i + 1 < face.size(); ++i) {
                faceIndices.push_back(face[0]);
                faceIndices.push_back(face[i]);
                faceIndices.push_back(face[i + 1]);
            }
        }
    }

    vertices.reserve(faceIndices.size());
    indices.reserve(faceIndices.size());

    for (size_t i = 0; i < faceIndices.size(); ++i) {
        const FaceIndex& idx = faceIndices[i];
        Vertex vertex = {};

        if (idx.coordIndex >= 0 && idx.coordIndex < static_cast<int>(tempCoords.size())) {
            vertex.coord[0] = tempCoords[idx.coordIndex].x;
            vertex.coord[1] = tempCoords[idx.coordIndex].y;
            vertex.coord[2] = tempCoords[idx.coordIndex].z;
        }
        else {
            vertex.coord[0] = vertex.coord[1] = vertex.coord[2] = 0.0f;
        }

        if (idx.normalIndex >= 0 && idx.normalIndex < static_cast<int>(tempNormals.size())) {
            vertex.normal[0] = tempNormals[idx.normalIndex].x;
            vertex.normal[1] = tempNormals[idx.normalIndex].y;
            vertex.normal[2] = tempNormals[idx.normalIndex].z;
        }
        else {
            vertex.normal[0] = 0.0f;
            vertex.normal[1] = 0.0f;
            vertex.normal[2] = 1.0f;
        }

        if (idx.texIndex >= 0 && idx.texIndex < static_cast<int>(tempTexCoords.size())) {
            vertex.texCoord[0] = tempTexCoords[idx.texIndex].x;
            vertex.texCoord[1] = tempTexCoords[idx.texIndex].y;
        }
        else {
            vertex.texCoord[0] = 0.0f;
            vertex.texCoord[1] = 0.0f;
        }

        vertices.push_back(vertex);
        indices.push_back(static_cast<GLuint>(i));
    }

    vertexCount = static_cast<int>(indices.size());
    return true;
}

void Mesh::createBuffers(const  vector<Vertex>& vertices, const  vector<GLuint>& indices) {
    // �������� VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // �������� VBO ��� ������
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // ��������� ��������� ������
    // ������� 0: ����������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, coord));
    glEnableVertexAttribArray(0);

    // ������� 1: �������
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    // ������� 2: ���������� ����������
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(2);

    // �������� IBO ��� ��������
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    // ������� VAO
    glBindVertexArray(0);
}