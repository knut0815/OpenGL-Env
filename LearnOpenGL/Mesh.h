#ifndef __LearnOpenGL__Mesh__
#define __LearnOpenGL__Mesh__

#include <sstream>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>
#include "Image.h"
#include "GlslProgram.h"

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

struct Texture
{
    Image img;
    std::string type;
    aiString path;
};

class Mesh
{

public:
    
    Mesh(const std::vector<Vertex> &meshVertices, const std::vector<GLuint> &meshIndices, const std::vector<Texture> &meshTextures);
    const std::vector<Vertex> &getVertices() const { return vertices; }
    const std::vector<GLuint> &getIndices() const { return indices; }
    const std::vector<Texture> &getTextures() const { return textures; }
    void draw(GlslProgram &program) const;
    
private:
    
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    
    // Render data
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    
    void setupMesh();
};

#endif
