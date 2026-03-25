#pragma once
#include <GL/glew.h>
#include <iostream>
#include "Vertex.h"
#include <vector>

using namespace std;

class Mesh
{
public:
    Mesh();
    bool load(const string& filename);
    void drawOne();
    
private:
    bool loadObjFile(const std::string& filename, std::vector<Vertex>& vertices, std::vector<GLuint>& indices);
    void createBuffers(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);
    
    GLuint vao;
    GLuint vertexBuffer;
    GLuint indexBuffer;
    int vertexCount;
};