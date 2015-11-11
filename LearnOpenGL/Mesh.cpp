#include "Mesh.h"

// ===============================
// Public member functions
// ===============================

Mesh::Mesh(const std::vector<Vertex> &meshVertices, const std::vector<GLuint> &meshIndices, const std::vector<Texture> &meshTextures) : vertices(meshVertices), indices(meshIndices), textures(meshTextures)
{
    setupMesh();
}

void Mesh::draw(GlslProgram &program) const
{
    GLuint id = 0;
    for (const auto &tex: textures)
    {
        std::stringstream ss;
        ss << id++;
        program.setUniformSampler2D("material." + tex.type + ss.str(), tex.img, id);
    }
    glActiveTexture(GL_TEXTURE0);
    
    // Draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    
    // Unbind the VAO
    glBindVertexArray(0);
}

// ===============================
// Private member functions
// ===============================

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
    
    /*
     * Structs have a great property in C++ that their memory layout is sequential.
     * Thanks to this useful property we can directly pass a pointer to a large list 
     * of Vertex structs as the buffer's data and they translate perfectly to what 
     * glBufferData expects as its argument.
     */
    
    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
   
    // Vertex Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
    
    // Vertex Texture Coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoord));
    
    glBindVertexArray(0);
}